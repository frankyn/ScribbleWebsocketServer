#include "json_writer.h"

/*
class JsonWriter{
	public:
		JsonWriter();
   	   ~JsonWriter();
		void writeDouble(std::string, double);
		void writeString(std::string, std::string);
		void writeBool(std::string, bool);
		void writeObjectOpen(std::string);
		void writeObjectClose();
		std::string dumpJson();
		void clear();
	private:
		std::string jsonOutput;
};*/
namespace jsonxx{
	 
JsonWriter::JsonWriter(){
	jsonOutput = "{";
}
JsonWriter::~JsonWriter(){
	clear();
}
void JsonWriter::writeInt(std::string name, int value){
	char num[11];
	memset(num,0x0,11);
	sprintf(num,"%d",value);
	jsonOutput += "\""+name+"\":"+std::string(num,10)+",";
}
void JsonWriter::writeString(std::string name, std::string value){
	jsonOutput += "\""+name+"\":\""+value+"\",";
}
void JsonWriter::writeBool(std::string name, bool value){
	jsonOutput += "\""+name+"\":"+(value ? "true" : "false")+",";
}
void JsonWriter::writeObjectOpen(std::string name){
	jsonOutput += "\""+name+"\":{";
}
void JsonWriter::writeObjectClose(){
	jsonOutput = jsonOutput.substr(0, jsonOutput.length()-2);
	jsonOutput += "},";
}
std::string JsonWriter::dumpJson(){
	jsonOutput = jsonOutput.substr(0, jsonOutput.length()-2);
	std::string outTemp = jsonOutput+"}";
	clear();
	jsonOutput = "{";
	return outTemp;
}
void JsonWriter::clear(){
	jsonOutput.erase(jsonOutput.begin(),jsonOutput.end());
}
}