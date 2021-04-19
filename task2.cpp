#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "for_surf_calc.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <SOIL2.h>

enum navigation{
    free_camera,
    first_person
};
struct LightInfo {
    glm::vec3 dir;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
struct MaterialProp{
    glm::vec3 color;
    glm::vec3 a_ref;
    glm::vec3 d_ref;
    glm::vec3 s_ref;
    float shininess;
};
glm::vec3 cameraPos   = glm::vec3(0.5f, 0.5f,  7.0f);
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);
GLfloat yaw   = 0.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = 0;
GLfloat lastY = 0;
GLfloat fov = 45.0f;
navigation navigation_type = free_camera;
GLfloat person_height = 0.2;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f; 
//Функция обратного вызова для обработки событий клавиатуры
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    GLfloat cameraSpeed = deltaTime * 1.0f;
    if(key == GLFW_KEY_W)
        cameraPos += cameraSpeed * cameraFront;
    if(key == GLFW_KEY_S)
        cameraPos -= cameraSpeed * cameraFront;
    if(key == GLFW_KEY_A)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(key == GLFW_KEY_D)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; 
    if(key == GLFW_KEY_SPACE){
        navigation_type = first_person;
    }
    if(key == GLFW_KEY_ENTER){
        navigation_type = free_camera;
    }

}
//Функция обратного вызова для обработки движения мыши
void mouseCallback(GLFWwindow* window, double xpos, double ypos){
    GLfloat dx = 0.0;
    GLfloat dy = 0.0;
    GLfloat sensitivity = 0.05f;
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(state == GLFW_PRESS){
        dx = xpos - lastX;
        dy = ypos - lastY;
        dx *= sensitivity;
        dy *= sensitivity;
        yaw   += dx;
        pitch += dy;
        if(pitch > 89.0f){
            pitch =  89.0f;
        }
        if(pitch < -89.0f){
            pitch = -89.0f;
        }
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.z = sin(glm::radians(pitch));
        front.y = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(front);
    }
    lastX = xpos;
    lastY = ypos;
}
//Функция обратного вызова для обработки колесика мыши
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 90.0f)
  	fov -= yoffset * 0.05;
  if(fov <= 1.0f)
  	fov = 1.0f;
  if(fov >= 90.0f)
  	fov = 90.0f;
}
int main()
{
    //Инициализируем библиотеку GLFW
    if (!glfwInit())
    {
        std::cerr << "ERROR: could not start GLFW3\n";
        exit(1);
    }

    //Устанавливаем параметры создания графического контекста
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Создаем графический контекст (окно)
    GLFWwindow* window = glfwCreateWindow(800, 800, "MIPT OpenGL demos", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "ERROR: could not open window with GLFW3\n";
        glfwTerminate();
        exit(1);
    }

    //Делаем этот контекст текущим
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    //Устанавливаем функцию обратного вызова для обработки событий клавиатуры и мыши
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPos 	(window, 400, 400); 	
    //Инициализируем библиотеку GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //=========================================================
    //Координаты вершин треугольников, нормали в вершинах и текстурные координаты
    
    int width;
    int height;
    int channels = 1;
    unsigned char *ht_map = SOIL_load_image("771SorokaData1/image_final", &width, &height, &channels, SOIL_LOAD_L);

    std::cout << width << " " << height << std::endl;
    GLuint number_of_points = width * height;

    GLfloat* vert = new GLfloat[(2 * 3 + 2) * number_of_points];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int point_number = i * width + j;
            // Координаты
            vert[point_number * 3] = (GLfloat)i / height;
            vert[point_number * 3 + 1] = (GLfloat)j / width;
            vert[point_number * 3 + 2] = (GLfloat)(ht_map[point_number]) / 255;
            // Нормали
            glm::vec3 res = normal(i, j, width, height, vert);
            vert[3 * number_of_points + point_number * 3] = res[0];
            vert[3 * number_of_points + point_number * 3 + 1] = res[1];
            vert[3 * number_of_points + point_number * 3 + 2] = res[2];
            // Текстура
            vert[3 * 2 * number_of_points + point_number * 2] = vert[3 * point_number] * 20;
            vert[3 * 2 * number_of_points + point_number * 2 + 1] = vert[3 * point_number + 1] * 20;
        }
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (2 * 3 + 2) * number_of_points * sizeof(float), vert, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(3 * number_of_points * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(3 * 2 * number_of_points * sizeof(float)));
    glBindVertexArray(0);

    //=========================================================
    // Индексный массив
    GLuint number_of_triangles = (height - 1) * (width - 1) * 2;
    GLuint* indices = new GLuint[number_of_triangles * 3];
    GLuint counter = 0;
    for(unsigned int i = 0; i < (height - 1); i++){
        for(unsigned int j = 0; j < (width - 1); j++){
            indices[counter * 3] = j + i * width;
            indices[counter * 3 + 1] = j + width + i * width;
            indices[counter * 3 + 2] = j + 1 + i * width;
            counter++;
            indices[counter * 3] = j + width + i * width;
            indices[counter * 3 + 1] = j + width + 1 + i * width;
            indices[counter * 3 + 2] = j + 1 + i * width;
            counter++;
        }
    }
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_triangles * 3 * sizeof(GL_UNSIGNED_INT), indices, GL_STATIC_DRAW);
    //=========================================================
    // Загрузка текстур
    GLuint rockTexture;
    glGenTextures(1, &rockTexture);
    glBindTexture(GL_TEXTURE_2D, rockTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int rockTexWidth, rockTexHeight;
    unsigned char* rockImage = SOIL_load_image("771SorokaData1/rock-texture.jpg", &rockTexWidth, &rockTexHeight,0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rockTexWidth, rockTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, rockImage); 
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(rockImage);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint mossTexture;
    glGenTextures(1, &mossTexture);
    glBindTexture(GL_TEXTURE_2D, mossTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int mossTexWidth, mossTexHeight;
    unsigned char* mossImage = SOIL_load_image("771SorokaData1/moss-texture.jpg", &mossTexWidth, &mossTexHeight,0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mossTexWidth, mossTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mossImage); 
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(mossImage);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint flowerTexture;
    glGenTextures(1, &flowerTexture);
    glBindTexture(GL_TEXTURE_2D, flowerTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int flowerTexWidth, flowerTexHeight;
    unsigned char* flowerImage = SOIL_load_image("771SorokaData1/flower-texture.jpg", &flowerTexWidth, &flowerTexHeight,0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, flowerTexWidth, flowerTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, flowerImage); 
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(flowerImage);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint snowTexture;
    glGenTextures(1, &snowTexture);
    glBindTexture(GL_TEXTURE_2D, snowTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int snowTexWidth, snowTexHeight;
    unsigned char* snowImage = SOIL_load_image("771SorokaData1/snow-texture.jpg", &snowTexWidth, &snowTexHeight,0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, snowTexWidth, snowTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, snowImage); 
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(snowImage);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint templateTexture;
    glGenTextures(1, &templateTexture);
    glBindTexture(GL_TEXTURE_2D, templateTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int templateWidth, templateHeight;
    unsigned char* templateImage = SOIL_load_image("771SorokaData1/layer8.png", &templateWidth, &templateHeight,0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, templateWidth, templateHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, templateImage); 
    SOIL_free_image_data(templateImage);
    glBindTexture(GL_TEXTURE_2D, 0);

    //=========================================================
    // Параметры освещения и характеристики материалов
    LightInfo Light = {glm::vec3(-1.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0)};
    MaterialProp rockMaterial = {glm::vec3(0.7, 0.7, 0.7), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.2, 0.2, 0.2), 4.0};
    MaterialProp mossMaterial = {glm::vec3(0.25, 0.4, 0.0), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.0, 0.0, 0.0), 4.0};
    MaterialProp flowerMaterial = {glm::vec3(0.5, 1.0, 0.0), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.1, 0.1, 0.1), 4.0};
    MaterialProp snowMaterial = {glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0.3, 0.3, 0.3), 1.0};
    Light.dir = glm::normalize(Light.dir);

    //=========================================================
    // Матрицы модели, вида и проекции
    glm::mat4 ModMat = glm::scale(glm::mat4(1.0f), glm::vec3(10.0, 10.0, 1.5));
    ModMat = glm::translate(ModMat, glm::vec3(-0.5f, -0.5f, 0.0f));
    glm::mat4 ViewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 ProjMat = glm::perspective(fov, 1.0f, 0.001f, 100.0f);
    glm::mat4 NormMat = glm::transpose(glm::inverse(ModMat));
 
    //=========================================================
    //Создание вершинного шейдера
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    std::string VertexShaderFile = "VertexShaderTask2";
    std::ifstream shaderFile(VertexShaderFile.c_str());
    if (shaderFile.fail())
    {
        std::cerr << "Failed to load shader file " << VertexShaderFile << std::endl;
        exit(1);
    }
    std::string shaderFileContent((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
    shaderFile.close();
    const char* vertexShaderText = shaderFileContent.c_str();
    glShaderSource(vs, 1, &vertexShaderText, nullptr);

    glCompileShader(vs);
    
    int status = -1;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint errorLength;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &errorLength);
        std::vector<char> errorMessage;
        errorMessage.resize(errorLength);
        glGetShaderInfoLog(vs, errorLength, 0, errorMessage.data());
        std::cerr << "Failed to compile the shader:\n" << errorMessage.data() << std::endl;
        exit(1);
    }

    //=========================================================
    //Создание фрагментного шейдера
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    std::string FragmentShaderFile = "FragmentShaderTask2";
    shaderFile = std::ifstream(FragmentShaderFile.c_str());
    if (shaderFile.fail())
    {
        std::cerr << "Failed to load shader file " << FragmentShaderFile << std::endl;
        exit(1);
    }
    shaderFileContent = std::string((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
    shaderFile.close();
    const char* fragmentShaderText = shaderFileContent.c_str();
    glShaderSource(fs, 1, &fragmentShaderText, nullptr);

    glCompileShader(fs);

    status = -1;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint errorLength;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &errorLength);
        std::vector<char> errorMessage;
        errorMessage.resize(errorLength);
        glGetShaderInfoLog(fs, errorLength, 0, errorMessage.data());
        std::cerr << "Failed to compile the shader:\n" << errorMessage.data() << std::endl;
        exit(1);
    }

    //=========================================================
    //Создание шейдерной программы

    GLuint program = glCreateProgram();

    //Прикрепляем шейдерные объекты
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //Линкуем программу
    glLinkProgram(program);

    //Проверяем ошибки линковки
    status = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint errorLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorLength);

        std::vector<char> errorMessage;
        errorMessage.resize(errorLength);

        glGetProgramInfoLog(program, errorLength, 0, errorMessage.data());

        std::cerr << "Failed to link the program:\n" << errorMessage.data() << std::endl;

        exit(1);
    }

    //=========================================================
    //Цикл рендеринга (пока окно не закрыто)

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        int wind_width, wind_height;
        glfwGetFramebufferSize(window, &wind_width, &wind_height);
        glViewport(0, 0, wind_width, wind_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rockTexture);
        glUniform1i(glGetUniformLocation(program, "rockTexture"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mossTexture);
        glUniform1i(glGetUniformLocation(program, "mossTexture"), 1);  
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, flowerTexture);
        glUniform1i(glGetUniformLocation(program, "flowerTexture"), 2); 
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, snowTexture);
        glUniform1i(glGetUniformLocation(program, "snowTexture"), 3); 
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, templateTexture);
        glUniform1i(glGetUniformLocation(program, "templateTexture"), 4); 

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(navigation_type == first_person){
            cameraPos.z = (surface_z(cameraPos, width, height, vert, ModMat) + person_height);
        }
        ProjMat = glm::perspective(fov, 1.0f, 0.001f, 100.0f);
        ViewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        GLint uniformLocMod = glGetUniformLocation(program, "ModMat");
        glUniformMatrix4fv(uniformLocMod, 1, 0, glm::value_ptr(ModMat));
        GLint uniformLocView = glGetUniformLocation(program, "ViewMat");
        glUniformMatrix4fv(uniformLocView, 1, 0, glm::value_ptr(ViewMat));
        GLint uniformLocProj = glGetUniformLocation(program, "ProjMat");
        glUniformMatrix4fv(uniformLocProj, 1, 0, glm::value_ptr(ProjMat));
        GLint uniformLocNorm = glGetUniformLocation(program, "NormMat");
        glUniformMatrix4fv(uniformLocNorm, 1, 0, glm::value_ptr(NormMat));
        GLint uniformLocLightInfoDir = glGetUniformLocation(program, "Light.dir");
        glUniform3fv(uniformLocLightInfoDir, 1, glm::value_ptr(Light.dir));
        GLint uniformLocLightInfoAmb = glGetUniformLocation(program, "Light.La");
        glUniform3fv(uniformLocLightInfoAmb, 1, glm::value_ptr(Light.ambient));
        GLint uniformLocLightInfoDif = glGetUniformLocation(program, "Light.Ld");
        glUniform3fv(uniformLocLightInfoDif, 1, glm::value_ptr(Light.diffuse));
        GLint uniformLocLightInfoSpec = glGetUniformLocation(program, "Light.Ls");
        glUniform3fv(uniformLocLightInfoSpec, 1, glm::value_ptr(Light.specular));
        GLint uniformLocCamPos = glGetUniformLocation(program, "CamPos");
        glUniform3fv(uniformLocCamPos, 1, glm::value_ptr(cameraPos));
       
        glUniform3fv(glGetUniformLocation(program, "Rock.color"), 1, glm::value_ptr(rockMaterial.color));
        glUniform3fv(glGetUniformLocation(program, "Rock.a_ref"), 1, glm::value_ptr(rockMaterial.a_ref));
        glUniform3fv(glGetUniformLocation(program, "Rock.d_ref"), 1, glm::value_ptr(rockMaterial.d_ref));
        glUniform3fv(glGetUniformLocation(program, "Rock.s_ref"), 1, glm::value_ptr(rockMaterial.s_ref));
        glUniform1f(glGetUniformLocation(program, "Rock.shininess"), rockMaterial.shininess);

        glUniform3fv(glGetUniformLocation(program, "Moss.color"), 1, glm::value_ptr(mossMaterial.color));
        glUniform3fv(glGetUniformLocation(program, "Moss.a_ref"), 1, glm::value_ptr(mossMaterial.a_ref));
        glUniform3fv(glGetUniformLocation(program, "Moss.d_ref"), 1, glm::value_ptr(mossMaterial.d_ref));
        glUniform3fv(glGetUniformLocation(program, "Moss.s_ref"), 1, glm::value_ptr(mossMaterial.s_ref));
        glUniform1f(glGetUniformLocation(program, "Moss.shininess"), mossMaterial.shininess);
        
        glUniform3fv(glGetUniformLocation(program, "Flower.color"), 1, glm::value_ptr(flowerMaterial.color));
        glUniform3fv(glGetUniformLocation(program, "Flower.a_ref"), 1, glm::value_ptr(flowerMaterial.a_ref));
        glUniform3fv(glGetUniformLocation(program, "Flower.d_ref"), 1, glm::value_ptr(flowerMaterial.d_ref));
        glUniform3fv(glGetUniformLocation(program, "Flower.s_ref"), 1, glm::value_ptr(flowerMaterial.s_ref));
        glUniform1f(glGetUniformLocation(program, "Flower.shininess"), flowerMaterial.shininess);

        glUniform3fv(glGetUniformLocation(program, "Snow.color"), 1, glm::value_ptr(snowMaterial.color));
        glUniform3fv(glGetUniformLocation(program, "Snow.a_ref"), 1, glm::value_ptr(snowMaterial.a_ref));
        glUniform3fv(glGetUniformLocation(program, "Snow.d_ref"), 1, glm::value_ptr(snowMaterial.d_ref));
        glUniform3fv(glGetUniformLocation(program, "Snow.s_ref"), 1, glm::value_ptr(snowMaterial.s_ref));
        glUniform1f(glGetUniformLocation(program, "Snow.shininess"), snowMaterial.shininess);

        //Подключаем VertexArrayObject с настойками полигональной модели
        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, number_of_triangles * 3, GL_UNSIGNED_INT, indices);
        glfwSwapBuffers(window);
    }

    //Удаляем созданные объекты OpenGL
    glDeleteProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    SOIL_free_image_data( ht_map );
    glfwTerminate();
    delete[] vert;
    delete[] indices;
    return 0;
}