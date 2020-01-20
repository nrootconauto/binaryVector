![logo](https://raw.githubusercontent.com/nrootconauto/binaryVector/master/logoSmaller.png)
# binaryVector

This is a vector that is aimed at btiwise operations such as bit shifting and masking. 

## QuickStart
[See Here](#quick-start)

## Addressing the bits

The binary vector `binaryVector::binaryVector <internalType>`works by storing an internal resizeable vector that internally uses  `internalType` to store the bits. The real magic is the addressors that can be used to address the bits.  So the addressors are 


### Addressing Mode #1: The default addressor (binaryVector)

This addressor addresses bits. A whole block of bits can be loaded into the binaryVector at once with

#### binaryVector::writeType(value,internalIndex);

This directly modifies the internal vector insider the `binaryVector` class. So internal Index is the index of the internalVector to write a value of type `internal` to,which is has a bit index of `sizeof(internal)*8*internalIndex`. (To write into an offset that is not aligned with the internal vector,use the binaryVectorView class)

#### binaryVector::readType(internalIndex)

This reads a value that is of the type `internal`. The value read is the `internalIndex`'th element in the internal vector of the `binaryVector`

#### binaryVector::_writeType(value, internalIndex)

Same as `binaryVector::binaryVector::writeType(value, internalIndex)` but doesnt perform a boundary check

#### binaryVector::_readType(internalIndex)

Same as `binaryVector::binaryVector::readType( internalIndex)` but doesnt perform a boundary check

#### binaryVector::blockStart()

This returns the first addresable internal element in the internal vector,always returns 0(Used in binaryVectorView to return the minimum addressable internal element *with* the virtual offset)

#### binaryVector::loadValue<T>(value) 

This loads a value into the binaryVector in its binary reprenation

#### binaryVector::blockSize()

Returns how many internal elements are addressable(Used in binaryVectorView to show how many of it's parent internal elments are visible though the binaryVectorView ) 

#### binaryVector::loadIntoPrimitive<type>(index)

Loads `sizeof(type)*8` bits into a return value starting from the item `index` of internal vector(which is bit `index*8*sizeof(internal)`)  

#### binaryVector::copy(other)

copies the contents on another binaryVector or binaryVector compatible class into the current binaryVector

#### binaryVector::binaryVector(sizeInBits) 

This makes an bitaryVector of `sizeinBits` bits

#### binaryVector::binaryVector(other)

clones antoher binaryVector into the current binaryVector

#### binaryVector::binaryVector(std::initializer_list<T>)

Loads the bits from the initiralizer list,**USES internal TYPE OF THE VECTOR,UNLESS YOUR INTENRAL TYPE IS unsigned char,IT WILL LOAD THE VALUES AS TPYE internal**

#### binaryVector::operator *X*

The binaryVector has these bitwise operators
- operator &=
- operator |=
- operator ^=
- operator <<=
- operator >>=
- operator &
- operator |
- operator ^
- TODO operator <<
- TODO operator >>

### Adressing Mode #2: The "window"(aka view) addressing mode (binaryVectorView)

This adressor is a "window" in the parent element. It writes and reads from bits from the starting bit and only processes a certian amount bits.(See the constructor for more info)

#### binaryVectorView::binaryVectorView(parent,offset,width)

This is the contructor,what it does is creates a window of the parent at offset `offset` and is `width` bits. Also, **a view can be a view of an other  `binaryVectorView`  by using : `binaryVectorView<T,decltype(otherView)>(otherView,offset,width)` **

#### binaryVector::X...

Inherients Methods from `binaryVector`

### Addressing mode #3: The nested binary View: (nestedBinaryView)

This is like a binaryVectorView but it is nested

#### nestedBinaryView::resize(width)

This resizes the width of the view,(which will be clipped to the end of the parent if goes past end)

#### nestedBinaryView::move(offset)

This changes the offset of the view(The offset will be clipped to the offset of the parent)

#### nestedBinaryView::X

 You can use the standard methods of `binaryVector`

## Quick Start

Quick usage
```cpp
//contructor
binaryVector::binaryVector<unsigned int> tenBits(10); //an empty bit vector of 10 bits 
binaryVector::binaryVector<unsigned int> valueOfTwelve({12}); //a vector with a value of twelve(THE 12 IS AN unsigend int,NOT A BYTE)
//bitwise operation ssignment
tenBits|=valueOfTwelve;
tenBits&=4;
tenBits^=5;
//shift operation
tenBits<<=3;
tenBits>>=1;
//assign operation
auto assigned=tenBits;
//bitwise operations
auto a=assigned&0xff;
auto x=assigned^0x1f;
auto o=assigned|0x01;
auto n=~assigned;
//load operation,load value starting at byte 0 into  an unsigned int
auto twelve=valueOfTwelve.template loadIntoPrimitive<unsigned int>(0);
//store operation ,sort value of unsigned int into byte 0
tenBits.template loadValue<unsigned int>(0x4ff);
//print
std::cout<<tenBits<<std::endl;
//View,a view is a windows that acts on only a certian region of bits
//Makes a "window" that operates and reads from bit 2 to 9(the window is 7 bits wide)
binaryVector::binaryVectorView<unsigned int> view(tenBits,2,7);
//addresses bits 3 through 9(width is not defined in consturtor so defualts entire view)
binaryVector::nestedBinaryView<unsigned int, decltype(view)>nested1 (view,1);
//nested view of nested view
binaryVector::nestedBinaryView<unsigned int, decltype(nested1)>nested2 (nested1);
```

## WARNINGS

- **MAKE SURE THAT `internalType` IS `unsigned` TO AVOID UNDESIRABLE ARITHMETIC SHIFTING,as opposed to logical shifting**
- **DONT NOT USE `unsigned long` AS THE INTENRAL OF A `binaryVector` AS ARITHMETIC SHIFTING WILL OCCUR IF THIS HAPPENS**

## Licensing

**DO WHATEVER YOU WANT WITH THIS,I DONT CARE**,(also if posibile,report any bugs you find in this library)
