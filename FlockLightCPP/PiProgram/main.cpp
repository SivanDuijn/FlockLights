#include "../Flocking/Flock.h"
#include "../Flocking/BoidUtils.h"
#include "../Utils/Vector3.h"
#include "../Utils/Utils.h"
#include "../Utils/ws2812-rpi.h"

#include <iostream>
#include <chrono>
#include <unistd.h>
#include <climits>

using namespace std;

#define AMOUNT_BOIDS 100
#define MAX_SPEED 2 // 60
#define MAX_FORCE .03 // 6
#define PERC_RADIUS 100

#define AMOUNT_LEDS 5

Color_t setBrightness(Color_t c, float k) {
    uint8_t r = c.r*k;
    uint8_t g = c.g*k;
    uint8_t b = c.b*k;
    // prevent one channel showing, always blend, (not only red)
    if ((uint8_t)c.r != 0 && r == 0 && (g != 0 || b != 0))
        r = 1;
    if ((uint8_t)c.g != 0 && g == 0 && (r != 0 || b != 0))
        g = 1;
    if ((uint8_t)c.b != 0 && b == 0 && (r != 0 || g != 0))
        b = 1;
    return Color_t(r, g, b);
}

int main() 
{
	Utils::init();

	// INITIATION
    // boids
	Vector3 boxSize = Vector3(900, 900, 900);
	Flock flock = Flock(AMOUNT_BOIDS, boxSize, MAX_SPEED, MAX_FORCE, PERC_RADIUS);

    // LEDS POSITIONS
    Vector3 ledsPos[AMOUNT_LEDS] = {
        Vector3(491,273,345),
        Vector3(321,265,335),
        Vector3(411,190,537),
        Vector3(386,355,619),
        Vector3(244,135,500)
    };
    Utils::putLEDPositionsInRelativeSpace(ledsPos, boxSize, AMOUNT_LEDS);
    // for (int i=0; i<AMOUNT_LEDS; i++) {
    //     ledsPos[i] = Utils::randVecInBox(boxSize);
    // }
    
    // LEDS
    Color_t color = Color_t(255, 90, 0);
    NeoPixel *leds = new NeoPixel(AMOUNT_LEDS);
    for (int i=0; i<AMOUNT_LEDS; i++)
        leds->setPixelColor(i, color);
    leds->show();
    usleep(100000);


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
		flock.updateEverything(microsPassed / 10000.0);

        // calculate leds brightness
        for (int i=0; i<AMOUNT_LEDS; i++) {
            int boidsInRange = 0;
            for (int j=0; j<AMOUNT_BOIDS; j++) {
                if ((flock.boids[j].pos - ledsPos[i]).length() < 300)
                    boidsInRange++;
            }
            // boidsInRange = std::max(boidsInRange, 10);
            float b = boidsInRange / (float)AMOUNT_BOIDS;
            b = std::max(b, 0.1f);
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
