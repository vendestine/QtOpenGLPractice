#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QOpenGLWidget>

class CCOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    CCOpenGLWidget(QWidget *parent = 0);
    ~CCOpenGLWidget();

protected:
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL(); // 绘制 OpenGL 内容
    void resizeGL(int w, int h); // 处理窗口大小变化

private:
    struct CCFloat3 { // 定义一个结构体，用于表示三维坐标
        float x; // x 坐标
        float y; // y 坐标
        float z; // z 坐标
    };
};

#endif // CCOPENGLWIDGET_H
