#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec[0]=0b11001101;
		binaryVector::binaryVectorView<unsigned char> view(&vec,2);
		vec<<=9;
		std::cout<<"Hello World one:"<<vec<<std::endl;
		vec>>=9;
		view.assignChunk(0xff,0);
		std::cout<<"Hello World two:"<<vec<<std::endl;
		return 0;
	}
