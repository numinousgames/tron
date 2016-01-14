# Tron #

[![Build Status][Travis Badge]][Travis Link]

## Overview ##
This project is meant to be a replica of the arcade Tron bike game.

## Support ##
This project officially supports Windows 7 & 8 and Ubuntu 15.0+;

## Requirements ##

### Ubuntu 15.0+ ###

 * CMake 2.8.7+
 * Clang 3.6+ or GCC 5.2+
 * Git 2.5.0+
 
### Windows 7/8 ###

 * CMake 2.8.7+
 * Visual Studio 2015
 * Git 2.5.0+ (without unix tools or not on path)
 
Older versions of Visual Studio may work but are not 
confirmed.

## Build Steps ##

### Ubuntu 15.0+ ###

1. Clone the repository (git clone https://github.com/numinousgames/tron.git)
2. Navigate to the tron folder (the project root)
3. mkdir build
4. cd build
5. Call **cmake..**
6. Call **make**

To build the unit tests call **cmake .. -DBUILD_TESTS=ON**.
To build for CLion use **cmake .. -DCLION=TRUE**.

### Windows 7/8 ###

1. Clone the repository (git clone https://github.com/numinousgames/tron.git)
2. Navigate to the tron folder (the project root)
3. Create a new folder named build
4. Open CMake GUI
5. Set the source folder to the tron folder (the project root)
6. Set the build folder to the torn/build folder you created in step 3
7. Press configure
8. Select Visual Studio 14 2015 (leave default compilers selected)
9. Click finish (see common errors below on failure)
10. Press generate
11. Open Visual Studio
12. Select File > Open > Solution
13. Navigate to the tron/build folder
14. Select the generated tron.sln file
15. Run build all

The application can be run through visual studio by changing the startup 
project in the Solution settings to tron.

#### Common CMake Issues: ####
##### C/C++ Compiler is not found #####
If you have just installed Visual Studio 2015 it is very likely that you did
not install the actual C++ toolchain which is installed when you either make 
a C++ project for the first time or manually install the feature using the
installer. To remedy go to File > New > Solution. After the window opens
select the C++ tab which should provide you with a link that will install
the VC++ tool chain.

## See ##

[Standards Document][Standards]


[Standards]: http://bit.ly/1J4g51q
[Travis Badge]: https://travis-ci.org/numinousgames/tron.svg?branch=master
[Travis Link]: https://travis-ci.org/numinousgames/tron