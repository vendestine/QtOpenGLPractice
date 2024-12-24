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
    // OpenGL渲染的关键函数
    void initializeGL();     // 初始化OpenGL环境
    void paintGL();          // 绘制OpenGL场景
    void resizeGL(int w, int h);  // 调整窗口大小时重新设置视口

    // 鼠标事件处理函数
    void mousePressEvent(QMouseEvent* event);    // 鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event);     // 鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event);  // 鼠标释放事件

    // 更新像素内存
    void updatePixelsMemory(int width,int height);

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

    char*   m_pixels = NULL;  // 像素数据指针
};

#endif // CCOPENGLWIDGET_H
