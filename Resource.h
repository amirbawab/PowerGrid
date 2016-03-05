#pragma once
#include "Helpers.h"

enum Resource {
	COAL,
	OIL,
	GARBAGE,
	URANIUM
};

namespace res {
	const int total = 4;
}

inline Resource GetResourceByName(std::string name)
{
    // By default, it's 'COAL'!
    Resource resource = COAL;

    if (ToLower(name) == "coal")
        resource = COAL;
    else if (ToLower(name) == "oil")
        resource = OIL;
    else if (ToLower(name) == "garbage")
        resource = GARBAGE;
    else if (ToLower(name) == "uranium")
        resource = URANIUM;

    return resource;
}

inline string GetResourceName(Resource resource)
{
    switch (resource)
    {
        case COAL:
            return "coal";
        case OIL:
            return "oil";
        case GARBAGE:
            return "garbage";
        case URANIUM:
            return "uranium";
        default:
            return "UNKNOWN";
    }
}