cmake_minimum_required(VERSION 3.17)
project(Algorithm)

set(CMAKE_CXX_STANDARD 17)

add_executable(Algorithm main.cpp LanguageChecker/CFGrammar.cpp LanguageChecker/CFGrammar.h LanguageChecker/LanguageChecker.cpp LanguageChecker/LanguageChecker.h LanguageChecker/Characters.h LanguageChecker/Characters.cpp)