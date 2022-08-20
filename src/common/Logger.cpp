#include "Logger.h"

Logger *Logger::m_Instance = NULL;

Logger::~Logger() {
    if (m_Instance) {
        delete m_Instance;
    }
}

Logger *Logger::Instance() {
    if (!m_Instance) {
        m_Instance = new Logger();
    }
    return m_Instance;
}

void Logger::print(LogString output) {
    FILE *fp = fopen("logs/logger.txt", "a+");
    if (!fp) {
        return;
    }
    fwrite((output + "\n").c_str(), output.length() + 1, sizeof(char), fp);
    fclose(fp);
} 
