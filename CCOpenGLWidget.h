#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector2D>
#include <QOpenGLTexture>
#include <QImage>
#include <QKeyEvent>
#include <QTimer>
#include <QMatrix4x4>

#ifdef CCOPENGL_PLATFORM_WIN32
#include <GL/glu.h>
#elif CCOPENGL_PLATFORM_MACX
#include <OpenGL/glu.h>
#else
#endif

// 定义顶点结构体
struct CCVertex {
    float x, y, z; // 顶点坐标
    float u, v;    // 纹理坐标
};


class CCOpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CCOpenGLWidget(QWidget *parent = 0);
    virtual ~CCOpenGLWidget();

protected:
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL();      // 渲染 OpenGL 内容
    void resizeGL(int width, int height); // 处理窗口大小变化


private:
    GLuint m_texture;    // 纹理 ID
    GLuint m_vbo;        // 顶点缓冲对象 ID
    QTimer* m_RenderTimer; // 渲染定时器
    float m_rotAngle;    // 旋转角度

private slots:
    void    onRenderTimerUpdate();

private:
    // 创建立方体纹理
    unsigned int  createCubeTextures(QString &pathString);
};

#endif // CCOPENGLWIDGET_H
