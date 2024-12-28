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
    void initializeGL();  // 初始化opengl上下文
    void paintGL();       // opengl渲染
    void resizeGL(int w, int h);  // 调整视口

private:
    void initializeGLSLShader();  // 初始化着色器
    void printOpenGLError();      // 检查 OpenGL 错误

private:
    // 着色器属性 ID 和着色器程序指针
    GLuint      m_attrPosID; // 顶点位置属性 ID
    GLuint      m_attrColorID; // 顶点颜色属性 ID
    GLuint      m_uniMatID; // 矩阵 uniform ID
    QOpenGLShaderProgram*   m_pShaderProgram; // 着色器程序指针

    // 顶点结构体
    struct CCVertex6 {
        float x, y, z; // 顶点坐标
        float r, g, b; // 顶点颜色
    };
};

#endif // CCOPENGLWIDGET_H
