#pragma once

#include <string>
#include <iostream>
#include <algorithm>

using std::string;
using std::transform;

inline void Error(string message)
{
    std::cerr << std::endl << "ERROR: " << message << std::endl;
}

inline void Warning(string message)
{
    std::cout << std::endl << "WARNING: " << message << std::endl;
}

inline string ToLower(string input)
{
    string result = input;
    transform(result.begin(), result.end(), result.begin(), tolower);
    return result;
}

inline string ToUpper(string input)
{
    string result = input;
    transform(result.begin(), result.end(), result.begin(), toupper);
    return result;
}

namespace PG {
	const int INVALID = -1;
}