#pragma once
#include <string>
#include <iostream>

inline void Error(std::string message)
{
    std::cerr << std::endl << "ERROR: " << message << std::endl;
}

inline void Warning(std::string message)
{
    std::cout << std::endl << "WARNING: " << message << std::endl;
}
