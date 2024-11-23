#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>    // 引入 QOpenGLWidget 类，提供 OpenGL 渲染功能

class CCOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    CCOpenGLWidget(QWidget *parent = 0);
    ~CCOpenGLWidget();

protected:
    // 初始化 OpenGL 环境
    // 通常在 OpenGL 上下文创建后调用。可以在这里设置 OpenGL 的状态（如清除颜色、启用深度测试等）
    void initializeGL();

    // 绘制 OpenGL 场景
    // 每次需要重绘 OpenGL 场景时调用，在这里进行实际的 OpenGL 渲染操作
    void paintGL();

    // 调整 OpenGL 视口大小
    // 用于调整视口和投影矩阵，保证渲染正确，w 和 h 分别是新的部件宽度和高度
    void resizeGL(int w, int h);
};

#endif // CCOPENGLWIDGET_H
