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
3. Run **git submodule init** (in the tron folder)
4. Run **git submodule update** (in the tron folder)
5. mkdir build
6. cd build
7. Call **cmake..**
8. Call **make**

To build the unit tests call **cmake .. -DBUILD_TESTS=ON**.
To build for CLion use **cmake .. -DCLION=TRUE**.

### Windows 7/8 ###

1. Clone the repository (git clone https://github.com/numinousgames/tron.git)
2. Navigate to the tron folder (the project root)
3. Run **git submodule init** (in the tron folder)
4. Run **git submodule update** (in the tron folder)
5. Create a new folder named build
6. Open CMake GUI
7. Set the source folder to the tron folder (the project root)
8. Set the build folder to the torn/build folder you created in step 3
9. Press configure
10. Select Visual Studio 14 2015 (leave default compilers selected)
11. Click finish (see common errors below on failure)
12. Press generate
13. Open Visual Studio
14. Select File > Open > Solution
15. Navigate to the tron/build folder
16. Select the generated tron.sln file
17. Run build all

To build the tests do the following:
1. Complete steps 1-11 above
2. Click add entry
3. Enter BUILD_TESTS, select type BOOL, and check value
4. Check the BUILD_SHARED_LIBS option
5. Press generate
6. Open Visual Studio
7. Select File > Open > Solution
8. Navigate to the tron/build folder
9. Select the generated tron.sln File
10. Build the project gtest_main
11. Navigate to the tron/build/ext/googletest/googletest/Debug
12. Copy gtest.dll and gtest_main.dll to tron/build/Debug
13. Build the project all_tests

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
[Travis Badge]: https://travis-ci.org/numinousgames/tron.svg
[Travis Link]: https://travis-ci.org/numinousgames/tron