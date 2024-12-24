#include "CCOpenGLWidget.h"
#include <QPoint>
#include <QDebug>
#include <random>

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    // 定义初始四边形的顶点坐标
    CCFloat3 vertexTriangle[]={
        {50,  50,  0},   // 左上角
        {300, 50,  0},   // 右上角
        {300, 300, 0},   // 右下角
        {50,  300, 0},   // 左下角
    };

    // 将顶点坐标添加到点数组中
    for(int i=0; i<(int)(sizeof(vertexTriangle)/sizeof(vertexTriangle[0])); i++)
    {
        CCFloat3 pos = vertexTriangle[i];
        m_pointsArray.push_back(pos);
    }

    // 开启鼠标追踪，使mouseMoveEvent能够持续响应
    // 即使没有按下鼠标按键也能捕获鼠标移动事件
    setMouseTracking(true);
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 释放动态分配的像素内存
    if(m_pixels != NULL){
        delete []m_pixels;
        m_pixels = NULL;
    }
}

void CCOpenGLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // 设置清屏颜色为黑色
    glClearDepth(1.0f);                    // 设置深度缓冲区
    glEnable(GL_DEPTH_TEST);               // 启用深度测试
    glDepthFunc(GL_LEQUAL);                // 设置深度测试函数
}

void CCOpenGLWidget::paintGL()
{
    // 清除颜色和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();  // 重置模型视图矩阵

    glColor3f(1.0f, 1.0f, 1.0f);  // 设置绘制颜色为白色

    // 计算像素区域的起始点和尺寸
    int startX = m_pointsArray[3].x;
    int startY = m_pointsArray[3].y;
    int width = m_pointsArray[2].x - m_pointsArray[0].x;
    int height = m_pointsArray[2].y - m_pointsArray[0].y;

    // 更新像素内存（随机生成像素）
    updatePixelsMemory(width, height);

    // 设置光栅位置并绘制像素
    glRasterPos2d(startX, startY);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, m_pixels);

    // 启用顶点数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(CCFloat3), &m_pointsArray[0]); // 指定顶点数组

    // 启用混合和抗锯齿
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置混合函数

     // 绘制线段（四边形轮廓）
    glLineWidth(3);  // 设置线宽
    glEnable(GL_LINE_SMOOTH);  // 启用线平滑
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // 设置线平滑的提示
    glDrawArrays(GL_LINE_LOOP, 0, 4);  // 绘制四边形轮廓

    // 绘制顶点
    glEnable(GL_POINT_SMOOTH); // 启用点平滑
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // 设置点平滑的提示
    glEnable(GL_POLYGON_SMOOTH); // 启用多边形平滑
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); // 设置多边形平滑的提示
    glPointSize(12);  // 设置点的大小
    glDrawArrays(GL_POINTS,0,4);  // 绘制点

    glDisableClientState(GL_VERTEX_ARRAY);  // 禁用顶点数组
}

void CCOpenGLWidget::resizeGL(int w, int h) {
    // 设置视口
    glViewport(0, 0, w, h);

    // 切换到投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // 重置投影矩阵

    // 设置正交投影, 将OpenGL坐标系映射到窗口坐标系
    glOrtho(0, w, h, 0, -100, 100);

    // 切换回模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // 重置模型视图矩阵
}

void CCOpenGLWidget::updatePixelsMemory(int width, int height)
{
    // 只在尺寸变化时重新分配内存
    static int lastWidth = 0;
    static int lastHeight = 0;

    if (width != lastWidth || height != lastHeight) {
        // 如果已有像素数据，先释放
        if(m_pixels != NULL){
            delete[] m_pixels;
            m_pixels = NULL;
        }

        // 分配新的像素内存
        m_pixels = new char[width * height * 3];

        // 使用更安全的随机数生成
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        // 随机填充像素数据
        for(int i = 0; i < width * height * 3; i++){
            m_pixels[i] = dis(gen);
        }

        // 更新最后的宽度和高度
        lastWidth = width;
        lastHeight = height;
    }
}

void CCOpenGLWidget::mousePressEvent(QMouseEvent* event) {
    // 处理鼠标左键按下事件
    if(event->button() == Qt::LeftButton) {
        // 记录鼠标初始位置
        m_lastMousePos = event->pos();

        // 找到被点击的点

        // 遍历所有顶点，检查是否点击在某个顶点的可交互区域内
        for(size_t i = 0; i < m_pointsArray.size(); i++) {
            // 计算顶点的可交互区域（20x20像素的矩形）
            float tx = m_pointsArray[i].x - 10;
            float ty = m_pointsArray[i].y - 10;
            QRect pointRect = QRect(tx, ty, 20, 20);

            // 如果鼠标点击在某个顶点的区域内
            if(pointRect.contains(m_lastMousePos)) {
                m_currentDragPointIndex = i;  // 记录当前拖动的点的索引
                m_bPressed = true; // 设置按下状态
                break;
            }
        }
    }
}

void CCOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint mousePos = event->pos(); // 获取当前鼠标位置
    m_isHoveringOverPoint = false; // 重置悬停状态

    // 检查是否悬停在顶点上
    for(size_t i = 0; i < m_pointsArray.size(); i++) {
        float tx = m_pointsArray[i].x - 10;
        float ty = m_pointsArray[i].y - 10;
        QRect pointRect = QRect(tx, ty, 20, 20);

        // 检查是否悬停在顶点上
        if(pointRect.contains(mousePos)) {
            m_isHoveringOverPoint = true;

            // 仅在没有按下鼠标时设置光标
            if (!m_bPressed) {
                // 根据顶点位置设置不同的光标样式
                if(i == 0 || i == 2) setCursor(Qt::SizeFDiagCursor);
                if(i == 1 || i == 3) setCursor(Qt::SizeBDiagCursor);
            }
            break;
        }
    }

    // 如果没有悬停在顶点上，恢复默认光标
    if (!m_isHoveringOverPoint) {
        setCursor(Qt::ArrowCursor);
    }

    // 拖动逻辑

    // 如果没有按下鼠标或没有选中拖动点，直接返回
    if (!m_bPressed || m_currentDragPointIndex == -1) return;

    // 获取当前拖动点的索引
    int i = m_currentDragPointIndex;

    // 复杂的顶点联动调整逻辑
    // 拖动一个顶点时，会联动调整相邻顶点的坐标
    if(i == 0){
        m_pointsArray[1].y = mousePos.y(); // 更新右上角的y坐标
        m_pointsArray[3].x = mousePos.x(); // 更新左下角的x坐标
    }
    if(i == 1){
        m_pointsArray[0].y = mousePos.y(); // 更新左上角的y坐标
        m_pointsArray[2].x = mousePos.x(); // 更新右下角的x坐标
    }
    if(i == 2){
        m_pointsArray[1].x = mousePos.x(); // 更新右上角的x坐标
        m_pointsArray[3].y = mousePos.y(); // 更新左下角的y坐标
    }
    if(i == 3){
        m_pointsArray[0].x = mousePos.x(); // 更新左上角的x坐标
        m_pointsArray[2].y = mousePos.y(); // 更新右下角的y坐标
    }

    // 更新当前被拖动点的坐标
    m_pointsArray[i].x = mousePos.x();
    m_pointsArray[i].y = mousePos.y();

    // 记录最后的鼠标位置，用于后续增量计算
    m_lastMousePos = mousePos;

    // 触发重绘窗口
    update();
}

void CCOpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    m_bPressed = false;  // 重置鼠标按下状态
    m_currentDragPointIndex = -1;  // 重置当前拖动点索引
    setCursor(Qt::ArrowCursor);  // 恢复默认光标
    m_isHoveringOverPoint = false;  // 重置悬停状态
}




