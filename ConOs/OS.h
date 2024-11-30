#pragma once
#include "Define.h"
#include "Logger.h"
#include "Window.h"
#include "FileSystem.h"

class OS : public Parent {
public:
    enum OSStatus {
        STARTED,
        CHECK,
        OK,
        CONFIG,
        FATALSCREEN
    };

    OSStatus stat;
    Window* window;
    FileSystem* fsystem;
    Config config;

    OS(Logger* l, Window* w, FileSystem* f, string name = "OS-Worker") : Parent(l, name), stat(STARTED), window(w), fsystem(f) { fsystem->config = &config; }

    bool isConfigLoaded(){}

    void Main() override {
        sf::Image logoImage{};

        fs::path logoPath = getABSPath("data\\images\\logo.png");

        if (!fs::exists(logoPath)) { logger->SendSignal(fsystem, ERR, logoPath.string() + " not found!"); }

        logoImage.loadFromFile(logoPath.string());
        logoImage.saveToFile(getABSPath("cache\\logo.png"));

        sf::Texture logoTexture{};
        logoTexture.loadFromImage(logoImage);

        sf::Sprite logoSprite{};
        logoSprite.setTexture(logoTexture);
        logoSprite.setTextureRect(sf::IntRect{ sf::Vector2i(0,0), sf::Vector2i(64, 32) });
        logoSprite.setScale(sf::Vector2f(2, 2));
        logoSprite.setPosition(sf::Vector2f(window->x / 2 - 32*2, window->y / 2 - 16*2 - 50));

        size_t logoPoolIter = window->SendDrawablePos(this, &logoSprite);
        this_thread::sleep_for(chrono::milliseconds(3000));
        window->RemoveDrawable(logoPoolIter);

        if (!fs::exists(config.loadOSPath)) {
            logger->SendSignal(fsystem, ERR, config.loadOSPath + " not found! Set OS status: CONFIG");
            sf::Font font{};
            font.loadFromFile(getABSPath("data\\fonts\\config.ttf"));
            font.getTexture(32).copyToImage().saveToFile(getABSPath("cache\\config.ttf.png"));
            sf::Text title{ "ConOS CONFIG MENU", font};
            title.setCharacterSize(32);
            title.setPosition(sf::Vector2f(20, 20));
            size_t titleIter = window->SendDrawablePos(this, &title);
            this_thread::sleep_for(chrono::milliseconds(3000));
        }
        window->Cleanup(this);
        logger->SendSignal(this, FATAL, "OS is closed! This signal sended for stopping all threads!");
        Exit();
    }
};