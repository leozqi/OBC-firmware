#include "ground_state_machine.h"
#include "hackRF_utils.h"
#include "gs_errors.h"
#include "obc_gs_ax25.h"

#define MAX_AX25_BUFFER_SIZE 200

typedef gs_error_code_t* ground_state_handler(void);

static gs_error_code_t GSStateDisconnectedHandler();
static gs_error_code_t GSStateSendConnHandler();
static gs_error_code_t GSStateAwaitConnAckHandler();
static gs_error_code_t GSStateUplinkCommands();
static gs_error_code_t GSStateDowlinkTelemtry();
static gs_error_code_t GSStateSendDiscAck();

static ground_state_handler* state_handlers[] = {
    [GS_STATE_DISCONNECTED] = GSStateDisconnectedHandler,   [GS_STATE_SEND_CONN] = GSStateSendConnHandler,
    [GS_STATE_AWAIT_CONN_ACK] = GSStateAwaitConnAckHandler, [GS_STATE_UPLINK_COMMANDS] = GSStateUplinkCommands,
    [GS_STATE_DOWNLINK_TELEMTRY] = GSStateDowlinkTelemtry,  [GS_STATE_SEND_DISC_ACK] = GSStateSendDiscAck};

gs_error_code_t updateGroundStationState(ground_station_state_t* state, ground_station_event_t event) {
  ground_station_state_t nextState = GS_STATE_DISCONNECTED;

  switch (*state) {
    case GS_STATE_DISCONNECTED:
      switch (event) {
        case GS_EVENT_UPLINK_BEGIN:
          UPDATE_STATE(state, GS_STATE_SEND_CONN)
      };
    case GS_STATE_SEND_CONN:
      switch (event) {}
    default:
      return GS_ERR_CODE_INVALID_STATE;
  }

  *state = nextState;
}
