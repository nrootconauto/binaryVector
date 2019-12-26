#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8);
		vec[0]=0x1f;
		//vec<<=1;
		std::cout<<"Hello World"<<vec<<std::endl;
		return 0;
	}
