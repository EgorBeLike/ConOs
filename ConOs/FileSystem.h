#pragma once
#include "Define.h"
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
    ConcfgLoadStatus stat = OK;
    bool isLoaded = false;
    ConcfgLoadStatus Load(string path) {
        configDisk = Disk(path);
        if ((stat = ReadAll()) != OK) { return stat; }
    }

    Config(Logger* l, string path) : logger(l) { Load(path); }
    Config(Logger* l) : logger(l) {}

    ConcfgLoadStatus ReadAll() {
        if(configDisk.fspath.ends_with(Constants::ConcfgFileType))
    }

    ConcfgLoadStatus Write() {

    }
};

class FileSystem : public Parent {
public:
    Disk mainDisk;
    Config config;
    bool configLoaded = false;
    FileSystem(Logger* l, string name = "FileSystem-Worker");
    void CreateNewFS();
    void Main() override;
};