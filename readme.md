# binaryVector
This is a vector that is aimed at btiwise operations such as bit shifting and masking. 

## Addressing the bits

The binary vector `binaryVector::binaryVector <internalType, addressor>`works by storing an internal resizeable vector that internally uses  `internalType` to store the bits. The real magic is the addressors that can be used to address the bits.  So the addressors are 

### Addressor #1: The default addressor (binaryVector)

This addressor addresses bits. A whole block of bits can be loaded into the binaryVector at once with

#### binaryVector::writeType(internalIndex);

This directly modifies the internal vector insider the `binaryVector` class. So internalIndex is the index of the internalVector to write a value of type `internal` to,which is has a bit index of sizeof(internal)*8*internalIndex. (To write into an offset that is not aligned with the internal vector,use the binaryVectorView class)

#### binaryVector::readType(internalIndex)

This direc

(**MAKE SURE THAT `internalType` IS `unsigned` TO AVOID UNDESIRABLE ARITHMETIC SHIFTING**,as opposed to logical shifting)