#include "param_manager.h"
#include "param_table.h"

#include <FreeRTOS.h>
#include <os_portmacro.h>
#include <os_semphr.h>

#include <string.h>

static param_handle_t paramTableHandler = paramTable;
static const uint16_t paramTableLength = sizeof(paramTable) / sizeof(param_t);
static SemaphoreHandle_t paramTableMutex = NULL;

uint8_t initParamManager(void) {
    if ( paramTableMutex == NULL ) {
        paramTableMutex = xSemaphoreCreateMutex();
        return 1;
    }
    return 0;
}

uint8_t getParam(param_names_t paramName, param_handle_t param) {
    param_handle_t paramHandle = getParamHandle(paramName);
    if ( paramHandle == NULL ) {
        return 0;
    }

    if ( xSemaphoreTake(paramTableMutex, portMAX_DELAY) == pdTRUE ) {
        memcpy(param, paramHandle, sizeof(param_t));
        xSemaphoreGive(paramTableMutex);
        return 1;
    }

    return 0;
}

uint8_t getParamByIndex(uint16_t paramIndex, param_handle_t param) {
    return getParam((param_names_t)paramIndex, param);
}

uint8_t getParamVal(param_names_t paramName, param_type_t paramType, void *out)
{
    uint8_t status = accessParamTable(GET_PARAM, paramName, paramType, out);
    xSemaphoreGive(paramTableMutex);
    return status;
}

uint8_t getParamValByIndex(uint16_t paramIndex, param_type_t paramType, void *out) {
    uint8_t status = getParamVal((param_names_t)paramIndex, paramType, out);
    return status;
}

uint8_t setParamVal(param_names_t paramName, param_type_t paramType, void *in)
{
    uint8_t status = accessParamTable(SET_PARAM, paramName, paramType, in);
    xSemaphoreGive(paramTableMutex);
    return status;
}

static uint8_t accessParamTable(access_type_t accessType, param_names_t paramName, param_type_t paramType, void *p)
{
    param_handle_t paramHandle = getParamHandle(paramName);
    if ( paramHandle == NULL ) {
        return 0;
    }

    param_size_t paramSize = getParamSize(paramType);
    if ( paramHandle->type != paramType || paramSize == 0 ) {
        return 0;
    }

    if (paramTableMutex == NULL) {
        return 0;
    }

    /* TODO: We may want to change the delay time instead of using the max delay */
    if ( xSemaphoreTake(paramTableMutex, portMAX_DELAY) == pdTRUE ) {
        param_val_t paramVal = paramHandle->value;

        switch (accessType) {
            case SET_PARAM:
                memcpy(&(paramHandle->value), p, paramSize);
                break;
            case GET_PARAM:
                memcpy(p, &paramVal, paramSize);
                break;
            default:
                return 0;
        }
    }

    return 1;
}

static param_handle_t getParamHandle(param_names_t paramName)
{
    if ( paramName < 0 || paramName >= paramTableLength ) {
        return NULL;
    }

    return &paramTableHandler[paramName];
}

static param_size_t getParamSize(param_type_t type)
{
    switch (type)
    {
    case UINT8_PARAM:
        return UINT8_SIZE;
    case INT8_PARAM:
        return INT8_SIZE;
    case UINT16_PARAM:
        return UINT16_SIZE;
    case INT16_PARAM:
        return INT16_SIZE;
    case UINT32_PARAM:
        return UINT32_SIZE;
    case INT32_PARAM:
        return INT32_SIZE;
    case UINT64_PARAM:
        return UINT64_SIZE;
    case INT64_PARAM:
        return INT64_SIZE;
    case FLOAT_PARAM:
        return FLOAT_SIZE;
    case DOUBLE_PARAM:
        return DOUBLE_SIZE;
    /* case STRING_PARAM: */
    /*     return STRING_SIZE; */
    default:
        return 0;
    }
}

static uint8_t isReadOnly(param_handle_t paramHandle) {
    return ( paramHandle->opts & READ_ONLY );
}
static uint8_t isTelemetry(param_handle_t paramHandle) {
    return ( paramHandle->opts & TELEMETRY );
}
static uint8_t isPersistent(param_handle_t paramHandle) {
    return ( paramHandle->opts & PERSISTENT );
}