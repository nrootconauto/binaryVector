#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec.write(0,0b11001101);
		binaryVector::binaryVectorView<unsigned char> view(vec,2);
		vec<<=9;
		std::cout<<"ReaD:"<<(unsigned int)(view.read(0))<<std::endl;
		std::cout<<"Read2:"<<(unsigned int )(view.read(1))<<std::endl;
		view.write(0,0b11011011);
		view<<=2;
		std::cout<<"Hello World one:"<<vec<<std::endl;
		std::cout<<"Hello World two:"<<view<<std::endl;
		return 0;
	}
