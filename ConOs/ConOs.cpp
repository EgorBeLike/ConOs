#include "Define.h"
#include "Window.h"
#include "Logger.h"

int main()
{
    Logger* logger = new Logger(&cout);
    Window* window = new Window(logger);

    thread* lthread = new thread(&Logger::LoggerWorker, logger);
    lthread->detach();
}