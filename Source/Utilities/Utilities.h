#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "axmol.h"

class Utilities
{
public:
    static void problemLoading(const char* filename)
    {
        printf("Error while loading: %s\n", filename);
        printf(
            "Depending on how you compiled you might have to add 'Content/' in front of filenames");
    }
};



#endif  // __UTILITIES_H__