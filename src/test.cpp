#include <iostream>
#include "../include/binaryVector.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
	typedef unsigned  int testType;
	TEST_CASE_TEMPLATE_DEFINE("shift tests",T,shift_tests) {
		binaryVector::binaryVector<T> vec(8*sizeof(testType));
		SUBCASE("left shifting by one") {
			testType compareTo=0b11001101;
			vec.write(0,0b11001101);
			auto cap=8*sizeof(uint64_t);
			for(auto index=0;cap!=index;index++) {
				testType computed=vec.template loadIntoPrimitive<testType>(0);
				std::cout<<vec<<std::endl;
				REQUIRE_MESSAGE(compareTo==computed,"left shifting failed by 1 at "<<index<<" expected "<<compareTo<<" but got"<<computed);
				compareTo<<=1;
				vec<<=1;
			}
		};
		SUBCASE("right shifting by one") {
			
		};
	}
	//
	TEST_CASE_TEMPLATE_INVOKE(shift_tests,unsigned char);
	/*
	int main() {
		//
		binaryVector::binaryVector<unsigned char> vec(8*sizeof(unsigned int));
		vec.write(0, 0xff);
		vec.write(1, 1);
		vec<<=16;
		std::cout<<vec<<std::endl;
		vec>>=17;
		std::cout<<vec<<std::endl;
		unsigned int me=vec.loadIntoPrimitive<unsigned int>(0);
		std::cout<<me<<std::endl;
		return 0;
	}
	*/
