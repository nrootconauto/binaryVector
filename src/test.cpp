#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec.write(0,0xff);
		binaryVector::binaryVectorView<unsigned char> view(vec,4);
		vec<<=10;
		vec>>=10;
		std::cout<<"Hello World zero:"<<view<<std::endl;
		//view.write(0,0b11011011);
		view<<=2;
		std::cout<<"Hello World one :"<<vec<<std::endl;
		std::cout<<"Hello World two :"<<~vec<<std::endl;
		return 0;
	}
