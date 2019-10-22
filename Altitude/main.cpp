//
//  main.cpp
//  GLFW OpenGL
//
//  Created by Jason Lai on 9/25/19.
//  Copyright © 2019 Jason Lai. All rights reserved.
//

#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

// user-defined viewport resize callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// esc key input control function prototype
void process_input(GLFWwindow *window);

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {
    
    /* INITIALIZE GLFW AND BUILD WINDOW */
    glfwInit();
    
    // ensures glfw version 3.3, core profile usage, and mac osx compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // creates window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Altitude", NULL, NULL);
    if(window == NULL) {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    /* NEED TO INITIALIZE GLEW AFTER CREATING A GLFW CONTEXT */
    glewInit();
    
    // sets the dimensions of the render window
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    // resize viewport callback function; executes everytime we resize window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /* BUILD SHADER PROGRAM */
    // vertex shader
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    // check compilation errors for shader
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR: Vertex shader compilation failed\n" << info_log << std::endl;
    }
    
    // fragment shader
    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    // check compilation errors for shader
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR: Fragment shader compilation failed\n" << info_log << std::endl;
    }
    
    // link shaders in shader program
    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // check for linkage errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR: Shader program linkage failed\n" << info_log << std::endl;
    }
    
    // delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    /* BUILD TRIANGLE */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.0f, 0.5f, 0.0f //top
    };
    
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // bind VAO first
    glBindVertexArray(VAO);
    
    // bind and set vertex buffer(s)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // configure vertex attribute(s)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO
    // rarely happens; requires a call to glBindVertexArray so we generally don't unbind
    // VAOs nor VBOs
    glBindVertexArray(0);
    
    /* OPTIONAL: WIREFRAME POLYGONS */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    /* RENDER LOOP; KEEP RENDERING UNTIL WINDOW IS CLOSED */
    while(!glfwWindowShouldClose(window)) {
        // process input
        process_input(window);
        
        // rendering commands; clear color buffer and bit
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw triangle
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    /* END PROGRAM */
    // optional: de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}

// execute this function whenever the window size is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// process any recognized keystrokes
void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
