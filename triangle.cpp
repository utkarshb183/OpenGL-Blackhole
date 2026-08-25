#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

const char* vertexShaderSource =
    "#version 330 core\r\n"
    "layout (location = 0) in vec3 aPos;\r\n"
    "void main()\r\n"
    "{\r\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\r\n"
    "}\r\n";

const char* fragmentShaderSource =
    "#version 330 core\r\n"
    "out vec4 FragColor;\r\n"
    "void main()\r\n"
    "{\r\n"
    "   vec2 uv = (gl_FragCoord.xy - vec2(400.0, 300.0))/ vec2(300.0, 300.0);\r\n"
    "   vec3 camPos = vec3(0.0, 0.0, -10.0);\r\n"
    "   vec3 rayDir = normalize(vec3(uv.x, uv.y, 1.0));\r\n"
    "   vec3 sphereCenter = vec3(0.0, 0.0, 0.0); \r\n"
    "   float radius = 2.0;\r\n"
    "   vec3 oc = camPos - sphereCenter;\r\n"
    "   float a = dot(rayDir, rayDir);\r\n"
    "   float b = 2.0*dot(oc, rayDir);\r\n"
    "   float c = dot(oc, oc) - radius*radius;\r\n"
    "   float d = b*b - 4.0*a*c;\r\n"
    "   if( d > 0.0){\r\n"
    "   vec3 hitPoint = camPos + rayDir*(-b - sqrt(d))/(2.0*a);\r\n"
    "   vec3 normal = normalize(hitPoint - sphereCenter);\r\n"
    "   vec3 lightDir = normalize(vec3(1.0, 1.0, -1.0));\r\n"
    "   float diff = max(dot(normal, lightDir), 0.0);\r\n"
    "   FragColor = vec4(diff, diff*0.5, 0.0, 1.0);}\r\n"
    "   else\r\n"
    "   FragColor = vec4(0.0, 0.0, 0.0, 1.0);\r\n"
    "}\r\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);
    if(window == NULL){
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Vertex shader error: " << infoLog << std::endl;
}

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = { -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
