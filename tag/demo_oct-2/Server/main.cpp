#include "./common/Logger.h"
#include "./ListenerClass.h"

int main(){
	ListenerClass LS = ListenerClass(1035,10);
	LS.Wait();
	
	return 0;
}
