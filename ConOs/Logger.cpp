#include "Define.h"
#include "Util.h"
#include "Logger.h"

Parent::Parent(Logger* l, string name) {
	this->name = name;
	this->logger = l;
	this->work = true;
	this->logger->SendSignal(this, LoggerMessageLevel::STARTED, "Starting...");
	this->functhread = new thread(&Parent::Main, this);
	this->functhread->detach();
}
void Parent::Exit() {
	this->work = false;
	this->logger->SendSignal(this, LoggerMessageLevel::STOPPED, "Stopped!");
}

void Logger::LoggerWorker()
{
	if (!this->work) { if (cout.good()) { cout << "[DEBUG] [Logger-Worker] Logger isn't inited!\n"; } return; }
	cout << "[DEBUG] [Logger-Worker] Logger is inited!\n";
	while (this->work) {
		//if (USE_ASSERT) { cout << to_string(this->threads.size()) + " " + to_string(this->pool.size()) + "\n"; }
		this->started = true;
		vector<string> pool_ = this->pool;
		this->pool.clear();
		for (poolIter iter = pool_.begin(); iter != pool_.end(); iter++) {
			cout << (*iter + "\n").c_str();
		}
		this_thread::sleep_for(chrono::milliseconds(10));
		if (this->stop) { this->work = false; }
		if (this->started && this->threads.empty()) { this->stop = true; }
	}
	cout << "[DEBUG] [Logger-Worker] Logger is stopped!";
}
void Logger::RemoveThread(Parent* elem) {
	size_t tc = this->threads.size();
	bool done = false;
	while (!done) {
		tc = this->threads.size();
		for (auto iter = this->threads.begin(); iter != this->threads.end(); iter++) {
			if (tc != this->threads.size()) { break; }
			if (*iter == elem) {
				this->threads.erase(iter);
				done = true;
			}
			else {
				iter++;
			}
		}
	}
}
void Logger::SendSignal(string name, LoggerMessageLevel::Values level, string message) {
	this->pool.push_back("[" + LoggerEnumToStr(level) + "] [" + name + "] " + message);
	if (level == LoggerMessageLevel::STARTED || level == LoggerMessageLevel::STOPPED) {
		throw exception((LoggerEnumToStr(level) + " not supported").c_str());
	}
	if (level == LoggerMessageLevel::FATAL) {
		for (auto& thrd : this->threads) {
			thrd->work = false;
		}
	}
}
void Logger::SendSignal(Parent* elem, LoggerMessageLevel::Values level, string message) {
	this->pool.push_back("[" + LoggerEnumToStr(level) + "] [" + elem->getName() + "] " + message);
	if (level == LoggerMessageLevel::STARTED) {
		this->threads.push_back(elem);
	}
	else if (level == LoggerMessageLevel::STOPPED) {
		RemoveThread(elem);
	}
	else if (level == LoggerMessageLevel::FATAL) {
		for (auto& thrd : this->threads) {
			thrd->work = false;
		}
	}
}
