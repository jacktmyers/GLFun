#!/bin/bash

if [ "$1" = clean ] || [ "$1" = cmake ]; then
	rm -rf CMakeCache.txt
	rm -rf cmake_install.cmake
	rm -rf Makefile
	rm -rf .cmake
	rm -rf CMakeFiles
	rm -rf libCFILES_LIB.a
	rm -rf libPRIMATIVES_LIB.a
fi

if [ "$1" = cmake ]; then
	cmake CMakeLists.txt
fi

if [ "$1" = clean ] || [ "$1" = run ]; then
	rm -rf Modern
fi

if [ "$1" = run ]; then
	make &&
	./Modern
fi

