#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>


class CCOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CCOpenGLWidget(QWidget *parent = 0);
    virtual ~CCOpenGLWidget();

protected:
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL(); // 渲染 OpenGL 内容
    void resizeGL(int width, int height); // 处理窗口大小变化
    void initShader(); // 初始化着色器
    void uninitShader(); // 释放着色器资源
    void shaderBegin(); // 开始使用着色器
    void shaderEnd(); // 结束使用着色器

private:
    GLuint vHandle; // 顶点着色器句柄
    GLuint pHandle; // 片段着色器句柄
    GLuint shaderProgram; // 着色器程序句柄

    // 定义顶点结构体
    struct CCVertex {
        float x, y, z; // 顶点坐标
        float r, g, b; // 顶点颜色
    };
};

#endif // CCOPENGLWIDGET_H
