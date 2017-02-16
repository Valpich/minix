cmake_minimum_required(VERSION 3.6)
project(cs551)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lncurses -stdlib=libc++ -D DEBUG")

set(SOURCE_FILES
        Command.cpp
        Command.h
        CommandFinder.cpp
        CommandFinder.h
        CommandManager.cpp
        CommandManager.h
        FileManager.cpp
        FileManager.h
        History.cpp
        History.h
        Profile.cpp
        Profile.h
        Shell.cpp
        Shell.h
        Test.cpp
        Test.h
        Main.cpp
        Main.h)

add_executable(cs551 ${SOURCE_FILES})