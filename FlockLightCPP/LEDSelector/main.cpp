#include "../Utils/ws2812-rpi.h"

#include <iostream>

using namespace std;

#define AMOUNT_LEDS 3

int main ()
{
  Color_t onColor = Color_t(255, 90, 0);
  Color_t offColor = Color_t(0, 0, 0);

  NeoPixel *leds = new NeoPixel(AMOUNT_LEDS);
  for (int i=0; i<AMOUNT_LEDS; i++)
      leds->setPixelColor(i, offColor);
  //leds->show();

  while (true) 
  {
    cout << "Show led index number: ";
    int index;
    cin >> index;

    if (index >= AMOUNT_LEDS) 
      cout << "this number is bigger than the maximum index: " << AMOUNT_LEDS-1 << endl;
    else {
      for (int i=0; i<AMOUNT_LEDS; i++) {
        if (i == index)
          leds->setPixelColor(i, onColor);
        else
          leds->setPixelColor(i, offColor);
      }
      //leds->show();
    }
  }
  return 0;
}