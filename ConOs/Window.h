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
    vector<sf::Drawable*> pool;
    sf::RenderWindow* window;
    using poolIter = vector<sf::Drawable*>::iterator;
    Window(Logger* l, string name = "Window-Worker") : Parent(l, name) {}
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
            for (auto& sp : pool) {
                window->draw(*sp);
            }
            window->clear();
            window->display();
        }

        Destroy();

        Exit();
    }
    
    poolIter SendDrawable(sf::Drawable* sp) {
        pool.push_back(sp);
        return pool.end();
    }

    size_t SendDrawablePos(sf::Drawable* sp) {
        pool.push_back(sp);
        return pool.size() - 1;
    }

    void RemoveBeginDrawable() {
        pool.erase(pool.begin());
    }

    void RemoveEndDrawable() {
        pool.erase(pool.end());
    }

    void RemoveDrawable(poolIter& iter) {
        pool.erase(iter);
    }

    void RemoveDrawable(size_t pos) {
        if (pos >= pool.size()) { logger->SendSignal(this, ERR, "RemoveDrawable: Position is invalid!"); }
        size_t i = 0;
        for (poolIter iter = pool.begin(); iter != pool.end(); iter++) {
            if (i == pos) {
                pool.erase(iter);
                return;
            }
            i++;
        }
    }

    void Destroy() override {
        window->close();
    }

    ~Window() override {
        Destroy();
    }
};

