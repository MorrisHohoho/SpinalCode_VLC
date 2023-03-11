# SpinalCode_C
SpinalCode implementation in C.
# Warning
**Memory Leak!!!!!**
# How to use
1. ```mkdir build```
2. ```cd build```
3. ```cmake ..```
4. ```make```


# Note
1. c/k is bigger the probability of symbol collision is lower.

2. If change k, the definition ```CHILD_NUMS``` in ```multinodetree.h``` should been changed too.

3. The implementation of multinodetree is to use ```malloc```, but the memory haven't been ```free``` when the ```SpinalDecode``` function is over.
