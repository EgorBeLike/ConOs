#pragma once
#ifndef CONOS_LOGGER
#define CONOS_LOGGER 1

#include "Define.h"
#include "Util.h"

class Parent {
public:
	Logger* logger;
	bool work = false;
	thread* functhread;
	string name = "Worker";
	virtual void Main() {}
	Parent(Logger*, string = "Worker");
	constexpr inline string getName() const { return name; }
	void Exit();
	virtual void Destroy() {}
	virtual ~Parent() {}
};

class Logger {
public:
	vector<string> pool;
	vector<Parent*> threads;
	using threadsIter = vector<Parent*>::iterator;
	using poolIter = vector<string>::iterator;
	bool work;
	bool started;
	bool stop;
	Logger(bool isDebug = false) : work(true), started(false), stop(false) {}
	void LoggerWorker();
	void RemoveThread(Parent*);
	void SendSignal(Parent*, LoggerMessageLevel::Values, string);
	void SendSignal(string, LoggerMessageLevel::Values, string);
};

#endif