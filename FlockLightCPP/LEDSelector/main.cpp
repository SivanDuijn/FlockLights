#include "../Utils/ws2812-rpi.h"

#include <iostream>
#include <cstdlib>

using namespace std;

#define AMOUNT_LEDS 3

int main (int argc, char *argv[])
{
  int n_leds;
  if (argc < 2)
    n_leds = AMOUNT_LEDS;
  else
    n_leds = atoi(argv[1]);

  Color_t onColor = Color_t(255, 90, 0);
  Color_t offColor = Color_t(0, 0, 0);

  NeoPixel *leds = new NeoPixel(n_leds);
  for (int i=0; i<n_leds; i++)
      leds->setPixelColor(i, offColor);
  leds->show();

  while (true) 
  {
    cout << "Show led index number: ";
    int index;
    cin >> index;
    index--;

    if (index >= n_leds)
      cout << "this number is bigger than the maximum index: " << n_leds << endl;
    else {
      for (int i=0; i<n_leds; i++) {
        if (i == index)
          leds->setPixelColor(i, onColor);
        else
          leds->setPixelColor(i, offColor);
      }
      leds->show();
    }
  }
  return 0;
}