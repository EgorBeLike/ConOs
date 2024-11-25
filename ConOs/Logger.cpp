#include "Define.h"
#include "Logger.h"

Parent::Parent(Logger* l) {
	this->logger = l;
	this->work = true;
	this->logger->SendSignal(this, STARTED, "[" + name + "] Starting...");
	this->functhread = new thread(&Parent::Main, this);
	this->functhread->detach();
}
void Parent::Exit() {
	this->work = false;
	this->logger->SendSignal(this, STOPPED, "[" + name + "] Stopped!");
}

Logger::Logger(ostream * stream) {
	this->strm = stream;
	this->work = true;
}
void Logger::LoggerWorker()
{
	if (!this->work) { if (this->strm->good()) { this->strm->operator<<("[DEBUG] Logger isn't inited!"); } }
	while (this->work) {
		for (poolIter iter = this->pool.end(); iter != this->pool.begin(); iter--) {
			this->strm->operator<<((*iter).c_str());
		}
		this_thread::sleep_for(chrono::milliseconds(10));
	}
	this->strm->operator<<("[DEBUG] Logger is stopped!");
}
Logger::threadsIter Logger::FindThread(Parent* elem) {
	for (Logger::threadsIter iter = this->threads.begin(); iter != this->threads.end(); iter++) { if (*iter == elem) { return iter; } }
}
void Logger::SendSignal(Parent* elem, LoggerMessageLevel level, string message) {
	string formatted;
	if (level == STARTED) {
		this->threads.push_back(elem);
	}
	if (level == STOPPED) {
		this->threads.erase(FindThread(elem), FindThread(elem));
	}
	if (level == FATAL) {
		for (auto& thrd : this->threads) {
			thrd->Exit();
		}
		this->work = false;
	}
}
