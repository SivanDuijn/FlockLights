#include "../Flocking/Flock.h"
#include "../Flocking/BoidUtils.h"
#include "../Utils/Vector3.h"
#include "../Utils/Utils.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <unistd.h>


using namespace std;

#define AMOUNT 100
#define MAX_SPEED 3 // 60
#define MAX_FORCE .03 // 6
#define PERC_RADIUS 100

int main() 
{
	Utils::init();
	sf::RenderWindow window(sf::VideoMode(900, 900), "Flocking Debugggg");

	// INITIATION
	Vector3 boxSize = Vector3(900, 900, 900);
	Flock flock = Flock(AMOUNT, boxSize, MAX_SPEED, MAX_FORCE, PERC_RADIUS);

	sf::Color colors[AMOUNT];
	for (int i = 0; i < AMOUNT; i++) {
		colors[i] = sf::Color(Utils::randInt(255), Utils::randInt(255), Utils::randInt(255));
	}


	int counter = 0;
	sf::CircleShape shape;
	sf::Font font;
	font.loadFromFile("courier-prime.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.move(0, boxSize.y - text.getCharacterSize() - 4);
	// keep track of passed millis
	auto timestamp = chrono::steady_clock::now();
	auto current = chrono::steady_clock::now();
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
		flock.updateEverything(microsPassed / 10000.0);

		//cout << flock.sepMult << endl;

		counter++;
		if (counter % 100 == 0)
			cout << microsPassed / 1000.0 << " ms" << endl;

		// draw
		text.setString(flock.boids[0].pos.toStr() + "   " + to_string(microsPassed));
		window.draw(text);
		for (int i = 0; i < AMOUNT; i++) {
			int r = Utils::map(flock.boids[i].pos.z, 0, boxSize.z, 1, 10);
			shape.setRadius(r);
			shape.setFillColor(colors[i]);	
			shape.setPosition(flock.boids[i].pos.x - r, flock.boids[i].pos.y-r);
			
			window.draw(shape);
			
		}
		int r = Utils::map(flock.destination.z, 0, boxSize.z, 10, 30);
		shape.setRadius(r);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(flock.destination.x - r, flock.destination.y - r);
		window.draw(shape);

		window.display();
    }

	return 0;
}
