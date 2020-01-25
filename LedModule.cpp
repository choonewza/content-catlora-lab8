#include "LedModule.h"

LedModule::~LedModule()
{
    debug("is destroyed.");
}
LedModule::LedModule() {}
LedModule::LedModule(const char *name)
{
    this->name = String(name);
}
LedModule::LedModule(uint8_t pin)
{
    this->pin = pin;
}
LedModule::LedModule(const char *name, uint8_t pin)
{
    this->name = String(name);
    this->pin = pin;
}
void LedModule::begin()
{
    pinMode(this->pin, OUTPUT);
    off(); //Turn-off LedModule
    delay(100);
}
void LedModule::begin(uint8_t pin)
{
    this->pin = pin;
    begin();
}
void LedModule::debug(String msg)
{
    Serial.print(F("_DEBUG: LedModule["));
    Serial.print(getName());
    Serial.print(F("] "));
    Serial.println(msg);
}
uint8_t LedModule::getState()
{
    return this->state;
}
uint8_t LedModule::setState(uint8_t state)
{
    state == HIGH ? on() : off();
}
uint8_t LedModule::getPinout()
{
    return this->pin;
}
String LedModule::getName()
{
    return this->name;
}
void LedModule::setName(const char *name)
{
    this->name = String(name);
}
void LedModule::on()
{
    if (this->state == LOW)
    {
        this->state = HIGH;
        digitalWrite(this->pin, this->state);

        #ifdef _DEBUG
            debug("Trun-on");
        #endif
    }
}
void LedModule::off()
{
    if (this->state == HIGH)
    {
        this->state = LOW;
        digitalWrite(this->pin, this->state);

        #ifdef _DEBUG
            debug("Trun-off");
        #endif
    }
}
void LedModule::toggle()
{
    this->state == HIGH ? off() : on();
}
bool LedModule::equal(uint8_t pin)
{
    return this->pin == pin;
}