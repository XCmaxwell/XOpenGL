//
//  main.cpp
//  Example
//
//  Created by Mac on 2023/11/12.
//

#include <iostream>
#include <fstream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utils.h"

using namespace std;

// C++应用程序--顶点着色器-- 各种着色器 --- 光栅化 --- 片段着色器 --- 像素操作
// 3D模型--> 图元--> 顶点
// 顶点着色器/片段着色器 --> 管线


/*
/// 通知 OpenGL 构建三角形-->管线中的GLSL 代码开始执行
/// - Parameter mode: 图元类型（三角形 GL_TRIANGLES ）
/// - Parameter first: 从哪个顶点开始绘制
/// - Parameter first: 总共要绘制的顶点数
///   线框模型： glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
glDrawArrays(GLenum mode, Glint first, GLsizei count);
 */

#define numVAOs 1
GLuint _renderingProgram;
GLuint _vao[numVAOs];
static float x = 0.0f; // 三角形在 x 轴的位置
static float moveX = 0.01f; // 移动三角形的偏移量

void init(GLFWwindow* window) {
    
    // 储存指向它的整数 ID
    _renderingProgram = Utils::createShaderProgram("/Users/admin/Study/XOpenGL/Example/Example/vertShader.glsl", "/Users/admin/Study/XOpenGL/Example/Example/fragShader.glsl");
    // 创建一个或多个顶点数组对象的标识符
    glGenVertexArrays(numVAOs, _vao);
    // 将创建的顶点数组对象绑定到当前OpenGL上下文
    glBindVertexArray(_vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    
    //清除深度缓冲，否则仍使用上一次渲染迭代中的写入的深度值
    glClear(GL_DEPTH_BUFFER_BIT);
    //指定了清除背景时用的颜色值
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //清除全部缓冲区---使用clear Color填充缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
     // 切换至让三角形向右移动
     x += moveX;
     if (x > 1.0f) moveX = -0.01f; // 沿 x 轴移动三角形
     if (x < -1.0f) moveX = 0.01f; // 切换至让三角形向左移动
     GLuint offsetLocation = glGetUniformLocation(_renderingProgram, "offset"); // 获取指向 offset 变量的指针
     glProgramUniform1f(_renderingProgram, offsetLocation, x);
    
    //绑定新的着色器程序
    glUseProgram(_renderingProgram);
    //glPointSize用于指定一个顶点对应渲染的像素点数，glPointSize来设置点的直径大小
    glPointSize(1.0f);
    //从已经绑定的顶点数组对象（VAO）中提取顶点数据
    //0.要渲染的图元类型:点、线、三角形， 1.开始渲染的第一个顶点的索引  2.渲染的顶点数量。
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, const char * argv[]) {
    
    //1.初始化glfw库
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    //指定了计算机必须与 OpenGL版本4.1兼容
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //指定OPENGL的配置文件
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //指定OPENGL是否向前兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //初始化window
    //创建与其相关的OpenGL上下文、前2个参数指定宽高标题等
    //后2个参数 NULL, 分别用来允许全屏显示和资源共享
    GLFWwindow * window = glfwCreateWindow(600, 600, "Chapter 2 - program 1", NULL, NULL);
    //将window 与 当前 OpenGL 上下文关联
    glfwMakeContextCurrent(window);
    //选择绘制区域
    glViewport(0,0,600,600);
    //启用GLEW库的实验性功能
    //有些显卡的驱动程序不能正确给出所支持的扩展信息，导致GLEW不能正确获取某些函数的入口地址
    glewExperimental = GL_TRUE;
    
    //2.初始化glew库
    if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }
    /* 开启垂直同步 v-sync
     * 垂直同步是一种图形渲染技术，它的目的是将图像的刷新与显示器的垂直刷新频率进行同步，以避免出现画面撕裂（tearing）现象。
     * 画面撕裂是指在图像渲染过程中，显示器正在进行刷新，而此时渲染的图像只完成了一部分，导致画面出现不连贯的情况。
     */
    glfwSwapInterval(1);

    //3.初始化函数
    init(window);

    //4.重复渲染
    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        //绘制屏幕
        glfwSwapBuffers(window);
        //处理窗口相关事件
        glfwPollEvents();
    }

    //5.销毁窗口、终止运行
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
