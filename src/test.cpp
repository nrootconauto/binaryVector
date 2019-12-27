#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec[0]=0b11001101;
		binaryVector::binaryVectorView<unsigned char> view(&vec,2);
		vec<<=9;
		std::cout<<"Hello World one:"<<view<<std::endl;
		vec>>=9;
		std::cout<<"Hello World two:"<<view<<std::endl;
		return 0;
	}
