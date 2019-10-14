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

//user-defined viewport resize callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//esc key input control function prototype
void processInput(GLFWwindow *window);

int main() {
    glfwInit();
    
    //ensures glfw version 3.3, core profile usage, and mac osx compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //creates window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "Altitude", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //sets the dimensions of the render window
    glViewport(0, 0, 800, 600);
    //resize viewport callback function; executes everytime we resize window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //render loop; keep rendering until window is closed
    while(!glfwWindowShouldClose(window)) {
        //process input
        processInput(window);
        
        //rendering commands; clear color buffer and bit
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
