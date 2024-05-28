#ifndef KeyPadHelper_h
#define KeyPadHelper_h

#include <Wire.h>
#include <I2CKeyPad.h>

char keypad_layout[19] = "123A456B789C*0#DNF"; // N = NO_KEY, F = FAILED

//Pins with pull-up resistors
//GPIO4
//GPIO5

//GPIO2
//GPIO12
//GPIO13
//GPIO14
//GPIO15
//GPIO25
//GPIO26
//GPIO27

class KeyPadHelper
{
public:
   KeyPadHelper() : keyPad(0x20)
	{
	}

  ~KeyPadHelper()
	{
	}

	void Start()
	{
    if (!keyPad.begin()) {
      Serial.print("Cannot connect to I2C.\n");
      //while(1);
    }

    keyPad.loadKeyMap(keypad_layout);
  }

	int Loop()
	{
    if(keyPad.isPressed())
    {
      int c = keyPad.getChar();
      Serial.print("Tecla ");
      Serial.println((char)c);
      return c;
    }
    else
      return 0;
  }

private:

  I2CKeyPad keyPad;
};
    
#endif
