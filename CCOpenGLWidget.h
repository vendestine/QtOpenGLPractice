#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class CCOpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    CCOpenGLWidget(QWidget *parent = 0);
    ~CCOpenGLWidget();

protected:
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL(); // 渲染 OpenGL 内容
    void resizeGL(int w, int h); // 处理窗口大小变化

private:
    void initializeGLSLShader(); // 初始化 GLSL 着色器
    void printOpenGLError(); // 打印 OpenGL 错误
    GLuint createImageTextures(QString &pathString); // 创建纹理

private:
    GLuint m_attrPosID; // 顶点位置属性 ID
    GLuint m_attrColorID; // 顶点颜色属性 ID
    GLuint m_uniMatID; // 矩阵 uniform ID

    GLuint m_texture[2]; // 存储两个纹理的数组

    QOpenGLShaderProgram* m_pShaderProgram; // 着色器程序指针

    // 定义一个结构体来表示顶点
    struct CCVertex {
        float x, y, z; // 顶点坐标
        float u, v; // 纹理坐标
    };
};

#endif // CCOPENGLWIDGET_H
