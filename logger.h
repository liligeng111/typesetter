#pragma once

#include <chrono>
#include <string>
#include <iostream>

using namespace std;

class Logger
{
public:
	Logger();
	~Logger();

	static void progress(string msg);
	static void error(int code, string msg);
	static void set_start_time();
private:
	static chrono::high_resolution_clock::time_point start_time_;
};

