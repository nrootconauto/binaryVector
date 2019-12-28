#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec.write(0,0b11001101);
		binaryVector::binaryVectorView<unsigned char> view(vec,2);
		vec<<=9;
		vec>>=9;
		//view.assignChunk(0b10111101,1);
		std::cout<<"ReaD:"<<(unsigned int)(view.read(0))<<std::endl;
		std::cout<<"Read2:"<<(unsigned int )(view.read(1))<<std::endl;
		//view<<=2;
		view.write(0,0xff);
		std::cout<<"Hello World one:"<<vec<<std::endl;
		std::cout<<"Hello World two:"<<view<<std::endl;
		return 0;
	}
