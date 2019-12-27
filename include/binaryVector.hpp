#include "../include/binaryGraph.hpp" 
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
	namespace binaryVector {
			//binary Vector
			template<typename internal=size_t> class binaryVector {
				public:
					//stream stuff;
					friend std::ostream& operator<<(std::ostream& out,binaryVector<internal>& thing) {
						std::string str;
						//nibble lookup table
						std::vector<const char*> nibbleTable {
							"0000",
							"0001",
							"0010",
							"0011",
							"0100",
							"0101",
							"0110",
							"0111",
							"1000",
							"1001",
							"1010",
							"1011",
							"1100",
							"1101",
							"1110",
							"1111"
						};
						//got through the internals
						for(size_t Xinternal=0;Xinternal!=thing.internals().size();Xinternal++)
							for(size_t b=0;b!=(8*sizeof(internal))/4;b++)
								str=nibbleTable[0x0f&(thing[Xinternal]>>4*b)]+str;
						//stream it
						out<<str;
						return out;
					}
					//access operator
					internal& operator[](size_t where) {
						return this->internalVec[where];
					}
					//rezie for bits
					void resize(size_t bits) {
						//see if enough room in this for other at offset
						auto capacity=sizeof(internal)*this->internalVec.size()*8;
						if(capacity<bits) {
							//figure out how much to push back,frst statement handles the raminder
							auto toPushBack=((bits-capacity)%(8*sizeof(internal)))?1:0;
							toPushBack+=(bits-capacity)/(8*sizeof(internal));
							this->internalVec.resize(this->internalVec.size()+toPushBack,0);
						} else {
							//% is for remainder
							auto neededSize=bits/(sizeof(internal)*8)+((bits%(8*sizeof(internal)))?1:0);
							this->internalVec.resize(neededSize);
						}
						this->bitCount=bits;
						//put 0's in the raiminding bits
						this->clipEndExtraBits();
					}
					//internal is to be equal or lesser in size to otherInternal
					template<typename otherInternal> void copy(binaryVector<otherInternal>& other,int offset=0) {
						this->resize(offset+other.size());
						auto currentBit=offset;
						//offset to qapply for each internal
						auto perInternalOffset=offset%(sizeof(internal)*8);
						//amount of bits to push right for the remainding bits of perInternalOffset
						auto backwardsOffset=sizeof(internal)*8-perInternalOffset;
						//copy that shiz
						internal extra=0;
						const internal ones=~0;
						size_t bitsToWrite=other.bitCount;
						bool firstRun=true;
						
						//the lambda to do this shiz
						auto mergeIntoThis=[&](internal& piece)->void {
							//make a unit to fill up wir
							internal mask;
							//make the mask for a bigger thing
							if(bitsToWrite+perInternalOffset>=sizeof(internal)*8) {
								if(firstRun)
									mask=~(ones<<perInternalOffset);
								else
									mask=~ones;
							} else
								mask=ones>>backwardsOffset;
							auto& internalUnit=this->internalVec[currentBit/(8*sizeof(internal))];
							internalUnit=((piece<<perInternalOffset));
							//go to next bits
							currentBit+=sizeof(internal)-perInternalOffset;
						};
						//
						otherInternal leftOverBitsBig=0;
						internal leftOverBitsSmall=0;
						//if other has bigger internal size,chug in chunks
						if(sizeof(otherInternal)>sizeof(internal)) {
							const auto extraPump=(8*sizeof(otherInternal))%(sizeof(internal)*8);
							const internal ones=~0;
							internal chunk;
							for(otherInternal unit:other) {
								//dont factor in left over bits on first run
								if(firstRun) {
									firstRun=false;
									chunk=unit;
								}
								leftOverBitsBig=unit>>(sizeof(internal)*8);
								//factor in the left over bits
								if(!firstRun) {
									chunk=(unit<<extraPump)|leftOverBitsBig;
								}
								//merge "piece by peice"
								const auto totalSubpieces=sizeof(otherInternal)/sizeof(internal);
								for(int i=0;i!=totalSubpieces;i++) {
									internal peice=chunk;
									mergeIntoThis(peice);
									//one internal(of this) at a time
									chunk>>=(sizeof(internal)*8);
								}
							}
						} else {
							//amass value into internal size
							const auto totalSubpieces=sizeof(internal)/sizeof(otherInternal);
							int onWhichSubPiece=0;
							internal toChug=0;
							//remainding bytes after a internal lengthed "chug"
							const auto remaindingBytes=sizeof(internal)%sizeof(otherInternal);
							for(otherInternal unit:other) {
								//push dat shiz,reaminderBytes is the offset of the remainding
								auto remainderOffset=firstRun?0:8*remaindingBytes;
								toChug|=unit<<(8*sizeof(otherInternal)*(onWhichSubPiece-1)+remainderOffset);
								//if on the maximum amount of bytes that can fit in
								if(totalSubpieces+(remaindingBytes?1:0)==++onWhichSubPiece) {
									//if has ramindering bytes,add to extra;
									if(remaindingBytes) {
										leftOverBitsSmall=unit>>(8*remaindingBytes);
									}
									//chug
									mergeIntoThis(toChug);
									//
									//reset the count and chug
									toChug=leftOverBitsSmall;
									firstRun=false;
									onWhichSubPiece=0;
								}
							}
							//push the remainng bits if extra bits are available
							if(leftOverBitsSmall!=0)
								mergeIntoThis(toChug);
						}
						this->clipEndExtraBits();
					}
					//constructor
					binaryVector(size_t sizeInBits=0) {
						this->resize(sizeInBits);
					}
					//constructor
					binaryVector(internal* items,size_t count=-1) {
						//if count if not defined(==-1),get the array length
						if(count==-1)
							count=sizeof(items)/sizeof(internal);
						//copy that shiz
						for(int i=0;i!=count;i++)
							this->internalVec[i]=items[i];
						//count the bits
						this->bitCount=count*8*sizeof(internal);
					}
					//constructor
					template<class otherInteral> binaryVector(binaryVector<otherInteral>& other) {
						this->copy(other);
					}
					//
					binaryVector<internal>& operator <<=(int bits) {
						internal carryRegister;
						internal carryOver=0;
						auto Xinternals=bits/(sizeof(internal)*8);
						//end bit is pushed past end,clear binaryVector
						if(Xinternals>internalVec.size()) {
							for(auto& item:internalVec)
								item=0;
							return *this;
						} else {
							//amount of remaining bits  after shift
							auto remainder=bits%(sizeof(internal)*8);
							for(auto i=internalVec.size()-1;i>=Xinternals;i--) {
								//assume 0 if carring over before first bit
								if(i-Xinternals-1<0)
									carryOver=0;
								else
									carryOver=internalVec[i-Xinternals-1]>>(sizeof(internal)*8-remainder);
								//put in register
								carryRegister=(internalVec[i-Xinternals]<<remainder)|carryOver;
								//move left X internals
								internalVec[i]=carryRegister;
							}
							std::cout<<(*this)<<std::endl;
							//fill rest with 0s
							for(auto i=0;i<Xinternals;i++) {
								internalVec[i]=0;
							}
						}
						this->clipEndExtraBits();
						return *this;
					}
					binaryVector<internal> operator>>= (int bits) {
						//how many internals the shift spans
						auto Xinternals=bits/(sizeof(internal)*8);
						auto remainder=bits%(sizeof(internal)*8);
						internal leftOverBits=0;
						//go foward in for statement
						for(auto i=0;i<=internalVec.size();i++) {
							if(i+Xinternals+1<internalVec.size()) {
								//shift to get remianing bits,then shift to begining of next internal
								leftOverBits=internalVec[i+Xinternals+1]<<(sizeof(internal)*8-remainder);
							} else
								leftOverBits=0;
							//apply shift and clip
							if(i+Xinternals<internalVec.size())
								internalVec[i]=(internalVec[i+Xinternals]>>remainder)|leftOverBits;
							else
								internalVec[i]=0;
							//
						}
						this->clipEndExtraBits();
						return *this;
					}
					//iterator class
					template <typename internal_> class internalsIt: public std::iterator<std::bidirectional_iterator_tag, internal_> {
						public:
							internalsIt(binaryVector<internal_>* container_=nullptr,size_t where=0):offset(where),container(container_) {}
							internalsIt& operator++() {
								offset++;
								return *this;
							}
							internalsIt& operator--() {
								offset--;
								return *this;
							}
							internal_& operator*() {
								return (*this->container).internalVec[offset];
							}
							bool operator==(internalsIt& other) {
								return (other.offset==offset)&&(other.container==container);
							}
							bool operator!=(internalsIt& other) {
								return !((*this)==other);
							}
						private:
							size_t offset;
							binaryVector<internal_>* container;
					};
					internalsIt<internal> begin() {
						internalsIt<internal> temp(this);
						return temp;
					}
					internalsIt<internal> end() {
						internalsIt<internal> temp(this,this->internalVec.size());
						return temp;
					}
					//gets iternals
					std::vector<internal> internals() {
						return internalVec;
					}
					size_t size() {
						return this->bitCount;
					}
				private:
					void clipEndExtraBits() {
						auto totalBits=8*sizeof(internal)*internalVec.size();
						auto toClip=totalBits-this->size();
						//makes a internal full of ones then shifts it right to make a mask
						internalVec.back()&=(~(internal)0)>>(sizeof(internal)*8-toClip);
					}
					size_t bitCount;
					std::vector<internal> internalVec;
			};
			template<class internal_> class binaryVectorView:public std::iterator<std::output_iterator_tag, internal_> {
				public:
					//constructor
					binaryVectorView(binaryVector<internal_>* parent_=nullptr,size_t offset_=0): parent(parent_), baseOffset(offset_), iterOffset(0) {}
					internal_ getChunk(size_t offset_) {
						auto offset=baseOffset+offset_;
						auto Xinternals=offset/(8*sizeof(internal_));
						auto remainder=offset%(8*sizeof(internal_));
						internal_ firstHalf=0;
						internal_ lastHalf=0;
						//get first half from previous
						if(Xinternals-1>=0&&Xinternals-1<parent->internals().size()) {
							firstHalf=(parent->internals()[Xinternals-1])>>(8*sizeof(internal_)-remainder);
						}
						//
						if(Xinternals>=0&&Xinternals<parent->internals().size()) {
							lastHalf=(parent->internals())&Xinternals<<(remainder);
						}
						return firstHalf|lastHalf;
					}
					void assignChunk(internal_ value,size_t offset_) {
						auto offset=baseOffset+offset_;
						auto Xinternals=offset/(8*sizeof(internal_));
						auto remainder=offset%(8*sizeof(internal_));
						auto& internals=parent->internals();
						if(Xinternals>=0&&Xinternals<internals.size()) {
							internals[Xinternals]=value<<(remainder);
						}
						if(Xinternals+1>=0&&Xinternals+1<internals.size()) {
							internals[Xinternals+1]=value<<(sizeof(internals)*8-remainder);
						}
					}
					//iterator stuff;
					internal_ operator* () {
						return this->getChunk();
					}
					binaryVectorView<internal_>& operator++() {
						iterOffset+=sizeof(internal_);
						return *this;
					}
					binaryVectorView<internal_>& operator--() {
						iterOffset-=sizeof(internal_);
						return *this;
					}
					bool operator==(binaryVectorView<internal_>& other) {
						return parent==other->parent&&(baseOffset+iterOffset)==(other->baseOffset+other->iterOffset);
					}
					bool operator!=(binaryVectorView<internal_>&other) {
						return !(*this==other);
					}
				private:
					binaryVector<internal_>* parent;
					size_t baseOffset;
					size_t iterOffset;
			};
	}
	
	namespace binaryGraph {
	}
