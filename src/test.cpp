#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec.write(0,0b11111111);
		binaryVector::binaryVectorView<unsigned char> view(vec,3,8*3-3);
		//vec<<=5+8;
		std::cout<<vec<<std::endl;
		//vec>>=5+8;
		
		view.internals().writeType(1,0b11100111);
		std::cout<<"abc: "<<view<<std::endl;
		std::cout<<"132:"<<vec<<std::endl;
		view.internals().writeType(0, 2);
		std::cout<<"sdfgfgdfh:"<<view<<std::endl;
		view<<=1;
		for(auto i=0;i!=view.internals().size();i++)
			std::cout<<"Hello World ze0o:"<<(int)view.internals().readType(i)<<std::endl;
		std::cout<<"gsdgsdfg:"<<view<<std::endl;
		//view.write(0,0b11011011);
		
		std::cout<<"Hello World one :"<<view<<std::endl;
		std::cout<<"Hello World two :"<<vec<<std::endl;
		return 0;
	}
 
