#pragma once
#include "Define.h"
#include "Logger.h"
#include "Window.h"

static inline string getABSPath(const char* path = "") {
    vector<char> pathBuf;
    DWORD copied = 0;
    do {
        pathBuf.resize(pathBuf.size() + MAX_PATH);
        copied = GetModuleFileNameA(nullptr, &pathBuf.at(0), pathBuf.size());
    } while (copied >= pathBuf.size());
    pathBuf.resize(copied);
    string path_(pathBuf.begin(), pathBuf.end());
    return fs::path(path_).parent_path().string() + "\\" + string(path);
}

using message = vector<char>;

static message StringToMessage(string str) {
    message mess;
    for (auto& ch : str) {
        mess.push_back(ch);
    }
    return mess;
}

namespace Constants {
    const message FSFileTypeMessage = StringToMessage("ConOSFileSystem");
    const size_t  FSFileTypeOffset = 0;
    const size_t  FSFileTypeSize = FSFileTypeMessage.size();
    const size_t  FSFileVerOffset = FSFileTypeOffset + FSFileTypeSize;
    const size_t  FSFileVerSize = 10;
    const size_t  UseReservedOffset = FSFileVerOffset + FSFileVerSize;
    const size_t  UseReservedSize = 1;
    const size_t  FSSizeOffset = UseReservedOffset + UseReservedSize;
    const size_t  FSSizeSize = 20;
    const size_t  OSFilesOffset = FSSizeOffset + FSSizeSize;
    const size_t  OSFilesSize = 1073741824;
    const size_t  ReservedOffset = OSFilesOffset + OSFilesSize;
    const size_t  ReservedSize = 1073741824;
    const size_t  FSOffset = ReservedOffset + ReservedSize;
    const size_t  FSSize = 35184372088832;
}

struct Disk {
    using fileState = _Iosb<int>;
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
    }
};

namespace Constants {

}

struct Config {
    string loadOSPath;
    Disk configDisk;
    bool isLoaded = false;
    void Read(string path) {

    }
    void Write(string path) {

    }
};

class FileSystem : public Parent {
public:
    Disk mainDisk;
    Config* config;
    bool configLoaded = false;
    FileSystem(Logger* l, string name = "FileSystem-Worker");
    void CreateNewFS();
    void Main() override;
};