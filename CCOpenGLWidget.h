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
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL(); // 渲染 OpenGL 内容
    void resizeGL(int w, int h); // 处理窗口大小变化

private slots:
    void onRenderTimerShot();

private:
    // 创建 OpenGL 纹理
    GLuint createOpenGLTexture(QString& pathString);

private:
    struct CCVertex { // 定义一个结构体表示顶点
        float x; // x 坐标
        float y; // y 坐标
        float z; // z 坐标
        float u; // 纹理坐标 u
        float v; // 纹理坐标 v
    };

    QTimer* m_pTimer; // 定时器指针
    float m_angle = 0.0f; // 旋转角度
    float m_vValue = 0.0f; // 纹理坐标变化值
    GLuint m_texture[6]; // 存储六个纹理ID的数组
};

#endif // CCOPENGLWIDGET_H
