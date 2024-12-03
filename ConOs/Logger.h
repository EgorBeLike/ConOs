#pragma once
#include "Define.h"

enum LoggerMessageLevel {
	DEBUG = -1,
	INFO,
	WARN,
	ERR,
	FATAL,
	STARTED,
	STOPPED
};

string EnumToStr(LoggerMessageLevel);

class Logger;

class Parent {
public:
	Logger* logger;
	bool work = false;
	thread* functhread;
	string name = "Worker";
	virtual void Main() {}
	Parent(Logger*, string name = "Worker");
	constexpr inline string getName() { return name; }
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
	void SendSignal(Parent*, LoggerMessageLevel, string);
	void SendSignal(string, LoggerMessageLevel, string);
};