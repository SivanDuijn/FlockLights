#include "../Utils/ws2812-rpi.h"

#include <iostream>
#include <cstdlib>

using namespace std;

#define DEFAULT_AMOUNT_LEDS 3

int main (int argc, char *argv[])
{
  int n_leds = DEFAULT_AMOUNT_LEDS;
  Color_t onColor = Color_t(255, 80, 0);
  Color_t offColor = Color_t(0, 0, 0);

  int c = 1;
	while (argc > c) 
	{
		if (string(argv[c]) == "-n") {
			n_leds = stoi(string(argv[++c]));
		}
		else if (string(argv[c]) == "-c") {
			int r = stoi(string(argv[++c]));
			int g = stoi(string(argv[++c]));
			int b = stoi(string(argv[++c]));
			onColor = Color_t(r, g, b);
		}
		c++;
	}


  NeoPixel *leds = new NeoPixel(n_leds);
  for (int i=0; i<n_leds; i++)
      leds->setPixelColor(i, onColor);
  leds->show();

  while (true) 
  {
    cout << "Show led number: ";
    int index;
    cin >> index;
    index--;

    if (index >= n_leds)
      cout << "this number is bigger than the maximum index: " << n_leds << endl;
    else if (index >= 0) {
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