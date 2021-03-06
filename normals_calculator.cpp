#include "normals_calculator.h"
glm::vec3 normal(int i, int j, unsigned int width, unsigned int height, float* vert){
    int point_number = (i * width + j);
    glm::vec3 res;
    if((i == 0  && j != width - 1) || (j == 0 && i != height - 1)){
        glm::vec3 center = glm::vec3(vert[point_number * 3], vert[point_number * 3 + 1], vert[point_number * 3 + 2]);
        glm::vec3 up = glm::vec3(vert[(point_number + width) * 3], vert[(point_number + width) * 3 + 1], vert[(point_number + width) * 3 + 2]);
        glm::vec3 right = glm::vec3(vert[(point_number + 1) * 3], vert[(point_number + 1) * 3 + 1], vert[(point_number + 1) * 3 + 2]);
        res = glm::normalize(glm::cross(up - center, right-center));
    }
    else if(j == width - 1 && i != height - 1){
        glm::vec3 center = glm::vec3(vert[point_number * 3], vert[point_number * 3 + 1], vert[point_number * 3 + 2]);
        glm::vec3 up = glm::vec3(vert[(point_number + width) * 3], vert[(point_number + width) * 3 + 1], vert[(point_number + width) * 3 + 2]);
        glm::vec3 left = glm::vec3(vert[(point_number - 1) * 3], vert[(point_number - 1) * 3 + 1], vert[(point_number - 1) * 3 + 2]);
        res = glm::normalize(glm::cross(left - center, up-center));
    }
    else if(i == height - 1 && j != width - 1){
        glm::vec3 center = glm::vec3(vert[point_number * 3], vert[point_number * 3 + 1], vert[point_number * 3 + 2]);
        glm::vec3 down = glm::vec3(vert[(point_number - width) * 3], vert[(point_number - width) * 3 + 1], vert[(point_number - width) * 3 + 2]);
        glm::vec3 right = glm::vec3(vert[(point_number + 1) * 3], vert[(point_number + 1) * 3 + 1], vert[(point_number + 1) * 3 + 2]);
        res = glm::normalize(glm::cross(right - center, down-center));
    }
    else if(i == height - 1 && j == width - 1){
        glm::vec3 center = glm::vec3(vert[point_number * 3], vert[point_number * 3 + 1], vert[point_number * 3 + 2]);
        glm::vec3 down = glm::vec3(vert[(point_number - width) * 3], vert[(point_number - width) * 3 + 1], vert[(point_number - width) * 3 + 2]);
        glm::vec3 left = glm::vec3(vert[(point_number - 1) * 3], vert[(point_number - 1) * 3 + 1], vert[(point_number - 1) * 3 + 2]);
        res = glm::normalize(glm::cross(down - center, left-center));
    }
    else{
        glm::vec3 down = glm::vec3(vert[(point_number - width) * 3], vert[(point_number - width) * 3 + 1], vert[(point_number - width) * 3 + 2]);
        glm::vec3 left = glm::vec3(vert[(point_number - 1) * 3], vert[(point_number - 1) * 3 + 1], vert[(point_number - 1) * 3 + 2]);
        glm::vec3 up = glm::vec3(vert[(point_number + width) * 3], vert[(point_number + width) * 3 + 1], vert[(point_number + width) * 3 + 2]);
        glm::vec3 right = glm::vec3(vert[(point_number + 1) * 3], vert[(point_number + 1) * 3 + 1], vert[(point_number + 1) * 3 + 2]);
        res = glm::normalize(glm::cross(up - down, right - left));
    }
    return res;
}