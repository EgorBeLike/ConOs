#pragma once
#ifndef CONOS_FILESYSTEM
#define CONOS_FILESYSTEM 1

#include "Define.h"
#include "Util.h"
#include "Logger.h"
#include "Window.h"

struct Disk {
    using fileState = ios_base::iostate;
    string fspath;
    fstream str;
    void Load(string path) {
        fspath = path;
        str = fstream{ fspath, ios_base::binary | ios_base::ate | ios_base::in | ios_base::out };
    }
    Disk(string path) { Load(path); }
    Disk() {}
    fileState Write(size_t offset, message mess, size_t block) {
        str.seekg(offset);
        for (auto& ch : mess) {
            str.write(&ch, 1);
        }
        for (size_t i = 0; i < block - mess.size(); i++) {
            str.write("\0", 1);
        }
        return str.rdstate();
    }
};

struct Config {
    string loadOSPath;
    Disk configDisk;
    Logger* logger;
    ConcfgLoadStatus::Values stat = ConcfgLoadStatus::OK;
    bool isLoaded = false;
    ConcfgLoadStatus::Values Load(string path) {
        configDisk = Disk(path);
        if ((stat = ReadAll()) != ConcfgLoadStatus::OK) { return stat; }
    }

    Config(Logger* l, string path) : logger(l) { Load(path); }
    Config(Logger* l) : logger(l) {}

    ConcfgLoadStatus::Values ReadAll() {
        
    }

    ConcfgLoadStatus::Values Write() {

    }
};

class FileSystem : public Parent {
protected:
    Disk mainDisk;
    Config config;
public:
    bool configLoaded = false;
    FileSystem(Logger* l, string name = "FileSystem-Worker") : Parent(l, name), config(l) {
        if (!fs::exists(getABSPath("fs\\"))) {
            logger->SendSignal(this, LoggerMessageLevel::ERR, "File Systems folder not found... Preparing new FS...");
        }
    }
    void CreateNewFS(string path) {
        
    }
    void Main() override {
        this->logger->SendSignal(this, LoggerMessageLevel::INFO, "Waiting for load config...");
        while (!this->config.isLoaded) {
            if (!this->work) {
                Exit();
                return;
            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        this->logger->SendSignal(this, LoggerMessageLevel::INFO, "Config founded!");
        this->mainDisk.Load(this->config.loadOSPath);
        Exit();
    }
    Config* getCfg() { return &config; }
    Disk* getMainDisk() { return &mainDisk; }
};

#endif