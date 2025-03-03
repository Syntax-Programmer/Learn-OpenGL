#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum { VERTEX, FRAGMENT } ShaderType;

const char *vert_shader_source = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);"
"}\n";

const char *frag_shader_source = "#version 460 core\n"
"out vec4 fragColor;\n"
"void main() {\n"
"   fragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
"}\n";

float vertices[] = {
    -0.7, -0.3, 0.0f,
    -0.3, -0.3, 0.0f,
    -0.5, 0.3, 0.0f,
    0.2, -0.3, 0.0f,
    0.6, -0.3, 0.0f,
    0.4, 0.3, 0.0f,
};

bool initGLFW(void);
void exitGLFW(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int32_t width, int32_t height);
uint32_t createShader(const char **shader_src, ShaderType type);
uint32_t createShaderProgram(void);

bool initGLFW(void) {
    if (!glfwInit()) { 
        fprintf(stderr, "Unable to initialize GLFW\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

void exitGLFW(GLFWwindow *window) {
    if (window) { glfwDestroyWindow(window); }
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow *window, int32_t width, int32_t height) {
    glViewport(0, 0, width, height);
}

uint32_t createShader(const char **shader_src, ShaderType type) {
    uint32_t shader;
    int32_t success;
    char info_log[512];

    shader = glCreateShader((type == VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, shader_src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Unable to compile %s shader: %s.\n", (type == VERTEX) ? "vertex" : "fragment", info_log);
        return 0;
    }

    return shader;
}

uint32_t createShaderProgram(void) {
    uint32_t v_shader = createShader(&vert_shader_source, VERTEX),
    f_shader = createShader(&frag_shader_source, FRAGMENT);

    if (!v_shader || !f_shader) {
        if (v_shader) { glDeleteShader(v_shader); }
        if (f_shader) { glDeleteShader(f_shader); }
        return 0;
    }

    uint32_t shader_program = glCreateProgram();
    int32_t success;
    char info_log[512];

    glAttachShader(shader_program, v_shader);
    glAttachShader(shader_program, f_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "Unable to make final shader program: %s.\n", info_log);
        return 0;
    }

    return shader_program;
}

int32_t main(void) {
    if (!initGLFW()) { return 1; }
    GLFWwindow *window = glfwCreateWindow(1000, 800, "Two-Triangles", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Unable to create window.\n");
        return 1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exitGLFW(window);
        fprintf(stderr, "Unable to initialize GLAD.\n");
        return 1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    uint32_t VBO, VAO, shader = createShaderProgram();

    if (!shader) { return 1; }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    int32_t frame_c = 0;

    while (!glfwWindowShouldClose(window)) {
        if (frame_c == 100) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if (frame_c == 200) {
            frame_c = 0;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glfwPollEvents();
        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        frame_c++;
    }

    exitGLFW(window);

    return 0;
}