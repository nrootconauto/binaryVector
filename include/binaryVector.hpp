#include "../include/binaryGraph.hpp" 
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
	namespace binaryVector {
			//nibble table
			//nibble lookup table
			const std::vector<const char*> nibbleTable {
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
			//addressor
			template<typename type> class addressor {
				public:
					type readType(size_t index) {
						if(index<0)
							return 0;
						if(index>=this->container.size())
							return 0;
						return container[index];
					}
					void writeType(size_t index,type value) {
						if(index<0)
							return;
						if(index>=this->container.size())
							return;
						container[index]=value;
					}
					size_t size() {
						return container.size();
					}
					void resize(size_t size) {
						this->container.resize(size);
					}
					addressor& baseContent() {
						return *this;
					}
				private:
					std::vector<type> container;
			};
			//binary Vector
			template<typename internal=size_t,class internalVector=addressor<internal>> class binaryVector {
				public:
					//access operator
					internal read(size_t index) {
						return this->internalVec.readType(index);
					}
					void write(size_t index,size_t value) {
						return this->internalVec.writeType(index,value);
					}
					//rezie for bits
					void resize(size_t bits) {
						//see if enough room in this for other at offset
						auto capacity=sizeof(internal)*this->internalVec.size()*8;
						if(capacity<bits) {
							//figure out how much to push back,frst statement handles the raminder
							auto toPushBack=((bits-capacity)%(8*sizeof(internal)))?1:0;
							toPushBack+=(bits-capacity)/(8*sizeof(internal));
							this->internalVec.resize(this->internalVec.size()+toPushBack);
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
							this->internalVec.writeType(currentBit/(8*sizeof(internal)),piece<<perInternalOffset);
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
					binaryVector(size_t sizeInBits) {
						this->resize(sizeInBits);
					}
					binaryVector() {}
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
					binaryVector& operator <<=(int bits) {
						internal carryRegister;
						internal carryOver=0;
						auto Xinternals=bits/(sizeof(internal)*8);
						//end bit is pushed past end,clear binaryVector
						if(Xinternals>internalVec.size()) {
							for(auto i=0;i!=internalVec.size();i++)
								internalVec.writeType(i,0);
							return *this;
						} else {
							//amount of remaining bits  after shift
							auto remainder=bits%(sizeof(internal)*8);
							for(auto I=internalVec.size()-1;true;I--) {
								auto& i=I;
								//assume 0 if carring over before first bit(Xinternals-1 must not (when subtracted) be before 0)
								if(i<Xinternals+1)
									carryOver=0;
								else
									carryOver=internalVec.readType(i-Xinternals-1)>>(sizeof(internal)*8-remainder);
								//put in register
								carryRegister=(internalVec.readType(i-Xinternals)<<remainder)|carryOver;
								//move left X internals
								internalVec.writeType(i,carryRegister);
								//BREAK IF GOING TO GO BEFORE FIRST THING
								if(I==0)
									break;
							}
							//fill rest with 0s
							for(auto I=0;I<Xinternals;I++) {
								internalVec.writeType(I,0);
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
								leftOverBits=internalVec.readType(i+Xinternals+1)<<(sizeof(internal)*8-remainder);
							} else
								leftOverBits=0;
							//apply shift and clip
							if(i+Xinternals<internalVec.size())
								internalVec.writeType(i,(internalVec.readType(i+Xinternals)>>remainder)|leftOverBits);
							else
								internalVec.writeType(i,0);
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
							internal_ operator*() {
								return this->container->internals().readType(this->offset);
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
					internalVector& internals() {
						return internalVec;
					}
					size_t size() {
						return this->bitCount;
					}
				private:
					void clipEndExtraBits() {
						auto& baseContent=this->internalVec.baseContent();
						auto totalBits=8*sizeof(internal)*baseContent.size();
						auto toClip=totalBits-this->size();
						//makes a internal full of ones then shifts it right to make a mask
						auto backIndex=internalVec.size()-1; //last elem
						baseContent.writeType(backIndex,baseContent.readType(backIndex)&(~(internal)0)>>(sizeof(internal)*8-toClip));
					}
				private:
					size_t bitCount;
				protected:
					internalVector internalVec;
			};
			template<class internal_,class vectorType=addressor<internal_>> class viewAddressor:public std::iterator<std::output_iterator_tag, internal_> {
				public:
					//constructor
					viewAddressor(binaryVector<internal_>* parent_=nullptr,size_t offset_=0): parent(parent_), baseOffset(offset_), iterOffset(0) {}
					internal_ readType(size_t offset_) {
						auto timesEight=offset_*8*sizeof(internal_);
						//make sure not addressing a negatice value(and see if there is room for a reaiminder)
						if(baseOffset>8*sizeof(internal_)+timesEight)
							return 0;
						auto remainder=(baseOffset)%(8*sizeof(internal_));
						auto Xinternals=(baseOffset>timesEight)?0:(timesEight+baseOffset)/(8*sizeof(internal_));
						//if baseOffset goes past negative,assume 0 IF there is a reminder 
						internal_ firstHalf=0;
						internal_ lastHalf=0;
						//===get first half from previous
						//(Xinternals must be above 0 as it checks the previous item)
						if(Xinternals+1<parent->internals().size()) {
							firstHalf=(parent->internals().readType(Xinternals+1))<<(8*sizeof(internal_)-remainder);
						}
						//=== second half 
						if(Xinternals<parent->internals().size()) {
							lastHalf=(parent->internals().readType(Xinternals))>>remainder;
						}
						return firstHalf|lastHalf;
					}
					void writeType(size_t offset_,internal_ value) {
						//do nothign if no parent
						if(parent==nullptr)
							return;
						//
						auto offset=baseOffset+offset_*sizeof(internal_)*8;
						auto Xinternals=offset/(8*sizeof(internal_));
						auto remainder=offset%(8*sizeof(internal_));
						auto& internals=parent->internals();
						if(Xinternals>=0&&Xinternals<internals.size()) {
							internals.writeType(Xinternals,value<<(remainder));
						}
						if(Xinternals+1>=0&&Xinternals+1<internals.size()) {
							internals.writeType(Xinternals+1,value>>(sizeof(internal_)*8-remainder));
						}
					}
					//iterator stuff;
					internal_ operator* () {
						return this->readType(0);
					}
					viewAddressor& operator++() {
						iterOffset+=8*sizeof(internal_);
						return *this;
					}
					viewAddressor& operator--() {
						iterOffset-=8*sizeof(internal_);
						return *this;
					}
					bool operator==(viewAddressor& other) {
						return parent==other->parent&&(baseOffset+iterOffset)==(other->baseOffset+other->iterOffset);
					}
					bool operator!=(viewAddressor&other) {
						return !(*this==other);
					}
					//
					friend std::ostream& operator<<(std::ostream& out,viewAddressor& view) {
						//span in internals to read from
						const size_t sizeInBits=view.parent->size();
						size_t Xinternals=(sizeInBits-view.iterOffset-view.baseOffset)/(8*sizeof(internal_));
						size_t remainder=(sizeInBits-view.iterOffset-view.baseOffset)%(8*sizeof(internal_));
						//if there are remaining bits
						Xinternals+=remainder?1:0;
						std::string str;
						//got through the internals
						for(size_t i=0;i!=Xinternals*8*sizeof(internal_);i+=sizeof(internal_)*8)
							for(size_t b=0;b!=(8*sizeof(internal_))/4;b++)
								str=nibbleTable[0x0f&(view.readType(i)>>4*b)]+str;
						//stream it
						out<<str;
						return out;
					};
					size_t size() {
						//return 0 if no parent
						if(this->parent==nullptr)
							return 0;
						//add one size if there is a remaidner
						auto addOne=((this->parent->size()-iterOffset-baseOffset)%8)?1:0;
						return (this->parent->size()-iterOffset-baseOffset)/8+addOne;
					}
					//dummy
					void resize(size_t size) {
					}
					vectorType& baseContent() {
						return parent->internals();
					}
				protected:
					binaryVector<internal_,vectorType>* parent;
				private:
					size_t baseOffset;
					size_t iterOffset;
			};
			//
			template<typename internal,typename vectorType=addressor<internal>> class binaryVectorView:public binaryVector<internal,viewAddressor<internal>> {
				public:
					binaryVectorView(binaryVector<internal,vectorType>& parent,int offset=0) {
						this->internals()=viewAddressor<internal>(&parent,offset);
					}
					binaryVectorView();
					binaryVectorView(binaryVectorView& other) =delete;
					binaryVectorView(size_t sizeInBits)= delete;
					binaryVectorView(internal* items,size_t count=-1) = delete;
			};
	}
	//stream stuff;
	template<typename internal,class vectorType> std::ostream& operator<<(std::ostream& out,binaryVector::binaryVector<internal,vectorType>& thing) {
		std::string str;
		//got through the internals
		for(size_t Xinternal=0;Xinternal!=thing.internals().size();Xinternal++)
			for(size_t b=0;b!=(8*sizeof(internal))/4;b++)
				str=binaryVector::nibbleTable[0x0f&(thing.read(Xinternal)>>4*b)]+str;
		//stream it
		out<<str;
		return out;
	}
	namespace binaryGraph {
	}
