#ifndef LOGGER_HEADER
#define LOGGER_HEADER
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

typedef std::string LogString;
//SINGLETON Design Pattern was used here.
class Logger{
	public:
	    ~Logger();

		static Logger * Instance();
		void print(LogString);
	private:
		Logger(){};
		Logger(Logger const&){};
		Logger& operator=(Logger const&){return *this;};

		static Logger * m_Instance;
};

#define Log(o) Logger::Instance()->print(o)
#endif 
