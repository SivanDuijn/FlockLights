#include "Boid.h"
#include "Vector3.h"

#include "Utils.h"
#include "BoidUtils.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <unistd.h>


using namespace std;

#define AMOUNT 100
#define MAX_SPEED 1 // 60
#define MAX_FORCE .02 // 6
#define PERC_RADIUS 100

int main() 
{
	Utils::init();
	sf::RenderWindow window(sf::VideoMode(900, 900), "Flocking Debugggg");

	// INITIATION
	Vector3 boxSize = Vector3(900, 900, 900);
	Boid boids[AMOUNT];
	sf::Color colors[AMOUNT];

	for (int i = 0; i < AMOUNT; i++) {
		boids[i] = BoidUtils::createRandomBoid(boxSize, 5);
		colors[i] = sf::Color(Utils::randInt(255), Utils::randInt(255), Utils::randInt(255));
	}


	// for (int i = 0; i < AMOUNT; i++) {
	// 	boids[i].calcFlockForce(boids, AMOUNT, 50, 20, 4, sizeBox, 2);
	// }
	// for (int i = 0; i < AMOUNT; i++) {
	// 	boids[i].update(20, 4, 1, sizeBox);
	// 	//boids[i].pos.log();
	// }
	//boids[0].calcFlockForce(boids, AMOUNT, 50, 20, 4, sizeBox, 2);


	int counter = 0;
	sf::CircleShape shape;
	sf::Font font;
	font.loadFromFile("courier-prime.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.move(0, boxSize.y - text.getCharacterSize() - 4);
	// keep track of passed millis
	auto start = chrono::steady_clock::now();
	auto end = chrono::steady_clock::now();
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

		// do flock calculations and updates
		for (int i = 0; i < AMOUNT; i++) {
			boids[i].calcFlockForce(boids, AMOUNT, PERC_RADIUS, MAX_SPEED, MAX_FORCE, boxSize, 2.5);
		}

		// timing calculations
		end = chrono::steady_clock::now();
		int microsPassed = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		start = end;
		for (int i = 0; i < AMOUNT; i++) {
			boids[i].update(MAX_SPEED, MAX_FORCE, microsPassed / 10000.0, boxSize); // was eerst nulletje meer
		}
		counter++;
		if (counter % 100 == 0)
			cout << microsPassed / 1000000.0 << endl;

		// draw
		text.setString(boids[0].pos.toStr() + "   " + to_string(microsPassed));
		window.draw(text);
		for (int i = 0; i < AMOUNT; i++) {
			int r = Utils::map(boids[i].pos.z, 0, boxSize.z, 1, 10);
			shape.setRadius(r);
			shape.setFillColor(colors[i]);	
			shape.setPosition(boids[i].pos.x - r, boids[i].pos.y-r);
			
			window.draw(shape);
			
		}
		window.display();
    }

	return 0;
}
