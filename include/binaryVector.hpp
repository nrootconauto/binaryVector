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
					//DOES NOT BOUND CHECK
					type _readType(signed long index) {
						return container[index];
					}
					//DOES NOT BONUD CHECK
					void  _writeType(signed long index,type value) {
						container[index]=value;
					}
					type readType(signed long index) {
						if(index<0)
							return 0;
						if(index>=this->container.size())
							return 0;
						return container[index];
					}
					void writeType(signed long index,type value) {
						if(index<0)
							return;
						if(index>=this->container.size())
							return;
						container[index]=value;
					}
					signed long size() {
						return container.size();
					}
					void resize(size_t size) {
						this->container.resize(size);
					}
					signed long firstXinternalInParent() {
						return 0;
					}
					addressor& baseContent() {
						return *this;
					}
					template<class binVec> binVec& getParent() {
						return *(binVec*)this->_parent;
					}
					addressor(void* vec) {
						this->_parent=vec;
					};
				private:
					void* _parent;
					std::vector<type> container;
			};
		//binary Vector
			template<typename internal=size_t, class internalVector=addressor<internal>> class binaryVector {
				public:
					template<typename T> void loadValue(T& value) {
						internal temp=0;
						unsigned long index=0;
						this->resize(sizeof(value)*8);
						for(unsigned int  byte=0;byte<sizeof(value);byte+=sizeof(internal)) {
							this->internalVec._writeType(index++,value>>(8*byte));
						}
						//clear the unused bytes
						for(;index!=this->internalVec.size();index++)
							this->internalVec._writeType(index,0);
					}
					//write into a primitive
					template<typename toLoadInto> toLoadInto loadIntoPrimitive(signed long index) {
						toLoadInto retVal=0;
						internal temp;
						long currentByte=0;
						//per byte
						for(signed long i=0;i<sizeof(retVal);i++) {
							temp=this->readBlock(index++);
							retVal|=temp<<(8*currentByte);
							currentByte+=sizeof(internal);
						}
						return retVal;
					}
					//binary operators
					binaryVector<internal,addressor<internal>> operator ~() {
						//make a blank new binaryVector
						auto temp=binaryVector<internal,addressor<internal>>(this->size());
						//copy over
						for(int i=0;i!=this->internals().size();i++)
							temp.internals()._writeType(i,~this->internalVec._readType(i));
						//
						temp.clipEndExtraBits();
						return temp;
					}
					binaryVector<internal,addressor<internal>> operator&(binaryVector& other) {
						auto temp=binaryVector<internal,addressor<internal>>(this->size());
						//copy over and apply & operator
						auto [start,end]=this->getAffectedRange(other);
						for(auto i=start;i!=end;i++)
							temp.internals()._writeType(i,this->internalVec._readType(i)&other.internals()._readType(i));
						return temp;
					}
					binaryVector<internal,addressor<internal>> operator|(binaryVector& other) {
						auto temp=binaryVector<internal,addressor<internal>>(this->size());
						temp.copy(*this);
						auto [start,end]=this->getAffectedRange(other);
						for(auto i=start;i!=end;i++)
							temp.internals()._writeType(i,this->internalVec._readType(i)|other.internals()._readType(i));
						return temp;
					}
					binaryVector<internal,addressor<internal>> operator^ (binaryVector& other) {
						auto temp=binaryVector<internal,addressor<internal>>(this->size());
						temp.copy(*this);
						auto [start,end]=this->getAffectedRange(other);
						for(auto i=start;i!=end;i++)
							temp.internals()._writeType(i,this->internalVec._readType(i)^other.internals()._readType(i));
						return temp;
					}
				private:
					//get affected range
					std::pair<signed long,signed long> getAffectedRange(binaryVector& other) {
						auto thisSize=this->internals().size();
						auto otherSize=other.internals().size();
						//choose the lower part
						auto minSize=(thisSize<otherSize)?thisSize:otherSize;
						//choose the maximum base offset
						auto thisOffset=this->internalVec.firstXinternalInParent();
						auto otherOffset=other.internalVec.firstXinternalInParent();
						auto baseOffset=(thisOffset>otherOffset)?thisOffset:otherOffset;
						return std::pair<signed long,signed long>(baseOffset,minSize);
					}
				public:
					//or
					//binary equality operators
					template<class vector> binaryVector& operator |=(binaryVector<internal,vector>& other) {
						auto [baseOffset,minSize]=this->getAffectedRange(other);
						//go though and or
						for(auto i=baseOffset;i!=minSize;i++) {
							this->internalVec._writeType(i,this->internalVec._readType(i)|other->internals()._readType(i));
						}
						this->clipEndExtraBits();
						return *this;
					}
					//xor !!!
					template<class vector> binaryVector& operator ^=(binaryVector<internal,vector>& other) {
						auto [baseOffset,minSize]=this->getAffectedRange(other);
						//
						for(auto i=baseOffset;i!=minSize;i++) {
							this->internalVec._writeType(i,this->internalVec._readType(i)^other->internals()._readType(i));
						}
						//
						this->clipEndExtraBits();
						return *this;
					}
					//
					template<class vector> binaryVector& operator &=(binaryVector<internal,vector>& other) {
						auto [baseOffset,minSize]=this->getAffectedRange(other);
						//erase the zeros before baseOffset
						for(auto i=baseOffset-1;i--;)
							this->internalVec._writeType(i,0);
						//do the ands
						for(auto i=baseOffset;i!=minSize;i++) {
							this->internalVec._writeType(i,this->internalVec._readType(i)|other->readBlock(i));
						}
						//zeroify this past the minSize
						for(auto i=minSize;i!=this->internalVec.size();i++)
							this->internalVec._writeType(i,0);
						this->clipEndExtraBits();
						return *this;
					}
					//access operator
					internal readBlock(size_t index) {
						return this->internalVec.readType(index);
					}
					void writeBlock(size_t index,size_t value) {
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
						if(sizeof(otherInternal)==sizeof(internal)) {
							for(int i=0;i!=other.internals().size();i++)
								this->internalVec._writeType(i,other.readBlock(i));
							return;
						}
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
					binaryVector(size_t sizeInBits):internalVec((void*)this) {
						this->resize(sizeInBits);
					}
					binaryVector():internalVec((void*)this) {}
					//constructor
					binaryVector(internal* items,size_t count=-1):internalVec((void*)this) {
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
					template<class otherInteral> binaryVector(binaryVector<otherInteral>& other):internalVec((void*)this) {
						this->copy(other);
					}
					//
					binaryVector& operator <<=(signed long bits) {
						internal carryRegister;
						internal carryOver;
						signed long Xinternals=bits/(sizeof(internal)*8);
						//end bit is pushed past end,clear binaryVector
						if(Xinternals>internalVec.size()) {
							for(auto i=internalVec.size()-1;i--;)
								internalVec._writeType(i,0);
							return *this;
						} else {
							//amount of remaining bits  after shift
							signed long remainder=bits%(sizeof(internal)*8);
							//find the last internal(towards 0) that will affect the binaryVector
							signed long lastTowardsZero=(signed long)Xinternals;
							//find the last internal(towards end) that will be affected;
							signed long lastTowardsEnd;
							if(Xinternals<internalVec.size())
								lastTowardsEnd=internalVec.size()-1;
							else
								lastTowardsEnd=0;
							//OPTIMIZATION:boundedCheck(for binaryVectorViews) will be used on "last" element to ensure doesnt write past last bit
							auto perXinternal=[&](signed long i,void (internalVector::*fp)(signed long,internal))->void {
								//assume 0 if carring over before first bit(Xinternals-1 must not (when subtracted) be before 0)
								if(i-Xinternals-1<0)
									carryOver=0;
								else
									carryOver=internalVec._readType(i-Xinternals-1)>>(sizeof(internal)*8-remainder);
								//put in register
								carryRegister=(internalVec._readType(i-Xinternals)<<remainder)|carryOver;
								//move left X internals
								(internalVec.*fp)(i,carryRegister);
							};
							//wipe those past the affected
							for(signed long I=internalVec.size()-1;I>lastTowardsEnd+1;I--)
								internalVec._writeType(I,0);
							//do the last element with a boundarycheck
							if(internalVec.size()-1>=lastTowardsEnd)
								perXinternal(lastTowardsEnd,&internalVector::writeType);
							//the actual loop with no boundary checks
							for(signed long I=lastTowardsEnd-1;I>=lastTowardsZero;I--)
								perXinternal(I,&internalVector::_writeType);
							//fill rest with 0s
							for(signed long I=0;I<Xinternals;I++)
								internalVec._writeType(I,0);
						}
						this->clipEndExtraBits();
						return *this;
					}
					binaryVector& operator>>= (signed long bits) {
						//how many internals the shift spans
						signed long Xinternals=bits/(sizeof(internal)*8);
						signed long remainder=bits%(sizeof(internal)*8);
						internal leftOverBits=0;
						//go foward in for statement (do all internals (except the last) without a boundary check)
						signed long i=0;
						for(;i<=internalVec.size()-2-Xinternals;i++) {
							signed long index=i+Xinternals;
							if(index+1<internalVec.size()) {
								//shift to get remianing bits,then shift to begining of next internal
								leftOverBits=internalVec._readType(index+1)<<sizeof(internal)*8-remainder;
							} else
								leftOverBits=0;
							//apply shift and clip
							internal value=internalVec._readType(index)>>remainder;
							internalVec._writeType(i,value|leftOverBits);
						}
						//go the last inernal with bounds checking
						if(internalVec.size()-1==i+Xinternals) {
							internalVec.writeType(i,internalVec.readType(i+Xinternals)>>remainder);
							//go past the write
							i++;
						}
						//wipe the rest of the internals with zeros
						if(i<internalVec.size()) {
							for(signed long I=internalVec.size()-2;I>=i;I--)
								internalVec._writeType(I,0);
							//write last with boundary check
							internalVec.writeType(internalVec.size()-1,0);
						}
						//clip
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
					signed long size() {
						return this->bitCount;
					}
					void clipEndExtraBits() {
						auto& master=this->internals().template getParent<binaryVector<internal,addressor<internal>>>();
						auto& baseContent=master.internals();
						auto totalBits=8*sizeof(internal)*baseContent.size();
						auto toClip=totalBits-master.size();
						//makes a internal full of ones then shifts it right to make a mask
						auto backIndex=internalVec.size()-1; //last elem
						const internal ones=~(internal)0;
						baseContent.writeType(backIndex,baseContent.readType(backIndex)&(ones>>toClip));
					}
				private:
					size_t bitCount;
				protected:
					internalVector internalVec;
			};
			template<class internal_,class vectorType=addressor<internal_>> class viewAddressor:public std::iterator<std::output_iterator_tag, internal_> {
					internal_ endMask(unsigned long Xinternal,signed long remainder,signed long widthRemainder) {
						const signed long sizeInBits=8*sizeof(internal_);
						//boundary is after the highest addreable Xinternal
						signed long boundary=this->firstXinternalInParent()+this->size();
						//trims the view to not see past the mask
						const internal_ ones=~0;
						internal_ mask;
						if(Xinternal+1==boundary)
							mask=ones>>(widthRemainder?sizeInBits-widthRemainder:0); //make sure if remainder doesnt pass into next byte if there is no reminder;
						else if(Xinternal>=boundary)
							return 0;
						else
							mask=ones;
						//if at first addressable bye,add maks
						if(Xinternal==baseOffset/sizeInBits)
							mask&=(ones<<remainder);
						return mask;
					}
				public:
					//constructor
					viewAddressor(void* parent_=nullptr,size_t offset_=0,signed long width_=-1): parent((binaryVector<internal_>*)parent_), baseOffset(offset_), viewSize(width_) {
					}
					template<class type> type& getParent() {
						return *(type*)this->parent;
					}
					internal_ _readType(signed long offset_) {
						return this->readType(offset_);
					}
					void _writeType(signed long offset_,internal_ value) {
						this->writeType(offset_,value);
					}
					internal_ readType(signed long offset_) {
						const signed long sizeInBits=8*sizeof(internal_);
						signed long timesEight=offset_*sizeInBits;
						//make sure not addressing a negatice value(and see if there is room for a reaiminder)
						if(baseOffset>2*sizeInBits+timesEight)
							return 0;
						signed long remainder=(baseOffset)%sizeInBits;
						signed long Xinternals=(timesEight+baseOffset)/(8*sizeof(internal_));
						//if baseOffset goes past negative,assume 0 IF there is a reminder 
						internal_ firstHalf=0;
						internal_ lastHalf=0;
						//raimder of the width
						signed long widthRemainder=(this->width()+this->baseOffset)%sizeInBits;
						//===function to make end mask
						//===get first half from previous
						//(Xinternals must be above 0 as it checks the previous item)
						if(Xinternals+1<parent->internals().size()) {
							firstHalf=(parent->internals().readType(Xinternals+1)&this->endMask(Xinternals+1, remainder, widthRemainder))<<8*sizeof(internal_)-remainder;
						}
						//=== second half 
						if(Xinternals<parent->internals().size()) {
							lastHalf=(parent->internals().readType(Xinternals)&this->endMask(Xinternals, remainder, widthRemainder ))>>remainder;
						}
						return firstHalf|lastHalf;
					}
					signed long firstXinternalInParent() {
						return (this->baseOffset)/(8*sizeof(internal_));
					}
					void writeType(signed long offset_,internal_ value) {
						//do nothign if no parent
						if(parent==nullptr)
							return;
						//find the last Xinternals that can be addressed with th e
						signed long maximumAdressableInternal;
						signed long lastBit=(this->baseOffset+this->width());
						//
						signed long offset=baseOffset+offset_*sizeof(internal_)*8;
						signed long Xinternals=offset/(8*sizeof(internal_));
						signed long remainder=offset%(8*sizeof(internal_));
						auto& internals=parent->internals();
						const internal_ ones=~(internal_)0;
						//remainder of the width in bits
						signed long widthRemainder=(this->width()+this->baseOffset)%(8*sizeof(internal_));
						//boundary is after maximum addresable boundary
						signed long boundary=this->firstXinternalInParent();
						//assume boundary is at end of parent if is -1
						if(this->size()==-1)
							boundary=internals.size();
						else
							boundary+=this->size();
						//function to apply "End" mask
						//
							if(Xinternals<internals.size()) {
								internal_ mask=endMask(Xinternals,remainder,widthRemainder);
								//apply mask to old vvalue
								internal_ leftOver=internals.readType(Xinternals);
								leftOver&=~(mask&ones<<remainder);
								internals.writeType(Xinternals,((value<<remainder)&mask)|leftOver);
							}
							if(Xinternals+1<internals.size()) {
								internal_ mask=endMask(Xinternals+1, remainder, widthRemainder);
								internal_ leftOver=internals.readType(Xinternals+1);
								leftOver&=~(mask&ones>>sizeof(internal_)*8-remainder); //CLEAR FIRST (SIZE-REMIANCDER)BYTES FOR WRITE(endMask chooses all of the bits THAT ARE ADRESSABLE BY internalVec,SO MAKE SURE TO STORE THOSE NOT AFFECT BY YHT WRITE OPERATION)
								internals.writeType(Xinternals+1, ((value>>sizeof(internal_)*8-remainder)&mask|leftOver));
							}
							//clip if writing on the last Internal
							if(Xinternals>=this->parent->internals().size()-1) {
								parent->clipEndExtraBits();
							}
						}
						signed long size() {
							const signed long typeWidth=8*sizeof(internal_);
							//return 0 if no parent
							if(this->parent==nullptr)
								return 0;
							//add one size if there is a remaidner
							signed long addOne=((this->parent->size()-baseOffset)%typeWidth)?1:0;
							//if no width is specified,use parents width
							if(this->width()==-1)
								return (this->parent->size()-baseOffset)/typeWidth+addOne;
							//
							return this->width()/typeWidth+addOne;
						}
						//dummy
						void resize(size_t size) {
						}
						vectorType& baseContent() {
							return parent->internals();
						}
						signed long  width() {
							if(this->viewSize==-1)
								this->viewSize=this->parent->size()-this->baseOffset;
							return this->viewSize;
						}
						binaryVector<internal_,vectorType>* parent;
					private:
						signed long viewSize;
						signed long baseOffset;
					};
					//
					template<typename internal,typename vectorType=addressor<internal>> class binaryVectorView:public binaryVector<internal,viewAddressor<internal>> {
						public:
							binaryVectorView(binaryVector<internal,vectorType>& parent,int offset=0,size_t width=-1) {
								this->internals()=viewAddressor<internal>(&parent,offset,width);
							}
							binaryVectorView();
							binaryVectorView(binaryVectorView& other) =delete;
							binaryVectorView(size_t sizeInBits)= delete;
							binaryVectorView(internal* items,size_t count=-1) = delete;		
					};
			}
	//stream stuff;
	template<typename internal,class vectorType> std::ostream& operator<<(std::ostream& out,binaryVector::binaryVector<internal,vectorType> thing) {
		std::string str;
		//got through the internals
		for(signed long Xinternal=0;Xinternal!=thing.internals().size();Xinternal++)
			for(signed long b=0;b!=(8*sizeof(internal))/4;b++) {
				str=binaryVector::nibbleTable[0x0f&(thing.readBlock(Xinternal)>>4*b)]+str;
			}
		//stream it
		out<<str;
		return out;
	}
	namespace binaryGraph {
	}
