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

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

float tf;

#define numVAOs 1
#define numVBOs 1
GLuint _renderingProgram;
GLuint _vao[numVAOs];
GLuint _vbo[numVBOs];


// variable allocation for display
GLuint vLoc, projLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat;

void init(GLFWwindow* window) {
    
    //GLFW: 直接检索窗口的帧缓冲区的当前大小
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    // 1.0472f radians = 60 degrees;  弧度 = (π / 180) * 角度
    // 1.视野角度   2.大平截头体宽高比   3.近视距（设置大了会穿过物体）   4.远视距
    pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
    
    //储存指向它的整数 ID
    //create GLuint 通过顶点着色器文件和片段着色器文件
    _renderingProgram = Utils::createShaderProgram("/Users/admin/Study/XOpenGL/Example03/Example/vertShader.glsl", "/Users/admin/Study/XOpenGL/Example03/Example/fragShader.glsl");
    //相机Location
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 240.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
    // 108/3 = 36个顶点, 12个三角形，6个面，1个面2个三角形，组成了放置在原点处 2X2X2 立方体， 实际只要8个顶点，使用EBO可以优化
    float vertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, 
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, 
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, 
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f
    };
    //创建 VAO 对象
    glGenVertexArrays(1, _vao);
    //标记VAO 为活跃状态，关联缓冲区和VAO
    glBindVertexArray(_vao[0]);
    //创建 VBO 对象
    glGenBuffers(numVBOs, _vbo);
    //把新创建的 VBO 绑定到 GL_ARRAY_BUFFER 目标上，同时也绑定到了 OpenGL 渲染管线上
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
    //将顶点数据 (CPU 内存) 拷贝到 VBO（GPU 显存）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void display(GLFWwindow* window, double currentTime) {
    
    //清除深度缓冲，否则仍使用上一次渲染迭代中的写入的深度值
    glClear(GL_DEPTH_BUFFER_BIT);
    //指定了清除背景时用的颜色值
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //清除全部缓冲区---使用clear Color填充缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    //绑定新的着色器程序
    glUseProgram(_renderingProgram);
    glPointSize(1.f);
    
    //返回位置序号
    //参数2： 必须是 an active uniform variable name ， 不能是以保留前缀“gl_”开头
    //获取shader文件中  MV矩阵和投影矩阵的统一变量
    vLoc = glGetUniformLocation(_renderingProgram, "v_matrix");
    projLoc = glGetUniformLocation(_renderingProgram, "proj_matrix");

    //构建透视矩阵
    // 1.构建视图矩阵、模型矩阵 => MV 矩阵
    // 1.1构建视图矩阵
    // GLM:创建一个平移矩阵，第一个参数是目标矩阵NXN，第二个参数是平移的方向向量
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    //通过一致变量（uniform修饰的变量）引用将一致变量值传入渲染管线
    //参数1：uniform的位置  参数2：需要加载数据的数组元素的数量或者需要修改的矩阵的数量,
    //参数3：指明矩阵是列优先(column major)（GL_FALSE）还是行优先(row major)（GL_TRUE）矩阵
    //参数4：指向由count个元素的数组的指针
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    
    timeFactor = ((float)currentTime);
    tf = glGetUniformLocation(_renderingProgram, "tf");
    glUniform1f(tf, (float)timeFactor);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);    // 0, 36, 24  (or 100000)
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(int argc, const char * argv[]) {
    
    //1.初始化glfw库
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    //指定了计算机必须与 OpenGL版本4.1兼容（Mac）,4.3（windows 10）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //指定OPENGL的配置文件  Mac
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //指定OPENGL是否向前兼容  Mac
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //初始化window
    //创建与其相关的OpenGL上下文、前2个参数指定宽高标题等
    //后2个参数 NULL, 分别用来允许全屏显示和资源共享
    GLFWwindow * window = glfwCreateWindow(800, 800, "Chapter 4 - program 1", NULL, NULL);
    //将window 与 当前 OpenGL 上下文关联
    glfwMakeContextCurrent(window);

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
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    //3.初始化函数
    init(window);

    //4.重复渲染
    //指定了清除背景时用的颜色值
    glClearColor(0.0, 0.0, 0.0, 1.0);
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
