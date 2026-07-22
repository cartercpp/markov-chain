cmake_minimum_required(VERSION 4.3)
project(MarkovChain)

set(CMAKE_CXX_STANDARD 23)

add_executable(MarkovChain main.cpp
        markov_chain.h)
