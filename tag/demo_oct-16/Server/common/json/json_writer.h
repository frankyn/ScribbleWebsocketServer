#ifndef JSON_WRITER_HEADER
#define JSON_WRITER_HEADER
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
namespace jsonxx{
class JsonWriter{
	public:
		JsonWriter();
   	   ~JsonWriter();
		void writeInt(std::string, int);
		void writeString(std::string, std::string);
		void writeBool(std::string, bool);
		void writeObjectOpen(std::string);
		void writeObjectClose();
		std::string dumpJson();
		void clear();
	private:
		std::string jsonOutput;
};
}

#endif