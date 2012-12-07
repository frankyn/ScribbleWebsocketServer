#include "./common/Logger.h"
#include "./ListenerClass.h"

int main(){
	try{
		ListenerClass LS = ListenerClass(1035, 10, 5);
		LS.Wait();
	} catch (...) {
		std::cout<<"HELP"<<std::endl;
	}
	return 0;
}
