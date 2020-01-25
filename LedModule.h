#ifndef LedModule_h
#define LedModule_h

#include "Arduino.h"

#define _DEBUG 1

class LedModule
{
protected:
  String name;
  uint8_t pin;
  uint8_t state = LOW;
  
public:
  virtual ~LedModule();
  LedModule();
  LedModule(const char *name);
  LedModule(uint8_t pin);
  LedModule(const char *name, uint8_t pin);
  void begin();
  void begin(uint8_t pin);
  void debug(String msg);
  uint8_t getState();
  uint8_t setState(uint8_t state);
  uint8_t getPinout();
  String getName();
  void setName(const char *name);
  virtual void on();
  virtual void off();
  virtual void toggle();
  bool equal(uint8_t pin);
};
#endif