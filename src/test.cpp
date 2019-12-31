#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*3);
		vec.write(0,0b11111111);
		binaryVector::binaryVectorView<unsigned char> view(vec,4,3);
		vec<<=5;
		std::cout<<vec<<std::endl;
		vec>>=5;
		std::cout<<"Hello World ze0o:"<<view<<std::endl;
		//view.write(0,0b11011011);
		view<<=2;
		std::cout<<"Hello World one :"<<vec<<std::endl;
		std::cout<<"Hello World two :"<<~vec<<std::endl;
		return 0;
	}
