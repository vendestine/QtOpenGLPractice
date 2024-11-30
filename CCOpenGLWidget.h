#ifndef CCOPENGLWIDGET_H
#define CCOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QHoverEvent>
#include <vector>

class CCOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    CCOpenGLWidget(QWidget *parent = 0); // 构造函数
    ~CCOpenGLWidget(); // 析构函数

protected:
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL(); // 渲染 OpenGL 内容
    void resizeGL(int w, int h); // 处理窗口大小变化

    void mousePressEvent(QMouseEvent* event); // 处理鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event); // 处理鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event); // 处理鼠标释放事件

private:
    struct CCFloat3 { // 定义一个三维点结构体
        float x;
        float y;
        float z;
    };

    std::vector<CCFloat3> m_pointsArray; // 存储顶点数组

    bool m_bPressed = false; // 鼠标是否按下的状态
    int m_currentDragPointIndex = -1; // 当前正在拖动的点的索引
    QPoint m_lastMousePos; // 上一次鼠标位置
    bool m_isHoveringOverPoint = false; // 是否悬停在顶点上
};

#endif // CCOPENGLWIDGET_H
