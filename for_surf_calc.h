#include <GL/glew.h>
#include <glm/glm.hpp>
glm::vec3 normal(int i, int j, int width, int height, float* vert);
GLfloat surface_z(glm::vec3 CamPos, int width, int height, float* vert, glm::mat4 ModMat);