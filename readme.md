# binaryVector
This is a vector that is aimed at btiwise operations such as bit shifting and masking. 

## Addressing the bits

The binary vector `binaryVector::binaryVector <internalType, addressor>`works by storing an internal resizeable vector that internally uses  `internalType` to store the bits. The real magic is the addressors that can be used to address the bits.  So the addressors are 

### Addressor #1: The default addressor (binaryVector)

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


(**MAKE SURE THAT `internalType` IS `unsigned` TO AVOID UNDESIRABLE ARITHMETIC SHIFTING**,as opposed to logical shifting)