#include <SFML/Graphics.hpp>
#include <chrono>
#include <math.h>
#include <iostream>

using namespace std;

#define WIDTH   900
#define HEIGHT  900
#define SPEED   5         // higher the slower

// 0 <= n <= 1
float easeIn(float n, float p = 3) {
    return powf32(n, p);
}
float easeOut(float n, float p = 3) {
    return 1 - (easeIn(1 - n, p));
}
float easeInOut(float n) {
    if (n <= .5) 
        return easeIn(n * 1.587402, 3);     // (n / .5) * 0.793701
    return easeOut((n - .5) * 1.587402 + .206299, 3);
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "EasInOut");
    sf::CircleShape dot;
    dot.setFillColor(sf::Color::Yellow);
    dot.setRadius(3);
    sf::RectangleShape line;
    line.setFillColor(sf::Color::Green);
    line.setSize(sf::Vector2f(WIDTH, 2));
    sf::CircleShape dot2;
    dot2.setFillColor(sf::Color::Red);
    dot2.setRadius(10);

    // keep track of passed millis
	auto start = chrono::steady_clock::now();
	auto end = chrono::steady_clock::now();
    int microsPassed = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

		// calculate passed microseconds
		end = chrono::steady_clock::now();
		microsPassed = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		if (microsPassed > 2000000 * SPEED)
            start = end;

		
        for (float i = 0; i < 1; i += .001) 
        {
            int x = WIDTH * i;
            int y = HEIGHT - HEIGHT * easeInOut(i);
            dot.setPosition(x + 1, y + 1);
		    window.draw(dot);
        }

        float a = microsPassed / (1000000.0  * SPEED);
        if (microsPassed > 1000000  * SPEED)
            a = ((2000000 * SPEED) - microsPassed) / (1000000.0 * SPEED);
        int y = HEIGHT - HEIGHT * easeInOut(a);
        line.setPosition(0, y);
        window.draw(line);

        int x = WIDTH * a;
        dot2.setPosition(x - 5, y - 5);
        window.draw(dot2);
        
		window.display();
    }
}