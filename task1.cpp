#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "surface_from_image.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

//Функция обратного вызова для обработки событий клавиатуры
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        //Если нажата клавиша ESCAPE, то закрываем окно
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_W){
        
    }
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

    //Инициализируем библиотеку GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    //=========================================================
    ilInit();
    //Координаты вершин треугольника
    PointStruct* p = GetPoints("image_final");
    std::cout << p->width << " " << p->height << std::endl;
    GLuint number_of_points = p->width * p->height;
    // for(int i = 0; i < number_of_points; i+=1){
    //     std::cout << p->points[3 * i] << " " << p->points[3 * i + 1] << " " << p->points[3 * i + 2] << " " <<std::endl;
    // }
    // std::cout << p->width << " " << p->height << std::endl;
    // std::cout << "end!" << std::endl;


    //Создаем буфер VertexBufferObject для хранения координат на видеокарте
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //Делаем этот буфер текущим
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Копируем содержимое массива в буфер на видеокарте
    glBufferData(GL_ARRAY_BUFFER, number_of_points * 3 * sizeof(float), p->points, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points, GL_STATIC_DRAW);

    //=========================================================

    //Создаем объект VertexArrayObject для хранения настроек полигональной модели
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //Делаем этот объект текущим
    glBindVertexArray(vao);

    //Делаем буфер с координатами текущим
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Включаем 0й вершинный атрибут
    glEnableVertexAttribArray(0);

    //Устанавливаем настройки:
    //0й атрибут,
    //3 компоненты типа GL_FLOAT,
    //не нужно нормализовать,
    //0 - значения расположены в массиве впритык,
    //0 - сдвиг от начала
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
    // std::cout << "ready!" << std::endl;
    glBindVertexArray(0);
    GLuint number_of_triangles = (p->height - 1) * (p->width - 1) * 2;
    GLuint* indices = (GLuint*)malloc(number_of_triangles * 3 * sizeof(GLuint));
    GLuint counter = 0;
    // std::cout << "ready!" << std::endl;
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
    // glm::mat4 ModMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
    // ModMat = glm::rotate(ModMat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat4 ModMat = glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 1.0));
    ModMat = glm::translate(ModMat, glm::vec3(-0.5f, -0.5f, 0.0f));
    ModMat = glm::rotate(ModMat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    // ModMat = glm::scale(ModMat, glm::vec3(2.0, 2.0, 1.0));
    // Камера находится на (4, 3, 3) в мировых координатах
    // и смотрит в центр мира
    // Верх - сверху (установите на (0, -1, 0), чтобы смотреть сверху вниз)
    glm::mat4 ViewMat = glm::lookAt( glm::vec3(0.0, 0.0, 2), glm::vec3(0.0f, 0.0f,0.0f), glm::vec3(0,-1,0) );
    glm::mat4 ProjMat = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    // glm::mat4 ProjMat = glm::ortho(-1.5, 0.5, -1.5, 0.5);

    // glm::mat4 myModelMatrix = glm::mat4(1.0f);
    
    //Вершинный шейдер
    const char* vertexShaderText =
        "#version 330\n"
        "uniform mat4 ModMat;\n"
        "uniform mat4 ViewMat;\n"
        "uniform mat4 ProjMat;\n"
        "layout(location = 0) in vec3 vertexPosition;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.1, 0.0, vertexPosition.z , 1.0);\n"
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
    free(p->points);
    free(p);
    return 0;
}