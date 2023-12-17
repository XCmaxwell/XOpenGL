//
//  main.cpp
//  Example
//
//  Created by Mac on 2023/11/12.
//

#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

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
GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
    
    const char *vshaderSource =
        "#version 410    \n"
        "void main(void) \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    const char *fshaderSource =
        "#version 410    \n"
        "out vec4 color; \n"
    "void main(void) \n"
    "{  if (gl_FragCoord.x < 300) {\n "
    "       color = vec4(1.0, 0.0, 1.0, 1.0);\n"
    "   } else {\n"
    "    color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "   }\n"
    "}";
    //创建 顶点着色器
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    //创建 片段着色器
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    //加载着色器资源
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    //编译着色器
    glCompileShader(vShader);
    glCompileShader(fShader);
    
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    return vfProgram;
}

void init(GLFWwindow* window) {
    
    // 储存指向它的整数 ID
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    
    //指定了清除背景时用的颜色值
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //清除全部缓冲区---使用clear Color填充缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(renderingProgram);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

int main(int argc, const char * argv[]) {
    
    //1.初始化glfw库
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    //指定了计算机必须与 OpenGL版本4.1兼容
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //Mac专有
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //初始化window
    //创建与其相关的OpenGL上下文、前2个参数指定宽高标题等
    //后2个参数 NULL， 分别用来允许全屏显示和资源共享
    GLFWwindow * window = glfwCreateWindow(600, 600, "Chapter 2 - program 1", NULL, NULL);
    //将window与 当前 OpenGL 上下文关联
    glfwMakeContextCurrent(window);
    //选择绘制区域
    glViewport(0,0,300,300);
    glewExperimental = GL_TRUE;
    
    //2.初始化glew库
    if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }
    //开启垂直同步
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
