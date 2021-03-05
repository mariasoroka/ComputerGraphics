#include "surface_from_image.h"
PointStruct* GetPoints(const char * file_name){
    ILuint ImageName;
    ilGenImages(1, &ImageName);
    ilBindImage(ImageName);
    ilLoad(IL_PNG, file_name);
    PointStruct* result = (PointStruct*)malloc(sizeof(PointStruct));
    result->width = ilGetInteger(IL_IMAGE_WIDTH);
    result->height = ilGetInteger(IL_IMAGE_HEIGHT);
    result->points = (float *)malloc(sizeof(float) * result->height * result->width * 3);
    ILfloat * pixel = (ILfloat *)malloc(ilGetInteger(IL_IMAGE_BPP)*ilGetInteger(IL_IMAGE_BPC));
    for(int i = result->height - 1; i >= 0; i--){
        for(int j = 0; j < result->width; j++){
            ilCopyPixels(j, i, 0, 1, 1, 1, IL_RGB,
                         IL_FLOAT, pixel);
            result->points[((result->height - 1 - i)*result->width + j) * 3] = float(result->height - 1 - i) / result->height;
            result->points[((result->height - 1 - i)*result->width + j) * 3 + 1] = float(j) / result->width;
            result->points[((result->height - 1 - i)*result->width + j) * 3 + 2] = float(*pixel);
        }
    }
    free(pixel);
    ilDeleteImage(1);
    return result;
}