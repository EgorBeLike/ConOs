#include "Define.h"
#include "Logger.h"

string EnumToStr(LoggerMessageLevel level) {
	switch (level)
	{
	case DEBUG:
		return "DEBUG";
	case INFO:
		return "INFO";
	case WARN:
		return "WARN";
	case ERR:
		return "ERROR";
	case FATAL:
		return "FATAL";
	case STARTED:
		return "START-INFO";
	case STOPPED:
		return "STOP-INFO";
	default:
		return "UNKNOWN";
	}
}

Parent::Parent(Logger* l, string name) {
	this->name = name;
	this->logger = l;
	this->work = true;
	this->logger->SendSignal(this, STARTED, "Starting...");
	this->functhread = new thread(&Parent::Main, this);
	this->functhread->detach();
}
void Parent::Exit() {
	this->work = false;
	this->logger->SendSignal(this, STOPPED, "Stopped!");
}

void Logger::LoggerWorker()
{
	if (!this->work) { if (cout.good()) { cout << "[DEBUG] [Logger-Worker] Logger isn't inited!\n"; } return; }
	cout << "[DEBUG] [Logger-Worker] Logger is inited!\n";
	while (this->work) {
		if (this->debug) { cout << to_string(this->threads.size()) + " " + to_string(this->pool.size()) + "\n"; }
		this->started = true;
		for (poolIter iter = this->pool.begin(); iter != this->pool.end(); iter++) {
			cout << (*iter + "\n").c_str();
		}
		this->pool.clear();
		this_thread::sleep_for(chrono::milliseconds(10));
		if (this->stop) { this->work = false; }
		if (this->started && this->threads.empty()) { this->stop = true; }
	}
	cout << "[DEBUG] [Logger-Worker] Logger is stopped!";
}
void Logger::RemoveThread(Parent* elem) {
	for (auto iter = this->threads.begin(); iter != this->threads.end(); iter++) {
		if (*iter == elem) {
			this->threads.erase(iter); 
		} 
	}
}
void Logger::SendSignal(Parent* elem, LoggerMessageLevel level, string message) {
	this->pool.push_back("[" + EnumToStr(level) + "] [" + elem->getName() + "] " + message);
	if (level == STARTED) {
		this->threads.push_back(elem);
	}
	else if (level == STOPPED) {
		RemoveThread(elem);
	}
	else if (level == FATAL) {
		for (auto& thrd : this->threads) {
			thrd->Exit();
		}
	}
}
