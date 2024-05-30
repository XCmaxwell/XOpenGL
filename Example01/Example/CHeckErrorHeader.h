//
//  CHeckErrorHeader.h
//  Example
//
//  Created by Mac on 2023/12/17.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

#ifndef CHeckErrorHeader_h
#define CHeckErrorHeader_h

static void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "[GL] Shader Info Log: " << log << endl;
        free(log);
    }
}

static void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "[GL] Program Info Log: " << log << endl;
        free(log);
    }
}

static bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "[GL] Error: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}




#endif /* CHeckErrorHeader_h */
