#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

void framebuffer_size_callback(GLFWwindow *window, int w, int h);

void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}
uint32_t createShaders(const char **pShader_src, const char type);

const char *vertex_shader_source = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";
const char *frag_shader_source = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"FragColor = vec4(0.5f, 0.2f, 0.3f, 1.0f);\n"
"}\0";

uint32_t createShaders(const char **pShader_src, const char type) {
    uint32_t shader;
    int success;
    char err_log[512];

    if (type == 'v') { shader = glCreateShader(GL_VERTEX_SHADER); }
    if (type == 'f') { shader = glCreateShader(GL_FRAGMENT_SHADER); }
    else { return 0; }

    glShaderSource(shader, 1, pShader_src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(err_log), NULL, err_log);
        printf("Error compiling %s shader: %s", (type == 'v') ? "Vertex" : "Fragment", err_log);
        return 0;
    }

    return shader;
}

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) { exit(1); }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { exit(1); }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    framebuffer_size_callback(window, SCREEN_WIDTH, SCREEN_HEIGHT);

    float triangle_vertices[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
    uint32_t vertex_shader = createShaders(&vertex_shader_source, 'v'),
             frag_shader = createShaders(&frag_shader_source, 'f');
    if (!vertex_shader || !frag_shader) { return 1; }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}