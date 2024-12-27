#include "CCOpenGLWidget.h"

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    vHandle = -1; // 初始化顶点着色器句柄
    pHandle = -1; // 初始化片段着色器句柄
    shaderProgram = -1; // 初始化着色器程序句柄
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    uninitShader(); // 释放着色器资源
}

void CCOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // 初始化 OpenGL 函数
    initShader(); // 初始化着色器
}


void CCOpenGLWidget::paintGL()
{
    // 定义矩形的顶点和颜色
    CCVertex rect[] =
        {
            {10, 10, 0, 1, 0, 1}, // 左下角
            {110, 10, 0, 1, 1, 0}, // 右下角
            {10, 110, 0, 0, 1, 1}, // 左上角
            {110, 110, 0, 1, 1, 0}, // 右上角
        };

    // 启用顶点数组和颜色数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // 设置顶点指针和颜色指针
    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), rect);
    glColorPointer(3, GL_FLOAT, sizeof(CCVertex), &rect[0].r);

    shaderBegin(); // 开始使用着色器
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制矩形
    shaderEnd(); // 结束使用着色器

    // 禁用顶点数组和颜色数组
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void CCOpenGLWidget::resizeGL(int width,int height)
{
    glViewport(0, 0, width, height); // 设置视口
    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    // 设置正交投影
    glOrtho(0, width, height, 0, -100, 100);
}

void CCOpenGLWidget::initShader()
{
    // 顶点着色器源代码
    const char* vs = "void main(){ \n"
                     "gl_Position = ftransform();\n"
                     "} \n\0";

    // 片段着色器源代码
    const char* fs = "void main(){ \n"
                     "gl_FragColor = vec4(0, 0, 1, 1);\n"
                     "}\n\0";

    // 创建顶点着色器和片段着色器
    vHandle = glCreateShader(GL_VERTEX_SHADER);
    pHandle = glCreateShader(GL_FRAGMENT_SHADER);

    // 设置着色器源代码
    glShaderSource(vHandle, 1, (const char**)&vs, 0);
    glShaderSource(pHandle, 1, (const char**)&fs, 0);

    // 编译顶点着色器
    glCompileShader(vHandle);
    GLint status = 0; // 编译状态
    char compileLog[1024] = {0}; // 编译日志
    glGetShaderiv(vHandle, GL_COMPILE_STATUS, &status); // 获取编译状态
    if (status == GL_FALSE) // 如果编译失败
    {
        glGetShaderInfoLog(vHandle, sizeof(compileLog), 0, compileLog); // 获取错误信息
        assert(false && compileLog); // 断言失败并输出错误信息
        uninitShader(); // 释放着色器资源
        return;
    }

    // 编译片段着色器
    glCompileShader(pHandle);
    glGetShaderiv(pHandle, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) // 如果编译失败
    {
        glGetShaderInfoLog(pHandle, sizeof(compileLog), 0, compileLog); // 获取错误信息
        assert(false && compileLog); // 断言失败并输出错误信息
        uninitShader(); // 释放着色器资源
        return;
    }

    // 创建着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vHandle); // 附加顶点着色器
    glAttachShader(shaderProgram, pHandle); // 附加片段着色器

    // 链接着色器程序
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status); // 获取链接状态
    if (status == GL_FALSE) // 如果链接失败
    {
        glGetShaderInfoLog(shaderProgram, sizeof(compileLog), 0, compileLog); // 获取错误信息
        assert(false && compileLog); // 断言失败并输出错误信息
        uninitShader(); // 释放着色器资源
        return;
    }
}

void CCOpenGLWidget::uninitShader()
{
    if (vHandle != -1) {
        glDeleteShader(vHandle); // 删除顶点着色器
    }
    if (pHandle != -1) {
        glDeleteShader(pHandle); // 删除片段着色器
    }
    if (shaderProgram != -1) {
        glDeleteProgram(shaderProgram); // 删除着色器程序
    }

    // 重置句柄
    vHandle = -1;
    pHandle = -1;
    shaderProgram = -1;
}

void CCOpenGLWidget::shaderBegin()
{
    glUseProgram(shaderProgram); // 使用着色器程序
}

void CCOpenGLWidget::shaderEnd()
{
    glUseProgram(0); // 禁用着色器程序
}
