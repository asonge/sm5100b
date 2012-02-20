#include "SM5100B.h"
#include <Arduino.h>

SM5100B::SM5100B(const char *apn,
      uint8_t rxPin, uint8_t txPin, uint16_t baud) : serial(rxPin, txPin) {
  _baud = baud;
  autoconnect = true;
  _apn = apn;
}

void SM5100B::begin() {
  Serial.print(F("Starting @ "));
  Serial.println(_baud);
  serial.begin(_baud);
  serial.setTimeout(10000);
}

void SM5100B::loop() {
  while(serial.available()) {
    read_line();
    if(strncmp(buffer,(const char*) F("+SIND: "),7)==0) {
      int status = -1;
      if(isDigit(buffer[7]) && isDigit(buffer[8])) {
        char a[3] = {buffer[7],buffer[8],0};
        status = atoi(a);
      } else if(isDigit(buffer[7])) {
        status = buffer[7] - '0';
      }
      Serial.print(F("Status "));
      Serial.println(status);
      switch(status) {
        case SM_NETWORK_REGISTERED:
        case SM_NETWORK_LOST:
          _network_state = status;
          break;
        case SM_SMS_READY:
          _phone_state = status;
          _lastAutoconnect = millis() + 1000;
          break;
      }
    } else if(buffer[0] == (char)160 && buffer[0] == buffer[1] &&
              buffer[0] == buffer[2] && buffer[0] == buffer[3]) {
      // PASS
    } else {
      Serial.print(F("Unknown line:"));
      Serial.println(buffer);
    }
  }
  if(_gprs_state!=1 && autoconnect && _phone_state==SM_SMS_READY && _lastAutoconnect + 10000 < millis()) {
    _lastAutoconnect = millis();
    if(attached()) {
      if(attach_gprs()==0) {
        _gprs_state=1;
      }
    }
  }
}

/*
 * TODO: error codes
 */
int8_t SM5100B::attach_gprs() {
  // Set up PDP context
  Serial.println(F("Setup PDP Context"));
  loop();
  serial.print(F("AT+CGDCONT=1,\"IP\",\""));
  serial.print(_apn);
  serial.println('"');
  read_line();
  if(strcmp(buffer, "OK")!=0) {
    Serial.print(F("Didn't set up context: "));
    Serial.println(buffer);
    return -2;
  }
  // Activate PDP context
  Serial.println(F("Activate PDP Context"));
  loop();
  serial.println(F("AT+CGACT=1,1"));
  if(strcmp(buffer, "OK")==0) {
    return 0;
  } else {
    Serial.print(F("Couldn't activate context: "));
    Serial.println(buffer);
    return -3;
  }
}

bool SM5100B::attached() {
  // Are we attached to GPRS?
  loop();
  serial.println(F("AT+CGATT?"));
  read_line();
  Serial.println(F("Attached?"));
  Serial.println(buffer);
  if(strcmp(buffer, "+CGATT: 1")==0) {
    Serial.print(F("We're attached!"));
    read_line(); // Get rid of OK or others...
    return true;
  } else {
    Serial.print(F("Is not attached: "));
    Serial.println(buffer);
    return false;
  }
}

bool SM5100B::ping_modem() {
  loop(); // get rid of any trash like leftover blank lines
  serial.println(F("AT"));
  read_line();
  if(strcmp(buffer,"OK")==0) {
    return true;
  } else if(strcmp(buffer,"ERROR")==0) {
    return false;
  } else {
    Serial.println(F("Unknown line"));
    Serial.println(buffer);
    return false;
  }
  return false;
}

void SM5100B::read_line() {
  int bytes_read = 0;
  while(bytes_read==0) {
    bytes_read = serial.readBytesUntil('\n', buffer, _SS_MAX_RX_BUFF);
    while(bytes_read && isSpace(buffer[bytes_read-1])) bytes_read--;
    if(bytes_read < _SS_MAX_RX_BUFF)
      buffer[bytes_read] = 0;
  }
}

virtual int connect(IPAddress ip, uint16_t port) {
  //AT+SDATACONF=1,"TCP","211.123.11.21",1234
  serial.print(F("1,\"TCP\",\""));
  serial.print(ip);
  serial.print('"');
  serial.println(port);
  read_line();
}
virtual int connect(const char *host, uint16_t port) {
  //AT+SDATACONF=1,"TCP","211.123.11.21",1234
  serial.print(F("1,\"TCP\",\""));
  serial.print(host);
  serial.print('"');
  serial.println(port);
}
virtual size_t write(uint8_t c) {
  serial.println("1,1");
  serial.write(c);
  serial.write(26);
  return 1;
}
virtual size_t write(const uint8_t *buf, size_t size) {
  serial.print("1,");
  serial.println(size);
  size_t = retsize = serial.write(buf, size);
  serial.write(26);
  return size_t;
}
virtual int available() {
  // TODO
  return 0;
}
virtual int read() {
  // TODO
  return 0;
}
virtual int read(uint8_t *buf, size_t size) {
  // TODO
  return 0;
}
virtual int peek() {
  // TOOD
  return 0;
}
virtual void flush() {
  // TODO
}
virtual void stop() {
  // TODO
}
virtual uint8_t connected() {
  // TODO
}
virtual operator bool() {
  // TODO
}
