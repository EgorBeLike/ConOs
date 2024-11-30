#include "Define.h"
#include "Logger.h"
#include "Window.h"
#include "OS.h"
#include "FileSystem.h"

FileSystem::FileSystem(Logger* l, string name) : Parent(l, name) {}

void FileSystem::CreateNewFS() {
    this->mainDisk.Write(Constants::FSFileTypeOffset, Constants::FSFileTypeMessage, Constants::FSFileTypeSize);
}

void FileSystem::Main() {
    this->logger->SendSignal(this, INFO, "Waiting for load config...");
    while (!this->config->isLoaded) {
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    this->logger->SendSignal(this, INFO, "Config founded!");
    this->mainDisk.Load(this->config->loadOSPath);
    Exit();
}