# Pintos
## _Introduction_
Pintos는 80x86 architecutre을 위한 간단한 operating system framework이다. pintos는 kernel threads, loading, user program, file system이 매우 간단한 방법으로 구현돼있다. 이 프로젝트에서는, 해당 부분을 strengthen해야 한다. 

## _About?_
_‘threads/’_
Source code for the base kernel, which you will modify starting in project 1.

_‘userprog/’_
Source code for the user program loader, which you will modify starting with project 2.

_‘vm/’_ 
An almost empty directory. You will implement virtual memory here in project 3.

_‘filesys/’_
Source code for a basic file system. You will use this file system starting with
project 2, but you will not modify it until project 4.

_‘devices/’_
Source code for I/O device interfacing: keyboard, timer, disk, etc. You will modify the timer implementation in project 1. Otherwise you should have no need to change this code.

_‘lib/’_
An implementation of a subset of the standard C library. The code in this directory is compiled into both the Pintos kernel and, starting from project
2, user programs that run under it. In both kernel code and user programs,
headers in this directory can be included using the #include <...> notation.
You should have little need to modify this code.

_‘lib/kernel/’_
Parts of the C library that are included only in the Pintos kernel. This also includes implementations of some data types that you are free to use in your
kernel code: bitmaps, doubly linked lists, and hash tables. In the kernel, headers
in this directory can be included using the #include <...> notation.

_‘lib/user/’_
Parts of the C library that are included only in Pintos user programs. In user
programs, headers in this directory can be included using the #include <...>
notation.

_‘examples/’_
Example user programs for use starting with project 2.

_‘misc/’_
_‘utils/’_ These files may come in handy if you decide to try working with Pintos on your own machine. Otherwise, you can ignore them.


[Reference]: https://web.stanford.edu/class/cs140/projects/pintos/pintos.pdf

## _Progress Result_
Project1, 2 구현 완료
