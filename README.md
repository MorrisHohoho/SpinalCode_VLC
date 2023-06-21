# SpinalCode
## Python
Python 3 version of the original spinal codes.
## C
C99 version of the spinal codes. (Or any other C version that supports VLA).
## Notes
k,B,d are parameters that influenced the time consumption drastcially.
When k,B,d are too large, the decoding complexity is too enough to be used in practical.
So in this C version, I choose k=4, B=4, d=2 which is as same as the original python version.

## BUGs
1. k or B > 4, the program will crash.
2. d >2, the program will also crash.
Perhaps the stack is overflow?