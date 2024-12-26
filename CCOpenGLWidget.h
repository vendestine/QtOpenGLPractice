#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QMatrix4x4>
#ifdef CCOPENGL_PLATFORM_WIN32
#include <GL/glu.h>
#elif CCOPENGL_PLATFORM_MACX
#include <OpenGL/glu.h>
#else
#endif


class CCOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit CCOpenGLWidget(QWidget *parent = 0);
    virtual ~CCOpenGLWidget();

protected:
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL(); // 渲染 OpenGL 内容
    void resizeGL(int width, int height); // 处理窗口大小变化

private:
    GLuint create2DTexturesWithPath(QString &path); // 创建 2D 纹理
    void displayTexture(); // 显示纹理
    void displayCube(); // 显示立方体

private:
    struct CCVertex { // 定义一个结构体表示顶点
        float x, y, z; // 顶点坐标
        float u, v; // 纹理坐标
    };

    QTimer* m_renderTimer; // 渲染定时器
    float m_rotAngle; // 旋转角度
    GLuint m_textures[6]; // 存储六个纹理的数组
    GLuint m_cubeDisplay; // 立方体显示列表 ID
    GLuint m_texDisplay; // 纹理显示列表 ID

private slots:
    void onRenderTimerUpdate();
};

#endif // CCOPENGLWIDGET_H
