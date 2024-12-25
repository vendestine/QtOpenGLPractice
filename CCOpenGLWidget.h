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
    void initializeGL(); // 初始化OpenGL环境
    void paintGL(); // 绘制OpenGL内容
    void resizeGL(int w, int h); // 处理窗口大小变化

private slots:
    void onRenderTimerShot();

private:
    struct CCVertex { // 自定义顶点结构体，包含位置和颜色信息
        float x; // x坐标
        float y; // y坐标
        float z; // z坐标
        float r; // 红色分量
        float g; // 绿色分量
        float b; // 蓝色分量
        float a; // 透明度
    };

    QTimer* m_pTimer; // 定时器指针
    float m_angle = 0.0f; // 立方体旋转角度

};

#endif // CCOPENGLWIDGET_H
