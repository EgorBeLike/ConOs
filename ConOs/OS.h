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
    OS(Logger* l, Window* w, FileSystem* f, string name = "OS-Worker") : Parent(l, name), stat(STARTED), window(w), fsystem(f) {}

    void Main() override {
        sf::Image logoImage{};
        fs::path logoPath = fsystem->getABSPath("data\\images\\logo.png");
        if (!fs::exists(logoPath)) {
            logger->SendSignal(fsystem, FATAL, logoPath.string() + " not found!");
            return;
        }
        logoImage.loadFromFile(logoPath.string());
        logoImage.saveToFile(fsystem->getABSPath("cache\\logo.png"));
        sf::Texture logoTexture{};
        logoTexture.loadFromImage(logoImage, sf::IntRect{ sf::Vector2i(window->x - 32, window->y - 16), sf::Vector2i(64, 32) });
        sf::Sprite logoSprite{ logoTexture };
        size_t logoPoolIter = window->SendDrawablePos(&logoSprite);
        this_thread::sleep_for(chrono::milliseconds(3000));
        window->RemoveDrawable(logoPoolIter);
        if (!fs::exists(fsystem->fspath)) {
            logger->SendSignal(fsystem, ERR, fsystem->fspath + " not found! Set OS status: CONFIG");
            sf::Font font{};
            font.loadFromFile(fsystem->getABSPath("data\\fonts\\config.ttf"));
            font.getTexture(32).copyToImage().saveToFile(fsystem->getABSPath("cache\\config.ttf.png"));
            sf::Text title{ "ConOS CONFIG MENU", font};
            title.setCharacterSize(32);
            title.setPosition(sf::Vector2f(20, 20));
            window->SendDrawablePos(&title);
        }
        Exit();
    }
};