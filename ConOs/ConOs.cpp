#include "Define.h"
#include "Window.h"
#include "Logger.h"
#include "FileSystem.h"
#include "OS.h"

int main()
{
    cout << string(NAME) + " v1.0\n";
    Logger* logger = new Logger();
    Window* window = new Window(logger);
    FileSystem* filesys = new FileSystem(logger);
    OS* os = new OS(logger, window, filesys);

    thread* lthread = new thread(&Logger::LoggerWorker, logger);
    lthread->join();
}