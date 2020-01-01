#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec.write(0,0);
		binaryVector::binaryVectorView<unsigned char> view(vec,3);
		//vec<<=5+8;
		std::cout<<vec<<std::endl;
		//vec>>=5+8;
		
		view.internals().writeType(1,0);
		std::cout<<"abc: "<<view<<std::endl;
		std::cout<<"132:"<<view<<std::endl;
		view.internals().writeType(1, 0b11000011);
		std::cout<<"sdfgfgdfh:"<<view<<std::endl;
		vec<<=3;
		std::cout<<"gsdgsdfg:"<<view<<std::endl;
		view.write(0,2|4|16);
		std::cout<<"Hello World one :"<<view<<std::endl;
		view>>=1;
		std::cout<<"Hello World two :"<<view<<std::endl;
		return 0;
	}
 
