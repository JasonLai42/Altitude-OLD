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
const char *fragment_shader1_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char *fragment_shader2_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
    
    /* BUILD SHADER PROGRAM(S) */
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
    
    // fragment shader(s)
    int fragment_shader_orange = glCreateShader(GL_FRAGMENT_SHADER);
    int fragment_shader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_orange, 1, &fragment_shader1_source, NULL);
    glCompileShader(fragment_shader_orange);
    glShaderSource(fragment_shader_yellow, 1, &fragment_shader2_source, NULL);
    glCompileShader(fragment_shader_yellow);
    // check compilation errors for shader
    glGetShaderiv(fragment_shader_orange, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader_orange, 512, NULL, info_log);
        std::cout << "ERROR: Fragment shader compilation failed\n" << info_log << std::endl;
    }
    glGetShaderiv(fragment_shader_yellow, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader_yellow, 512, NULL, info_log);
        std::cout << "ERROR: Fragment shader compilation failed\n" << info_log << std::endl;
    }
    
    // link shaders in shader program
    int shader_program_orange = glCreateProgram();
    int shader_program_yellow = glCreateProgram();
    glAttachShader(shader_program_orange, vertex_shader);
    glAttachShader(shader_program_orange, fragment_shader_orange);
    glLinkProgram(shader_program_orange);
    glAttachShader(shader_program_yellow, vertex_shader);
    glAttachShader(shader_program_yellow, fragment_shader_yellow);
    glLinkProgram(shader_program_yellow);
    // check for linkage errors
    glGetProgramiv(shader_program_orange, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program_orange, 512, NULL, info_log);
        std::cout << "ERROR: Shader program linkage failed\n" << info_log << std::endl;
    }
    glGetProgramiv(shader_program_yellow, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program_yellow, 512, NULL, info_log);
        std::cout << "ERROR: Shader program linkage failed\n" << info_log << std::endl;
    }
    
    // delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader_orange);
    glDeleteShader(fragment_shader_yellow);
    
    /* BUILD TRIANGLE(S) */
    float vert_1[] = {
        -0.5f, -0.25f, 0.0f, // left
        0.0f, -0.25f, 0.0f, // right
        -0.25f, 0.25f, 0.0f, // top
    };
    float vert_2[] = {
        0.0f, -0.25f, 0.0f, // left
        0.5f, -0.25f, 0.0f, // right
        0.25f, 0.25f, 0.0f // top
    };
    
    /* BUILD RECTANGLE */
    //float vertices[] = {
    //    0.5f, 0.5f, 0.0f, // top right
    //    0.5f, -0.5f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f, // bottom left
    //    -0.5f, 0.5f, 0.0f // top left
    //};
    
    /* INDICES FOR TRIANGLES -> RECTANGLE */
    //unsigned int indices[] = {
    //    0, 1, 3, // first triangle
    //    1, 2, 3 // second triangle
    //};
    
    unsigned int VAO[2], VBO[2];
    // for rectangle
    //unsigned int EBO;
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    // for rectangle
    //glGenBuffers(1, &EBO);
    
    /* FIRST TRIANGLE */
    // bind VAO first
    glBindVertexArray(VAO[0]);
    // bind and set vertex buffer(s)
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_1), vert_1, GL_STATIC_DRAW);
    // bind and set element buffer(s) for rectangle
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // configure vertex attribute(s)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    /* SECOND TRIANGLE */
    // bind VAO first
    glBindVertexArray(VAO[1]);
    // bind and set vertex buffer(s)
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_2), vert_2, GL_STATIC_DRAW);
    // configure vertex attribute(s)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);
    
    // the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO
    // rarely happens; requires a call to glBindVertexArray so we generally don't unbind
    // VAOs nor VBOs
    //glBindVertexArray(0);
    
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
        glUseProgram(shader_program_orange);
        // first triangle
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // draw triangle
        glUseProgram(shader_program_yellow);
        // second triangle
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // for rectangle
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    /* END PROGRAM */
    // optional: de-allocate all resources
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    // for rectangle
    //glDeleteBuffers(1, &EBO);
    
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
