#include "../Flocking/Flock.h"
#include "../Utils/Vector3.h"
#include "../Utils/Utils.h"
#include "../Utils/LEDPosUtils.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std;

#define AMOUNT_BOIDS 100
#define MAX_SPEED 2 // 60
#define MAX_FORCE .03 // 6
#define PERC_RADIUS 100

#define DEFAULT_POSFILE_LOCATION "../ledspos.txt"
#define DEFAULT_PASSED_MICROSEC_DEVIDER 5000

#define MIN_AXIS_SIZE 500
#define EXTRA_SPACE_AXIS 200

#define WINDOW_SIZE_MOD .7

int main(int argc, char *argv[]) 
{
	// INITIATION
	Utils::init();

	sf::Color boidsColor = sf::Color::Green;

	string filename = DEFAULT_POSFILE_LOCATION;
	float passedMicrosDevider = DEFAULT_PASSED_MICROSEC_DEVIDER;
	int c = 1;
	while (argc > c) 
	{
		cout << "c: " << c << "   argc: " << argc << endl;
		if (string(argv[c]) == "-f") {
			filename = argv[++c];
		}
		else if (string(argv[c]) == "-c") {
			int r = stoi(string(argv[++c]));
			int g = stoi(string(argv[++c]));
			int b = stoi(string(argv[++c]));
			boidsColor = sf::Color(r, g, b);
		}
		else if (string(argv[c]) == "-s") {
			passedMicrosDevider = stoi(string(argv[++c]));
		}
		c++;
	}

	int AmountLeds;
    Vector3* ledsPos = LEDPosUtils::readLedsPosFromFile(filename, &AmountLeds);
	Vector3 boxSize = LEDPosUtils::putLEDPositionsInRelativeSpace(ledsPos, AmountLeds, MIN_AXIS_SIZE, EXTRA_SPACE_AXIS);
	cout << "virtual box: " << boxSize.toStr() << endl;

	Flock flock = Flock(AMOUNT_BOIDS, boxSize, MAX_SPEED, MAX_FORCE, PERC_RADIUS, EXTRA_SPACE_AXIS - 50);

	sf::RenderWindow window(sf::VideoMode(boxSize.x * WINDOW_SIZE_MOD, boxSize.y * WINDOW_SIZE_MOD), "Flocking Debugggg");
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
		// calc flock forces and update velocities and positions
		flock.updateEverything(microsPassed / passedMicrosDevider);


		// draw
		text_bot.setString(flock.boids[0].pos.toStr() + "   sepMult: " + to_string(flock.sepMult));
		window.draw(text_bot);
		for (int i = 0; i < AMOUNT_BOIDS; i++) {
			int r = Utils::map(flock.boids[i].pos.z, 0, boxSize.z, 1, 10);
			shape.setRadius(r);
			shape.setFillColor(boidsColor);	
			shape.setPosition(flock.boids[i].pos.x*WINDOW_SIZE_MOD - r, flock.boids[i].pos.y*WINDOW_SIZE_MOD - r);
			
			window.draw(shape);	
		}
		// draw ledpositions
        for (int i=0; i<AmountLeds; i++) {
			int r = Utils::map(ledsPos[i].z, 0, boxSize.z, 1, 20);
			int x = ledsPos[i].x*WINDOW_SIZE_MOD-r;
			int y = ledsPos[i].y*WINDOW_SIZE_MOD-r;
			shape.setRadius(r);
			shape.setPosition(x, y);
			

            int boidsInRange = 0;
            for (int j=0; j<AMOUNT_BOIDS; j++) {
                if ((flock.boids[j].pos - ledsPos[i]).length() < 200)
                    boidsInRange++;
            }
            // boidsInRange = std::max(boidsInRange, 100);
            float b = boidsInRange / (float)AMOUNT_BOIDS;

			shape.setFillColor(sf::Color(255, 255, 0, b * 255));	
			window.draw(shape);	
			text_leds.setString(to_string(boidsInRange));
			text_leds.setPosition(x+r*2 + 2, y+r - text_leds.getCharacterSize());
			window.draw(text_leds);
        }
		
		int r = Utils::map(flock.destination.z, 0, boxSize.z, 10, 30);
		shape.setRadius(r);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(flock.destination.x*WINDOW_SIZE_MOD - r, flock.destination.y*WINDOW_SIZE_MOD - r);
		window.draw(shape);

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
