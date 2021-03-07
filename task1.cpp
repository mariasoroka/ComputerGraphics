//export LD_LIBRARY_PATH=/usr/local/lib
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "surface_from_image.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "normals_calculator.h"
#include <iostream>
#include <vector>

glm::vec3 cameraPos   = glm::vec3(2.0f, 2.0f,  1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);
GLfloat yaw   = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = -1;
GLfloat lastY = -1;
GLfloat fov = 45.0f;

//Функция обратного вызова для обработки событий клавиатуры
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        //Если нажата клавиша ESCAPE, то закрываем окно
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    GLfloat cameraSpeed = 0.05f;
    if(key == GLFW_KEY_W)
        cameraPos += cameraSpeed * cameraFront;
    if(key == GLFW_KEY_S)
        cameraPos -= cameraSpeed * cameraFront;
    if(key == GLFW_KEY_A)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(key == GLFW_KEY_D)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; 
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos){
    GLfloat xoffset = 0;
    GLfloat yoffset = 0;
    GLfloat sensitivity = 0.05f;
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS){
        if(lastX != -1 && lastY != -1){
            xoffset = xpos - lastX;
            yoffset = ypos - lastY; 
            xoffset *= sensitivity;
            yoffset *= sensitivity;
            yaw   += xoffset;
            pitch += yoffset;
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
    else{
        lastX = -1;
        lastY = -1;
    }
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
  	fov -= yoffset * 0.1;
  if(fov <= 1.0f)
  	fov = 1.0f;
  if(fov >= 45.0f)
  	fov = 45.0f;
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

    //Устанавливаем функцию обратного вызова для обработки событий клавиатуры
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPos 	(window, 400, 400); 	
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //Инициализируем библиотеку GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    glClearColor(0.3f, 0.8f, 1.0f, 1.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    //=========================================================
    ilInit();
    //Координаты вершин треугольника
    PointStruct* p = GetPoints("b.png");
    std::cout << p->width << " " << p->height << std::endl;
    GLuint number_of_points = p->width * p->height;
    // for(int i = 0; i < number_of_points; i+=1){
    //     std::cout << p->points[3 * i] << " " << p->points[3 * i + 1] << " " << p->points[3 * i + 2] << " " <<std::endl;
    // }
    // std::cout << p->width << " " << p->height << std::endl;
    // std::cout << "end!" << std::endl;

    GLfloat* normals = (GLfloat*)malloc(3 * number_of_points * sizeof(GLfloat));

    for(int i = 0; i < p->height; i++){
        for(int j = 0; j < p->width; j++){
            int point_number = (i * p->width + j);
            glm::vec3 res = normal(i, j, p->width, p->height, p->points);
            normals[point_number * 3] = res[0];
            normals[point_number * 3 + 1] = res[1];
            normals[point_number * 3 + 2] = res[2];
        }
    }
    GLfloat* vert = (GLfloat*)malloc(2 * 3 * number_of_points * sizeof(GLfloat));
    for(int i = 0; i < 3 * number_of_points; i++){
        vert[i] = p->points[i];
    }
    for(int i = 3 * number_of_points; i < 6 * number_of_points; i++){
        vert[i] = normals[i - 3 * number_of_points];
    }
    free(p->points);
    free(normals);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    //Делаем этот буфер текущим
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //Копируем содержимое массива в буфер на видеокарте
    glBufferData(GL_ARRAY_BUFFER, 6 * number_of_points * sizeof(float), vert, GL_STATIC_DRAW);
    //=========================================================
    //Создаем объект VertexArrayObject для хранения настроек полигональной модели
    GLuint vao;
    glGenVertexArrays(1, &vao);
    //Делаем этот объект текущим
    glBindVertexArray(vao);
    //Делаем буфер с координатами текущим
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //Включаем 0й вершинный атрибут - координаты
    glEnableVertexAttribArray(0);
    //Включаем 1й вершинный атрибут - цвета
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(3 * number_of_points * sizeof(float)));
    glBindVertexArray(0);

    //Создаем буфер VertexBufferObject для хранения координат на видеокарте
    // GLuint coord;
    // GLuint normal;
    // glGenBuffers(1, &coord);
    // glGenBuffers(1, &normal);
    // glNamedBufferData(coord, number_of_points * 3 * sizeof(float), p->points, GL_STATIC_DRAW);
    // glNamedBufferData(normal, number_of_points * 3 * sizeof(float), normals, GL_STATIC_DRAW);
    // GLuint vao;
    // glCreateVertexArrays(1, &vao);
    // glEnableVertexArrayAttrib(vao, 0);
    // glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    // glEnableVertexArrayAttrib(vao, 1);
    // glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0);

    // glVertexArrayAttribBinding(vao, 0, 0);
    // glVertexArrayAttribBinding(vao, 1, 1);

    // GLuint firstBinding = 0;
    // GLsizei bindingsCount = 2;
    // GLsizei bindingsCount = 1;
    // std::vector<GLuint> buffers = {coord, normal };
    // std::vector<GLintptr> offsets = {0, 0};
    // std::vector<GLsizei> strides = { static_cast<GLsizei>(3 * sizeof(float)),
    //     static_cast<GLsizei>(3 * sizeof(float)) };
    // std::vector<GLuint> buffers = {coord};
    // std::vector<GLintptr> offsets = {0};
    // std::vector<GLsizei> strides = { static_cast<GLsizei>(3 * sizeof(float))};
    // glVertexArrayVertexBuffers(vao, firstBinding, bindingsCount, buffers.data(), offsets.data(), strides.data());

    GLuint number_of_triangles = (p->height - 1) * (p->width - 1) * 2;
    GLuint* indices = (GLuint*)malloc(number_of_triangles * 3 * sizeof(GLuint));
    GLuint counter = 0;
    for(unsigned int i = 0; i < (p->height - 1); i++){
        for(unsigned int j = 0; j < (p->width - 1); j++){
            indices[counter * 3] = j + i * p->width;
            indices[counter * 3 + 1] = j + p->width + i * p->width;
            indices[counter * 3 + 2] = j + 1 + i * p->width;
            counter++;
            indices[counter * 3] = j + p->width + i * p->width;
            indices[counter * 3 + 1] = j + p->width + 1 + i * p->width;
            indices[counter * 3 + 2] = j + 1 + i * p->width;
            counter++;
        }
    }
    // for(int i = 0; i < number_of_triangles; i++){
    //     std::cout << indices[i * 3] << " " << indices[i * 3 + 1] << " " << indices[i * 3 + 2] << " " << std::endl;
    // }
    // std::cout << "ready!" << std::endl;

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_triangles * 3 * sizeof(GL_UNSIGNED_INT), indices, GL_STATIC_DRAW);
    //=========================================================
    glm::mat4 ModMat = glm::scale(glm::mat4(1.0f), glm::vec3(3.0, 3.0, 1.0));
    ModMat = glm::translate(ModMat, glm::vec3(-0.5f, -0.5f, 0.0f));
    glm::mat4 ViewMat = glm::lookAt( glm::vec3(0.0, 0.0, 2), glm::vec3(0.0f, 0.0f,0.0f), glm::vec3(0,0,1) );
    glm::mat4 ProjMat = glm::perspective(fov, 1.0f, 0.1f, 100.0f);

 
    //Вершинный шейдер
    const char* vertexShaderText =
        "#version 330\n"
        "uniform mat4 ModMat;\n"
        "uniform mat4 ViewMat;\n"
        "uniform mat4 ProjMat;\n"
        "layout(location = 0) in vec3 vertexPosition;\n"
        "layout(location = 1) in vec3 vertexNormal;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.1, 0.0, (vertexPosition.z) , 1.0);\n"
        "   gl_Position = ProjMat * ViewMat * ModMat * vec4(vertexPosition, 1.0);\n"
            // "gl_Position = ModMat * vec4(vertexPosition, 1.0);\n"
        "}\n";

    //Создаем шейдерный объект
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    
    //Передаем в шейдерный объект текст шейдера
    glShaderSource(vs, 1, &vertexShaderText, nullptr);

    //Компилируем шейдер
    glCompileShader(vs);
    
    //Проверяем ошибки компиляции
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

    //Фрагментный шейдер
    const char* fragmentShaderText =
        "#version 330\n"
        "in vec4 color;\n"
        "out vec4 fragColor;\n"

        "void main()\n"
        "{\n"
        "    fragColor = color;\n"
        "}\n";

    //Создаем шейдерный объект
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    //Передаем в шейдерный объект текст шейдера
    glShaderSource(fs, 1, &fragmentShaderText, nullptr);

    //Компилируем шейдер
    glCompileShader(fs);

    //Проверяем ошибки компиляции
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

    //Создаем шейдерную программу
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
        //Проверяем события ввода (здесь вызывается функция обратного вызова для обработки событий клавиатуры)
        glfwPollEvents();

        //Получаем размеры экрана (окна)
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        //Устанавливаем порт вывода на весь экран (окно)
        glViewport(0, 0, width, height);

        //Очищаем порт вывода (буфер цвета и буфер глубины)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Подключаем шейдерную программу
        glUseProgram(program);

        // GLfloat radius = 2.0f;
        // GLfloat camX = sin(glfwGetTime()) * radius;
        // GLfloat camY = cos(glfwGetTime()) * radius;
        // glm::mat4 view;
        // ViewMat = glm::lookAt(glm::vec3(camX, camY, 1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
        ProjMat = glm::perspective(fov, 1.0f, 0.1f, 100.0f);
        ViewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        GLint uniformLocMod = glGetUniformLocation(program, "ModMat");
        glUniformMatrix4fv(uniformLocMod, 1, 0, glm::value_ptr(ModMat));
        GLint uniformLocView = glGetUniformLocation(program, "ViewMat");
        glUniformMatrix4fv(uniformLocView, 1, 0, glm::value_ptr(ViewMat));
        GLint uniformLocProj = glGetUniformLocation(program, "ProjMat");
        glUniformMatrix4fv(uniformLocProj, 1, 0, glm::value_ptr(ProjMat));
        //Подключаем VertexArrayObject с настойками полигональной модели
        glBindVertexArray(vao);

        //Рисуем полигональную модель (состоит из треугольников, сдвиг 0, количество вершин 3)
        glDrawElements(GL_TRIANGLES, number_of_triangles * 3, GL_UNSIGNED_INT, indices);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window); //Переключаем передний и задний буферы
    }

    //Удаляем созданные объекты OpenGL
    glDeleteProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    free(p);
    free(vert);
    return 0;
}