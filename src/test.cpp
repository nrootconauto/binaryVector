#include <iostream>
#include "../include/binaryVector.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
	
	typedef unsigned  int testType;
	TEST_CASE_TEMPLATE_DEFINE("shift tests",T,shift_tests) {
		const int shiftBy=2;
		binaryVector::binaryVector<T> vec(8*sizeof(testType));
		SUBCASE("left shifting ") {
			testType compareTo=0b11001101;
			vec.loadValue(compareTo);
			auto cap=8*sizeof(testType)/shiftBy;
			for(auto index=0;cap!=index;index++) {
				testType computed=vec.template loadIntoPrimitive<testType>(0);
				REQUIRE_MESSAGE(compareTo==computed,"left shifting failed by 1 at "<<index<<" expected "<<compareTo<<" but got"<<computed);
				compareTo<<=shiftBy;
				vec<<=shiftBy;
			}
		};
		SUBCASE("right shifting") {
			testType compareTo=0b11000111<<8*(sizeof(testType)-1);
			vec.loadValue(compareTo);
			auto cap=8*sizeof(testType)/shiftBy;
			for(auto index=0;index!=cap;index++) {
				testType computed=vec.template loadIntoPrimitive<testType>(0);
				REQUIRE_MESSAGE(computed==compareTo,"right shifting failed by 1 at"<<index<<" expected "<<compareTo<<" but got "<<computed);
				compareTo>>=shiftBy;
				vec>>=shiftBy;
			}
		};
		SUBCASE("~negation") {
			std::vector<testType> values {0xff,0xf113f,0x1ef};
			for(auto& value:values) {
				testType computed;
				vec.loadValue(value);
			}
		}
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
