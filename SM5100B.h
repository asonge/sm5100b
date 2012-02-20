/**
 * SM5100B-D usage library
 */
#ifndef sm5100b_h

#define sm5100b_h

#include <SoftwareSerial.h>
#include <IPAddress.h>

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
    SoftwareSerial serial;
    bool autoconnect;
    
    SM5100B(const char* apn, uint8_t rxPin=2, uint8_t txPin=3, uint16_t baud = 9600L);
    
    int8_t attach_gprs();
    void begin();
    void loop();
    bool ping_modem();
    
    virtual int connect(IPAddress ip, uint16_t port) =0;
    virtual int connect(const char *host, uint16_t port) =0;
    virtual size_t write(uint8_t) =0;
    virtual size_t write(const uint8_t *buf, size_t size) =0;
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int read(uint8_t *buf, size_t size) = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual void stop() = 0;
    virtual uint8_t connected() = 0;
    virtual operator bool() = 0;
    
  private:
    uint16_t _baud;
    uint8_t _phone_state;
    uint8_t _network_state;
    uint8_t _gprs_state;
    bool attached();
    void read_line();
    char buffer[_SS_MAX_RX_BUFF];
    const char* _apn;
    long _lastAutoconnect;
};

#endif
