# binaryVector

This is a vector that is aimed at btiwise operations such as bit shifting and masking. 

## Addressing the bits

The binary vector `binaryVector::binaryVector <internalType, addressor>`works by storing an internal resizeable vector that internally uses  `internalType` to store the bits. The real magic is the addressors that can be used to address the bits.  So the addressors are 

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

### outside Functions

These are functions  that can be used with `binaryVector`

#### virtualShift(vector,offset)

This function applys an offset to the value of the window when reading in a  `binaryVectorView`. This can be used to emulate reading a shifted value without any modification to the view's data. 

## WARNINGS

- **MAKE SURE THAT `internalType` IS `unsigned` TO AVOID UNDESIRABLE ARITHMETIC SHIFTING,as opposed to logical shifting**
- **DONT NOT USE `unsigned long` AS THE INTENRAL OF A `binaryVector` AS ARITHMETIC SHIFTING WILL OCCUR IF THIS HAPPENS**

## Licensing

**DO WHATEVER YOU WANT WITH THIS,I DONT CARE**,(also if posibile,report any bugs you find in this library)
