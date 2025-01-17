#pragma once
#include "Define.h"
#include "Logger.h"
#include "Drawable.h"

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

    DrawableSorter pool;
    using poolIter = DrawableSorter::iterator;

    sf::RenderWindow* window;

    Window(Logger* l, string name = "Window-Worker") : Parent(l, name), pool(l, DrawableSorter::List()) {}

    void Main() override {
        window = new sf::RenderWindow{ sf::VideoMode(x, y), NAME, sf::Style::Close | sf::Style::Titlebar };
        if (window->isOpen()) {
            logger->SendSignal(this, INFO, "Window is created!");
        }
        else {
            logger->SendSignal(this, FATAL, "Window isn't created!");
        }
        
        while (window->isOpen() && work)
        {
            
            sf::Event event;
            while (window->pollEvent(event))
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
                    Destroy();
                    logger->SendSignal(this, FATAL, "Window is closed!");
                }
            }
            window->clear();
            for (poolIter iter = pool.begin(); iter != pool.end(); iter++) {
                window->draw(*(*iter)->drawable);
            }
            window->display();
        }

        Destroy();

        Exit();
    }

    size_t SendDrawablePos(Drawable* d) {
        pool += d;
        return pool.last();
    }

    size_t SendDrawablePos(Parent* p, sf::Drawable* d, size_t l) {
        pool += new Drawable(p, d ,l);
        return pool.last();
    }

    void RemoveBeginDrawable() {
        pool-=0;
    }

    void RemoveEndDrawable() {
        pool--;
    }

    void RemoveDrawable(size_t pos) {
        if (pos >= pool.size()) { logger->SendSignal(this, ERR, "RemoveDrawable: Position is invalid!"); }
        size_t i = 0;
        for (poolIter iter = pool.begin(); iter != pool.end(); iter++) {
            if (i == pos) {
                pool -= iter;
                return;
            }
            else {
                iter++;
            }
            i++;
        }
    }

    void Cleanup(Parent* elem) {
        for (poolIter iter = pool.begin(); iter != pool.end(); iter++) {
            if ((*iter)->elem == elem) {
                pool -= iter;
            }
            else {
                iter++;
            }
        }
    }

    void Destroy() override {
        window->close();
    }

    ~Window() override {
        Destroy();
    }
};

