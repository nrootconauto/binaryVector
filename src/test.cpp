#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec[0]=0b10110101;
		vec<<=9;
		std::cout<<"Hello World"<<vec<<std::endl;
		return 0;
	}
