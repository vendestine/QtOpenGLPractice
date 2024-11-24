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
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    // 自定义的3D浮点数结构体，包含位置(x,y,z)和颜色(r,g,b)信息
    // 这个结构体可以用于存储顶点的空间坐标和颜色属性
    struct CCFloat3{
        float x;  // x坐标
        float y;  // y坐标
        float z;  // z坐标
        float r;  // 红色分量
        float g;  // 绿色分量
        float b;  // 蓝色分量
    };
};

#endif // CCOPENGLWIDGET_H
