#include "./common/Logger.h"
#include "./ListenerClass.h"

int main(){
	try{
		ListenerClass LS = ListenerClass(1035, 10, 5);
	} catch (const char * e) {
		std::cout<<"Error: "<<e<<std::endl;
	}
	return 0;
}
