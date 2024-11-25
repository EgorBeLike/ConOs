#pragma once
#include "Define.h"

enum LoggerMessageLevel {
	DEBUG = -1,
	INFO,
	WARN,
	ERROR,
	FATAL,
	STARTED,
	STOPPED
};

class Logger;

class Parent {
public:
	Logger* logger;
	bool work = false;
	thread* functhread;
	string name = "Worker";
	virtual void Main() {}
	Parent(Logger* l);
	void Exit();
};

class Logger {
public:
	ostream* strm;
	vector<string> pool;
	vector<Parent*> threads;
	using threadsIter = vector<Parent*>::iterator;
	using poolIter = vector<string>::iterator;
	bool work;
	Logger(ostream*);
	void LoggerWorker();
	threadsIter FindThread(Parent*);
	void SendSignal(Parent*, LoggerMessageLevel, string);
};