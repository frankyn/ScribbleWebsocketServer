#include "Logger.h"

Logger * Logger::m_Instance =  NULL;
Logger::~Logger(){
	if(m_Instance)
		delete m_Instance;
}
Logger * Logger::Instance(){
	if(!m_Instance)
		m_Instance = new Logger();

	return m_Instance;
}

void Logger::print(LogString output){
	std::cout<<output<<std::endl;
} 