#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QMatrix4x4>

class CCOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    CCOpenGLWidget(QWidget *parent = 0);
    ~CCOpenGLWidget();

protected:
    void initializeGL(); // 初始化 OpenGL
    void paintGL(); // 绘制 OpenGL
    void resizeGL(int w, int h); // 窗口大小调整

private slots:
    void onRenderTimerShot();

private:
    struct CCVertex { // 顶点结构体
        float x; // x 坐标
        float y; // y 坐标
        float z; // z 坐标
        float u; // 纹理坐标 u
        float v; // 纹理坐标 v
    };

    QTimer* m_pTimer; // 渲染定时器
    float m_angle = 0.0f; // 旋转角度
    GLuint m_textureID; // 纹理 ID

};

#endif // CCOPENGLWIDGET_H
