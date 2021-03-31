cmake_minimum_required(VERSION 3.17)
project(Algorithm)

set(CMAKE_CXX_STANDARD 17)

add_executable(Algorithm tests.cpp CFGrammar.cpp CFGrammar.h LanguageChecker.cpp LanguageChecker.h Characters.h Characters.cpp)
