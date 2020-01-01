#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec.write(0,0);
		binaryVector::binaryVectorView<unsigned char> view(vec,3);
		//vec<<=5+8;
		std::cout<<vec<<std::endl;
		//vec>>=5+8;
		
		view.internals().writeType(1,0b10000000);
		std::cout<<"abc: "<<view<<std::endl;
		std::cout<<"132:"<<view<<std::endl;
		view.internals().writeType(0,0b10000000);
		std::cout<<"sdfgfgdfh:"<<view<<std::endl;
		std::cout<<"gsdgsdfg:"<<view<<std::endl;
		view.write(2,0b00010000);
		std::cout<<"Hello World one :"<<view<<std::endl;
		//view.write(0,64);
		//view.write(1,64);
		//view.write(2,64);
		//view.internals().parent->clipEndExtraBits();
		view>>=5;
		std::cout<<"Hello World two :"<<view<<std::endl;
		return 0;
	}
 
