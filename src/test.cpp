#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec[1]=0xff;
		vec<<=9;
		std::cout<<"Hello World 0"<<vec<<std::endl;
		vec>>=9;
		std::cout<<"Hello World"<<vec<<std::endl;
		return 0;
	}
