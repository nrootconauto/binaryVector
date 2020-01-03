#include <iostream>
#include "../include/binaryVector.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>
#include <utility>
#include "doctest.h"
	
	typedef unsigned  int testType;
	TEST_CASE_TEMPLATE_DEFINE("shift tests",T,shift_tests) {
		const int shiftBy=2;
		binaryVector::binaryVector<T> vec(8*sizeof(testType));
		SUBCASE("left shifting ") {
			testType compareTo=0b110011011;
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
			testType compareTo=0b11110011<<8*(sizeof(testType)-1);
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
				vec.loadValue(value);
				auto computed=(~vec).template loadIntoPrimitive<testType>(0);
				REQUIRE_MESSAGE(computed==~value,"Binary Inverse");
			}
		}
		MESSAGE("Binary Negation works");
		SUBCASE("& and") {
			std::vector<std::pair<testType,testType>> testValues {
				{0xe,0xfe},
				{0x1221,0x3214},
				{0xefe23,0xffe1}
			};
			binaryVector::binaryVector vecA(8*sizeof(testType));
			binaryVector::binaryVector vecB(8*sizeof(testType));
			for(auto value:testValues) {
				auto [aValue,bValue]=value;
				vecA.loadValue(aValue);
				vecB.loadValue(bValue);
				testType result=(vecA&vecB).template loadIntoPrimitive<testType>(0);
				REQUIRE_MESSAGE((aValue&bValue),"Binary and failed with "<<aValue<<" and "<<bValue);
			}
		}
		MESSAGE("Binary And works");
		SUBCASE("BinAry Or ") {
			std::vector<std::pair<testType,testType>> testValues {
				{0xe,0xfe},
				{0x1221,0x3214},
				{0xefe23,0xffe1}
			};
			binaryVector::binaryVector vecA(8*sizeof(testType));
			binaryVector::binaryVector vecB(8*sizeof(testType));
			for(auto value:testValues) {
				auto [aValue,bValue]=value;
				vecA.loadValue(aValue);
				vecB.loadValue(bValue);
				testType result=(vecA|vecB).template loadIntoPrimitive<testType>(0);
				REQUIRE_MESSAGE((aValue|bValue),"Binary or failed with "<<aValue<<" and "<<bValue);
			}
		}
		MESSAGE("Binary OR works");
		SUBCASE("BinAry XOr ") {
			std::vector<std::pair<testType,testType>> testValues {
				{0xe,0xfe},
				{0x1221,0x3214},
				{0xefe23,0xffe1}
			};
			binaryVector::binaryVector vecA(8*sizeof(testType));
			binaryVector::binaryVector vecB(8*sizeof(testType));
			for(auto value:testValues) {
				auto [aValue,bValue]=value;
				vecA.loadValue(aValue);
				vecB.loadValue(bValue);
				testType result=(vecA^vecB).template loadIntoPrimitive<testType>(0);
				REQUIRE_MESSAGE((aValue^bValue),"Binary xor failed with "<<aValue<<" and "<<bValue);
			}
		}
		MESSAGE("Binary XOR works");
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
