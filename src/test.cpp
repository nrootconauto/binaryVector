#include <iostream>
#include "../include/binaryVector.hpp"
	
	int main() {
		binaryVector::binaryVector<unsigned char> vec(8*sizeof(unsigned int));
		vec.write(0, 0xff);
		vec.write(1, 1);
		unsigned int me=vec.loadIntoPrimitive<unsigned int>(0);
		std::cout<<me<<std::endl;
		return 0;
	}
 
