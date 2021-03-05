#include <IL/il.h>
#include <stdlib.h>
#include <iostream>
struct PointStruct{
    float * points;
    unsigned int width;
    unsigned int height;
};
PointStruct* GetPoints(const char * file_name);