/**
 * SM5100B-D usage library
 */
#ifndef sm5100b_h

#define sm5100b_h

#include <SoftwareSerial.h>
#include "SM5100BClient.h"

enum {
  SM_SIM_REMOVED,
  SM_SIM_INSERTED,
  SM_RING_MELODY,
  SM_CALL_READY,
  SM_SMS_READY,
  SM_ID_RELEASED_CALLS,
  SM_RELEASED_CALL_ID,
  SM_NETWORK_EMERGENCY,
  SM_NETWORK_LOST,
  SM_AUDIO_ON,
  SM_PHONEBOOK_STATUS_INIT,
  SM_NETWORK_REGISTERED
};

class SM5100B {
  public:
    SM5100B(const char* apn, uint8_t rxPin=2, uint8_t txPin=3, uint16_t baud = 9600L);
    
    void loop();
    void begin();
    bool ping_modem();
    int8_t attach_gprs();
    //bool setup_pdp();
    SoftwareSerial serial;
    bool autoconnect;
    
  private:
    uint16_t _baud;
    uint8_t _phone_state;
    uint8_t _network_state;
    bool attached();
    void read_line();
    char buffer[_SS_MAX_RX_BUFF];
    const char* _apn;
    long _lastAutoconnect;
};

#endif
