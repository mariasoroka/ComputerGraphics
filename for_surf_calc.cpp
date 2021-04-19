#include "for_surf_calc.h"
#include <iostream>
//Подсчет нормалей в вершинах
glm::vec3 normal(int i, int j, int width, int height, float* vert){
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
//Расчет координаты z из положения по x, y
GLfloat surface_z(glm::vec3 CamPos, int width, int height, float* vert, glm::mat4 ModMat){
    glm::vec4 newCamPos = glm::inverse(ModMat) * glm::vec4(CamPos, 1.0); 
    GLfloat x = newCamPos.x;
    GLfloat y = newCamPos.y;
    double dy = 1.0 / width;
    double dx = 1.0 / height;
    double x_cell_d;
    double y_cell_d;
    double x_frac = modf(x / dx, &x_cell_d);
    double y_frac = modf(y / dy, &y_cell_d);
    int x_cell = int(x_cell_d);
    int y_cell = int(y_cell_d);
    int point_number = x_cell * width + y_cell;
    GLfloat z;
    if(x_cell < 0){
        if(y_cell < 0){ z = vert[0 * 3 + 2]; }
        else if(y_cell >= width){ z = vert[(width - 1) * 3 + 2]; }
        else{ z = vert[int(y_cell) * 3 + 2]; }
    }
    else if(y_cell < 0){
        if(x_cell >= height){ z = vert[(height - 1) * width * 3 + 2]; }
        else{ z = vert[int(x_cell) * width * 3 + 2]; }
    }
    else if(x_cell >= height){
        if(y_cell >= width){ z = vert[(width * height - 1) * 3 + 2]; }
        else{ z = vert[((height - 1) * width + int(y_cell)) * 3 + 2]; }
    }
    else if(y_cell >= width){ z = vert[((int(x_cell) + 1) * width - 1) * 3 + 2]; }
    else if(x_frac < y_frac){
        glm::vec3 v_1 = glm::vec3(0, vert[(point_number + 1) * 3 + 1] - vert[(point_number) * 3 + 1], vert[(point_number + 1) * 3 + 2] - vert[(point_number) * 3 + 2] );
        glm::vec3 v_2 = glm::vec3(vert[(point_number + width) * 3 + 0] - vert[(point_number) * 3 + 0], 0, vert[(point_number + width) * 3 + 2] - vert[(point_number) * 3 + 2] );
        z = vert[(point_number) * 3 + 2] + y_frac * v_1.z + x_frac * v_2.z;
    }
    else{
        glm::vec3 v_1 = glm::vec3(0, vert[(point_number + width + 1) * 3 + 1] - vert[(point_number + width) * 3 + 1], vert[(point_number + width + 1) * 3 + 2] - vert[(point_number + width) * 3 + 2] );
        glm::vec3 v_2 = glm::vec3(vert[(point_number + width) * 3 + 0] - vert[(point_number) * 3 + 0], 0, vert[(point_number + width) * 3 + 2] - vert[(point_number) * 3 + 2] );
        z = vert[(point_number) * 3 + 2] + y_frac * v_1.z + x_frac * v_2.z;
    }
    glm::vec4 resPos = ModMat * glm::vec4(x_cell, y_cell, z, 1.0);
    return resPos.z;
}