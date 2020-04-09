#include "../Utils/Vector3.h"
#include "../Utils/Utils.h"
#include "../Utils/LEDPosUtils.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std;

#define SPEED 200
#define GROUP_RADIUS 150

#define DEFAULT_POSFILE_LOCATION "../ledspos.txt"
#define DEFAULT_PASSED_MICROSEC_DEVIDER 5000

#define WINDOW_SIZE_MOD .7

struct Connection 
{
    int ledIndex;
    float distance;
};

int main(int argc, char *argv[]) 
{
	// INITIATION
	Utils::init();
	Vector3 boxSize = Vector3(700,700,700);

	sf::Color dotColor = sf::Color::Green;

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

	cout << connections[0][0].distance << endl;

	int lonerDotDestLEDi = Utils::randInt(nLEDs-1);
	Vector3 lonerDot = ledsPos[lonerDotDestLEDi];
	int lonerDotPrevDestLEDi = lonerDotDestLEDi;
	Vector3 lonerDotVel = Vector3(0,0,0);
	int groupDotDestLEDi = Utils::randInt(nLEDs-1);
	Vector3 groupDot = ledsPos[groupDotDestLEDi];
	Vector3 groupDotVel = Vector3(0,0,0);
	

	sf::RenderWindow window(sf::VideoMode(boxSize.x * WINDOW_SIZE_MOD, boxSize.y * WINDOW_SIZE_MOD), "LonelyDot Debug");
	int counter = 0;
	sf::CircleShape shape;
	sf::Font font;
	font.loadFromFile("courier-prime.ttf");
	sf::Text text_bot;
	text_bot.setFont(font);
	text_bot.setCharacterSize(20);
	text_bot.move(0, boxSize.y*WINDOW_SIZE_MOD - text_bot.getCharacterSize() - 4);

	sf::Text text_leds;
	text_leds.setFont(font);
	text_leds.setCharacterSize(15);

	// initiate timing calculations
	auto current = chrono::steady_clock::now();
	auto timestamp = chrono::steady_clock::now();
  auto fpsTimestamp = chrono::steady_clock::now();
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

		// timing calculations
		current = chrono::steady_clock::now();
		int microsPassed = chrono::duration_cast<std::chrono::microseconds>(current - timestamp).count();
		timestamp = current;
		
		// update loop
		// choose a random led from the closest eight to the groupDot 
		// when it's at its destination		
		if ((groupDot - ledsPos[groupDotDestLEDi]).length() < 5) {
			int r = Utils::randInt(7);
			groupDotDestLEDi = connections[groupDotDestLEDi][r].ledIndex;
			groupDotVel = ledsPos[groupDotDestLEDi] - groupDot;
			groupDotVel.setMag(SPEED);
		}
		// do the same for lonerDot but move away from groupDot (maximize the dinstance between em)
		if ((lonerDot - ledsPos[lonerDotDestLEDi]).length() < 5) {
			float max = 0;
			int maxIndex = 0;
			for (int i = 0; i < 4; i++) {
				int connIndex = connections[lonerDotDestLEDi][i].ledIndex;
				float d = (ledsPos[connIndex] - groupDot).length();
				if (d > max && connIndex != lonerDotPrevDestLEDi) {
					max = d;
					maxIndex = connIndex;
				}
			}
			lonerDotPrevDestLEDi = lonerDotDestLEDi;
			lonerDotDestLEDi = maxIndex;
			
			lonerDotVel = ledsPos[lonerDotDestLEDi] - lonerDot;
			lonerDotVel.setMag(SPEED);
		}

		// update positions
		float secondsPassed = microsPassed / 1000000.0;
		groupDot += groupDotVel * secondsPassed;
		lonerDot += lonerDotVel * secondsPassed;

		// draw dots
		int r = Utils::map(groupDot.z, 0, boxSize.z, 1, 10);
		shape.setRadius(r);
		shape.setFillColor(sf::Color::Green);	
		shape.setPosition(groupDot.x*WINDOW_SIZE_MOD - r, groupDot.y*WINDOW_SIZE_MOD - r);
		window.draw(shape);	
		r = Utils::map(lonerDot.z, 0, boxSize.z, 1, 10);
		shape.setRadius(r);
		shape.setFillColor(sf::Color::Red);	
		shape.setPosition(lonerDot.x*WINDOW_SIZE_MOD - r, lonerDot.y*WINDOW_SIZE_MOD - r);
		window.draw(shape);	
		
		// draw leds
    for (int i=0; i<nLEDs; i++) {
			float distanceToGroupDot = (ledsPos[i] - groupDot).length();
			float b = 0;
			if (distanceToGroupDot < GROUP_RADIUS) {
				int r = Utils::map(ledsPos[i].z, 0, boxSize.z, 1, 20);
				int x = ledsPos[i].x*WINDOW_SIZE_MOD-r;
				int y = ledsPos[i].y*WINDOW_SIZE_MOD-r;
				shape.setRadius(r);
				shape.setPosition(x, y);
				b = distanceToGroupDot / GROUP_RADIUS;
				shape.setFillColor(sf::Color(255, 255, 0, (1 - b) * 255));	
				window.draw(shape);	
				text_leds.setString(to_string((int)((1 - b)*100)));
				text_leds.setPosition(x+r*2 + 2, y+r - text_leds.getCharacterSize());
			}

			window.draw(text_leds);
    }

		Vector3 lPos = ledsPos[lonerDotDestLEDi];
		r = Utils::map(lPos.z, 0, boxSize.z, 1, 20);
		int x = lPos.x*WINDOW_SIZE_MOD-r;
		int y = lPos.y*WINDOW_SIZE_MOD-r;
		shape.setRadius(r);
		shape.setPosition(x, y);
		float distToLonerDot = (lonerDot - lPos).length();
		float b = distToLonerDot / GROUP_RADIUS;
		shape.setFillColor(sf::Color(255, 255, 0, (1 - b) * 255));	
		window.draw(shape);	
		text_leds.setString(to_string((int)((1 - b)*100)));
		text_leds.setPosition(x+r*2 + 2, y+r - text_leds.getCharacterSize());

		lPos = ledsPos[lonerDotPrevDestLEDi];
		r = Utils::map(lPos.z, 0, boxSize.z, 1, 20);
		x = lPos.x*WINDOW_SIZE_MOD-r;
		y = lPos.y*WINDOW_SIZE_MOD-r;
		shape.setRadius(r);
		shape.setPosition(x, y);
		distToLonerDot = (lonerDot - lPos).length();
		b = distToLonerDot / GROUP_RADIUS;
		shape.setFillColor(sf::Color(255, 255, 0, (1 - b) * 255));	
		window.draw(shape);	
		text_leds.setString(to_string((int)((1 - b)*100)));
		text_leds.setPosition(x+r*2 + 2, y+r - text_leds.getCharacterSize());

		window.display();
		
		// calc and print fps
        counter++;
        auto temp = chrono::steady_clock::now();
        int passed  = chrono::duration_cast<std::chrono::microseconds>(temp - fpsTimestamp).count();
        if (passed > 1000000.0) { 
            cout << counter  << " fps" <<  "     micros: " << microsPassed << endl;
            counter = 0;
            fpsTimestamp = chrono::steady_clock::now();
        }
		//usleep(33333);
    }

	return 0;
}
