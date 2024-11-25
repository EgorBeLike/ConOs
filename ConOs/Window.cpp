#include "Define.h"
#include "Logger.h"

class Window : public Parent {
	string name = "Window-Worker";
	Window(Logger* l) : Parent(l) {}
    void Main() override {
        sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.display();
        }
    }
};
