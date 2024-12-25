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
    // OpenGL渲染的三个核心函数
    void initializeGL();     // 初始化OpenGL环境
    void paintGL();          // 绘制场景
    void resizeGL(int w, int h);  // 窗口大小调整时重新设置视口

private slots:
    void onRenderTimerShot();

private:
    // 创建 OpenGL 纹理
    GLuint createOpenGLTexture(QString& pathString);

private:
    struct CCVertex{
        // 3D坐标
        float x;
        float y;
        float z;
        // 纹理坐标
        float u;
        float v;
    };

    QTimer* m_pTimer; // 定时器指针
    float m_angle = 0.0f; // 旋转角度
    GLuint m_texture[6]; // 存储六个纹理ID的数组
};

#endif // CCOPENGLWIDGET_H
