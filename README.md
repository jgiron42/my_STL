Implementation of the containers from the c++98 standard template library for a 42 project. unordered associatives containers are adapted to c++98. there is also an extra container `xorlist` of my own creation (implementing a [xor linked list](https://en.wikipedia.org/wiki/XOR_linked_list)).

The rules of this project is to not use anything from the standard library except: 
- std::allocator
- std::string
- std::numeric_limits
- iterator_tags

This repo also include a fuzzer for the containers
