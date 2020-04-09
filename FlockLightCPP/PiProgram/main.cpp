#include "../Flocking/Flock.h"
#include "../Flocking/BoidUtils.h"
#include "../Utils/Vector3.h"
#include "../Utils/Utils.h"
#include "../Utils/ws2812-rpi.h"
#include "../Utils/LEDPosUtils.h"

#include <iostream>
#include <chrono>
#include <unistd.h>
#include <climits>

using namespace std;

#define AMOUNT_BOIDS 100
#define MAX_SPEED 2 // 60
#define MAX_FORCE .03 // 6
#define PERC_RADIUS 100

#define DEFAULT_POSFILE_LOCATION "../ledspos.txt"
#define DEFAULT_PASSED_MICROSEC_DEVIDER 5000
#define DEFAULT_COLOR Color_t(255, 90, 0)

#define MIN_AXIS_SIZE 400
#define EXTRA_SPACE_AXIS 200

Color_t setBrightness(Color_t c, float k) {
    k = std::max(k, 0.1f);
    
    uint8_t r = c.r*k;
    uint8_t g = c.g*k;
    uint8_t b = c.b*k;

    // // prevent one channel showing, always blend, (not only red)
    // if ((uint8_t)c.r != 0 && r == 0 && (g != 0 || b != 0))
    //     r = 1;
    // if ((uint8_t)c.g != 0 && g == 0 && (r != 0 || b != 0))
    //     g = 1;
    // if ((uint8_t)c.b != 0 && b == 0 && (r != 0 || g != 0))
    //     b = 1;
    return Color_t(r, g, b);
}

int main(int argc, char *argv[]) 
{
	Utils::init();

	// INITIATION

    // LEDs positions
    string filename = DEFAULT_POSFILE_LOCATION;
    float passedMicrosDevider = DEFAULT_PASSED_MICROSEC_DEVIDER;
	int c = 1;
	while (argc > c) 
	{
		cout << "c: " << c << "   argc: " << argc << endl;
		if (string(argv[c]) == "-f") {
			filename = argv[++c];
		}
        else if (string(argv[c]) == "-s") {
            passedMicrosDevider = stoi(string(argv[++c]));
        }
		c++;
	}

    // read LEDs positions from file and scale them so they fit in virtual space
	int amountLeds;
    Vector3* ledsPos = LEDPosUtils::readLedsPosFromFile(filename, &amountLeds);
	Vector3 boxSize = LEDPosUtils::putLEDPositionsInRelativeSpace(ledsPos, amountLeds, MIN_AXIS_SIZE, EXTRA_SPACE_AXIS);
    
    // Initiate LEDs
    Color_t color = DEFAULT_COLOR;
    NeoPixel *leds = new NeoPixel(amountLeds);
    for (int i=0; i<amountLeds; i++)
        leds->setPixelColor(i, color);
    leds->show();
    usleep(1000000);

    // boids
	Flock flock = Flock(AMOUNT_BOIDS, boxSize, MAX_SPEED, MAX_FORCE, PERC_RADIUS, EXTRA_SPACE_AXIS - 50);

    // initiate timing calculations
	auto start = chrono::steady_clock::now();
	auto end = chrono::steady_clock::now();
    auto fpsTimer = chrono::steady_clock::now();

    int counter = 0;
    while (true) {
        end = chrono::steady_clock::now();
        int microsPassed = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        start = end;
        // calc flock forces and update velocities and positions
		flock.updateEverything(microsPassed / passedMicrosDevider);

        // calculate leds brightness
        for (int i=0; i<amountLeds; i++) {
            int boidsInRange = 0;
            for (int j=0; j<AMOUNT_BOIDS; j++) {
                if ((flock.boids[j].pos - ledsPos[i]).length() < 200)
                    boidsInRange++;
            }
            // boidsInRange = std::max(boidsInRange, 10);
            float b = boidsInRange / (float)AMOUNT_BOIDS;
            Color_t c = setBrightness(color, b);
            leds->setPixelColor(i, c);
            //cout << (int)c.r << " " << (int)c.g << " " << (int)c.b << endl;
        }
        leds->show();

        // calc and print fps
        counter++;
        auto temp = chrono::steady_clock::now();
        int passed  = chrono::duration_cast<std::chrono::microseconds>(temp - fpsTimer).count();
        if (passed > 1000000.0) { 
            cout << counter  << " fps" <<  "     micros: " << microsPassed << endl;
            counter = 0;
            fpsTimer = chrono::steady_clock::now();
        }
    }

	return 0;
}
