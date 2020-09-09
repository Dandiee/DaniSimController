#include <Arduino.h>
#include <TM1637Display.h>

TM1637Display d[] = {
  TM1637Display(7, 2),
  TM1637Display(8, 3),
  //TM1637Display (7, 4),
  //TM1637Display (7, 5),
  //TM1637Display (7, 6),
};



int value = 0;

void setup()
{
    for (int i = 0; i < sizeof(d); i++)
    {
        d[i].setBrightness(0x0f);
    }
}

void loop()
{

    value++;

    for (int i = 0; i < sizeof(d); i++)
    {
        d[i].showNumberDec(value, true);
    }

}
