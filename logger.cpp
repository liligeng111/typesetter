#include "logger.h"
#include <iomanip>
#include <codecvt>
#include <windows.h>  
#include <stdio.h>
#include <stdlib.h>

chrono::high_resolution_clock::time_point Logger::start_time_;

Logger::Logger()
{
}


Logger::~Logger()
{
}


void Logger::progress(string msg)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	cout << fixed << setprecision(3) << chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time_).count() / 1000.0 << "s--" << msg << endl;
}

void Logger::error(int code, string msg)
{
	/*
	1xx for hyphenating related error
	2xx for input related
	3xx for magic relater
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	cout << "Error " << code << ": " << msg << endl;
}

void Logger::set_start_time()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	start_time_ = chrono::high_resolution_clock::now();
	cout << endl << "Start Typesetting" << endl;
}