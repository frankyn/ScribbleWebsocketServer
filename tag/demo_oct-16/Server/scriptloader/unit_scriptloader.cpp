#include <iostream>
#include "ScriptLoader.h"


int main ( ) {
	ScriptLoader scriptloader;
	scriptloader.load ( "test.lua" );
	std::cout<<scriptloader.parse ( "Frank is cool" )<<std::endl;
	return 0;
};
