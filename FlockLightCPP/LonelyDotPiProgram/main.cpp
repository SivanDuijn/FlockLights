#include "../Utils/Vector3.h"
#include "../Utils/Utils.h"
#include "../Utils/ws2812-rpi.h"
#include "../Utils/LEDPosUtils.h"

#include <iostream>
#include <chrono>
#include <unistd.h>
#include <climits>

using namespace std;

#define SPEED 200
#define GROUP_RADIUS 220

#define DEFAULT_POSFILE_LOCATION "../ledspos.txt"
#define DEFAULT_COLOR Color_t(255, 90, 0)

Color_t setBrightness(Color_t c, float k) {
    k = std::max(k, 0.1f);
    
    uint8_t r = c.r*k;
    uint8_t g = c.g*k;
    uint8_t b = c.b*k;
    return Color_t(r, g, b);
}

struct Connection 
{
    int ledIndex;
    float distance;
};

struct Dot 
{
	Vector3 pos,vel;
	int destLEDi,prevDestLEDi;
};

void moveDotAwayFrom(Dot &dot, Dot &otherDot, Connection *(*connections), Vector3 *ledsPos, int chanceToChooseRandom) {
	if ((dot.pos - ledsPos[dot.destLEDi]).length() < 5) {
		int newDestIndex = 0;
		
		if (Utils::randInt(100) <= chanceToChooseRandom) {
			int r = Utils::randInt(5);
			newDestIndex = connections[dot.destLEDi][r].ledIndex;
		}
		else {
			float max = 0;
			newDestIndex = 0;
			for (int i = 0; i < 4; i++) {
				int connIndex = connections[dot.destLEDi][i].ledIndex;
				float d = (ledsPos[connIndex] - otherDot.pos).length();
				if (d > max && connIndex != dot.prevDestLEDi) {
					max = d;
					newDestIndex = connIndex;
				}
			}
		}
		dot.prevDestLEDi = dot.destLEDi;
		dot.destLEDi = newDestIndex;
		
		dot.vel = ledsPos[dot.destLEDi] - dot.pos;
		dot.vel.setMag(SPEED);
	}
}

int main(int argc, char *argv[]) 
{
	Utils::init();

	// INITIATION

	// LEDs positions
	string filename = DEFAULT_POSFILE_LOCATION;
	float passedMicrosDevider = 5000;
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
	int nLEDs;
  Vector3* ledsPos = LEDPosUtils::readLedsPosFromFile(filename, &nLEDs);
	Connection *connections[nLEDs];
	for (int i = 0; i < nLEDs; i++) {
		Connection *conns = new Connection[nLEDs - 1];
		for (int j = 0; j < i; j++) {
            conns[j] = {j, (ledsPos[i] - ledsPos[j]).length()};
		}
		for (int j = i+1; j < nLEDs; j++) {
			conns[j-1] = {j, (ledsPos[i] - ledsPos[j]).length()};
		}

		qsort(conns, nLEDs - 1, sizeof(conns[0]), [](const void *a, const void *b){
			// Need to cast the void * to Connection *
			return (int)( (*(Connection*)a).distance - (*(Connection*)b).distance );
		});

		connections[i] = conns;
	}

	// initiate dots
	Dot lonerDot = Dot();
  lonerDot.destLEDi = Utils::randInt(nLEDs-1);
	lonerDot.pos = ledsPos[lonerDot.destLEDi];
	lonerDot.prevDestLEDi = lonerDot.destLEDi;
	lonerDot.vel = Vector3(0,0,0);
	Dot groupDot = Dot();
  groupDot.destLEDi = Utils::randInt(nLEDs-1);
	groupDot.pos = ledsPos[groupDot.destLEDi];
	groupDot.prevDestLEDi = groupDot.destLEDi;
	groupDot.vel = Vector3(0,0,0);

	// Initiate LEDs
	Color_t color = DEFAULT_COLOR;
	NeoPixel *leds = new NeoPixel(nLEDs);
	for (int i=0; i<nLEDs; i++)
			leds->setPixelColor(i, color);
	leds->show();
	usleep(1000000);

	// initiate timing calculations
	auto start = chrono::steady_clock::now();
	auto end = chrono::steady_clock::now();
  auto fpsTimer = chrono::steady_clock::now();

  int counter = 0;
  while (true) {
		end = chrono::steady_clock::now();
		int microsPassed = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		start = end;

		// update loop

		moveDotAwayFrom(lonerDot, groupDot, connections, ledsPos, 0);
		moveDotAwayFrom(groupDot, lonerDot, connections, ledsPos, 50);

		// update positions
		float secondsPassed = microsPassed / 1000000.0;
		groupDot.pos += groupDot.vel * secondsPassed;
		lonerDot.pos += lonerDot.vel * secondsPassed;

    // draw leds
    for (int i=0; i<nLEDs; i++) {
			float distanceToGroupDot = (ledsPos[i] - groupDot.pos).length();
			float b = 0;
			if (distanceToGroupDot < GROUP_RADIUS) 
				b = 1 - (distanceToGroupDot / GROUP_RADIUS);

			Color_t c = setBrightness(color, b);
			leds->setPixelColor(i, c);
    }
        
		// loner dot set leds
		Color_t lonerCol = Color_t(255,40,0);
		float prevDestToDestDist = (ledsPos[lonerDot.destLEDi] - ledsPos[lonerDot.prevDestLEDi]).length();

		Vector3 lPos = ledsPos[lonerDot.destLEDi];
		float lonerDotToDestDist = (lonerDot.pos - lPos).length();
		float b = lonerDotToDestDist / prevDestToDestDist;
    Color_t col = setBrightness(lonerCol, (1-b));
  	leds->setPixelColor(lonerDot.destLEDi, col);
		col = setBrightness(lonerCol, b);
		leds->setPixelColor(lonerDot.prevDestLEDi, col);

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
		usleep(10);
    }

	return 0;
}
