#include "Define.h"
#include "Window.h"
#include "Logger.h"

int main()
{
    cout << string(NAME) + " v1.0\n";
    Logger* logger = new Logger();
    Window* window = new Window(logger);

    thread* lthread = new thread(&Logger::LoggerWorker, logger);
    lthread->join();
}