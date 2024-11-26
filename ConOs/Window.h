#pragma once
#include "Define.h"
#include "Logger.h"

struct KeyBoardMouse {
    sf::Event::KeyEvent key;
    sf::Event::TouchEvent touch;
    sf::Event::JoystickButtonEvent joystick;
    sf::Event::JoystickConnectEvent joystickConn;
    sf::Event::JoystickMoveEvent joystickMove;
    sf::Event::MouseButtonEvent mouseButton;
    sf::Event::MouseMoveEvent mouseMove;
    sf::Event::MouseWheelEvent mouseWheel;
    sf::Event::MouseWheelScrollEvent mouseScroll;
    sf::Event::SensorEvent sensor;
    sf::Event::SizeEvent size;
    sf::Event::TextEvent text;
    sf::Event::EventType type;
    sf::Vector2i pos;
};

class Window : public Parent {
public:
    KeyBoardMouse kbm{};
    unsigned int x = 800, y = 600;
    Window(Logger* l, string name = "Window-Worker") : Parent(l, name) {}
    void Main() override {
        sf::RenderWindow window(sf::VideoMode(x, y), NAME, sf::Style::Close | sf::Style::Titlebar);
        if (window.isOpen()) {
            logger->SendSignal(this, INFO, "Window is created!");
        }
        else {
            logger->SendSignal(this, FATAL, "Window isn't created!");
        }
        
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                kbm.key = event.key;
                kbm.touch = event.touch;
                kbm.joystick = event.joystickButton;
                kbm.joystickConn = event.joystickConnect;
                kbm.joystickMove = event.joystickMove;
                kbm.mouseButton = event.mouseButton;
                kbm.mouseMove = event.mouseMove;
                kbm.mouseScroll = event.mouseWheelScroll;
                kbm.mouseWheel = event.mouseWheel;
                kbm.sensor = event.sensor;
                kbm.size = event.size;
                kbm.text = event.text;
                kbm.type = event.type;
                if (kbm.type == sf::Event::Closed)
                {
                    window.close();
                    logger->SendSignal(this, FATAL, "Window is closed!");
                }
                
            }
            window.clear();
            window.display();
        }

        Exit();
    }
    void Draw() {
    }
};

