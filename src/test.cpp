#include <iostream>
#include "../include/binaryVector.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>
#include <utility>
	#include "doctest.h"
	typedef unsigned  int testType;
	TEST_CASE_TEMPLATE_DEFINE("View tests",T,view_tests) {
		const unsigned int viewIndex=10;
		const unsigned int viewWidth=17;
		const unsigned int size=sizeof(unsigned int)*8;
		const unsigned int mask=0x1ffff;
		const unsigned int viewMask=mask<<viewIndex;
		SUBCASE("view left shift") {
			const int shiftAmount=2;
			unsigned int value=0xffffffff;
			unsigned int originalValue=value;
			binaryVector::binaryVector<T> vec(size);
			binaryVector::binaryVectorView<T> view(vec,viewIndex,viewWidth);
			vec.loadValue(value);
			//view.writeBlock(1,0b10000001);
			for(int i=0;i<viewWidth;i+=shiftAmount) {
				auto result=view.template loadIntoPrimitive<unsigned int>(0);
				auto mastervalue=vec.template loadIntoPrimitive<unsigned int>(0);
				//check the value of the window
				std::cout<<"vec :"<<vec<<std::endl;
				std::cout<<"view:"<<view<<std::endl;
				REQUIRE_MESSAGE(((mastervalue&viewMask)>>viewIndex)==((value&viewMask)>>viewIndex),"value");
				//check value outside of the "window"
				REQUIRE_MESSAGE(((originalValue&~viewMask))==(vec.template loadIntoPrimitive<unsigned int>(0)&~viewMask),"value outsie of");
				REQUIRE_MESSAGE((result)==((value>>viewIndex)&(mask)),"View doesnt match computed");
				//
				//value<<=shiftAmount;
				//view.writeBlock(1,0);
				//std::cout<<view<<std::endl;
				//view.writeBlock(0,0b11000000);
				view<<=shiftAmount;
				value=(value&~viewMask)|((value&viewMask)<<shiftAmount);
			}
		}
		SUBCASE("right view shifting") {
			const int shiftAmount=2;
			unsigned int value=0xffffffff;
			unsigned int originalValue=value;
			binaryVector::binaryVector<T> vec(size);
			binaryVector::binaryVectorView<T> view1(vec,viewIndex,viewWidth);
			binaryVector::binaryVectorView<T,decltype(view1)> view(view1);
			vec.loadValue(value);
			for(int i=0;i<viewWidth;i+=shiftAmount) {
				auto result=view.template loadIntoPrimitive<unsigned int>(0);
				auto mastervalue=vec.template loadIntoPrimitive<unsigned int>(0);
				//check the value of the window
				std::cout<<"vec :"<<vec<<std::endl;
				std::cout<<"view:"<<view<<std::endl;
				REQUIRE_MESSAGE(((mastervalue&viewMask)>>viewIndex)==((value&viewMask)>>viewIndex),"value");
				//check value outside of the "window"
				REQUIRE_MESSAGE(((originalValue&~viewMask))==(vec.template loadIntoPrimitive<unsigned int>(0)&~viewMask),"value outsie of");
				REQUIRE_MESSAGE((result)==((value>>viewIndex)&(mask)),"View doesnt match computed");
				//
				//value<<=shiftAmount;
				//view.writeBlock(1,0);
				//std::cout<<view<<std::endl;
				//view.writeBlock(0,0b11000000);
				view>>=shiftAmount;
				value=(value&~viewMask)|((value&viewMask)>>shiftAmount);
			}
		}
		SUBCASE("bitwise And") {
			unsigned int value=0xfe2324;
			unsigned int originalValue=value;
			unsigned int  toApply=0xe12;
			binaryVector::binaryVector<T> vec(sizeof(unsigned int)*8);
			vec.template loadValue(originalValue);
			binaryVector::binaryVector<T> byte(sizeof(unsigned int)*8);
			byte.loadValue(toApply);
			for(int i=0;i!=sizeof(unsigned int)*8;i++) {
				vec.template loadValue(originalValue);
				vec&=binaryVector::virtualShift(byte, -i);
				std::cout<<"shifted byte"<<binaryVector::virtualShift(byte, -i)<<std::endl;
				auto computedResult=vec.template loadIntoPrimitive<unsigned int>(0);
				auto referenceResult=originalValue&toApply>>i;
				std::cout<<"Referce expected"<<referenceResult<<std::endl;
				REQUIRE_MESSAGE(computedResult==referenceResult,"Bitwie and failed");
			}
			//slide the window and do an "&" operation
		}
	}
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
			binaryVector::binaryVector<unsigned int> vecA(8*sizeof(testType));
			binaryVector::binaryVector<unsigned int> vecB(8*sizeof(testType));
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
			binaryVector::binaryVector<T> vecA(8*sizeof(testType));
			binaryVector::binaryVector<T> vecB(8*sizeof(testType));
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
			binaryVector::binaryVector<testType> vecA(8*sizeof(testType));
			binaryVector::binaryVector<testType> vecB(8*sizeof(testType));
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
	//	TEST_CASE_TEMPLATE_INVOKE(shift_tests,unsigned int);
	TEST_CASE_TEMPLATE_INVOKE(view_tests,unsigned char);
