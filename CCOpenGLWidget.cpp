#include "CCOpenGLWidget.h"

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

CCOpenGLWidget::~CCOpenGLWidget()
{

}

void CCOpenGLWidget::initializeGL()
{
    // 设置清屏颜色为红色（RGBA）
    // 1.0f, 0.0f, 0.0f, 1.0f 分别代表红、绿、蓝、透明度
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    // 设置深度缓冲区的清除值为 1.0
    // 深度缓冲区用于存储每个像素的深度信息
    glClearDepth(1.0f);

    // 启用深度测试
    // 深度测试可以确保近处的物体遮挡远处的物体
    glEnable(GL_DEPTH_TEST);

    // 设置深度测试函数为 GL_LEQUAL
    // 表示如果新像素的深度小于或等于当前深度，则绘制该像素
    glDepthFunc(GL_LEQUAL);
}

void CCOpenGLWidget::paintGL()
{
    // 清除颜色缓冲区和深度缓冲区
    // GL_COLOR_BUFFER_BIT：清除颜色缓冲区
    // GL_DEPTH_BUFFER_BIT：清除深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 重置模型视图矩阵为单位矩阵
    // 这将重置所有之前的变换（平移、旋转、缩放）
    glLoadIdentity();
}

// OpenGL 窗口大小调整函数
void CCOpenGLWidget::resizeGL(int w, int h)
{
    // 设置视口
    // 将 OpenGL 渲染区域设置为整个窗口
    // 参数：x偏移、y偏移、宽度、高度
    glViewport(0, 0, w, h);

    // 切换到投影矩阵模式
    glMatrixMode(GL_PROJECTION);

    // 重置投影矩阵为单位矩阵
    glLoadIdentity();

    // 设置正交投影
    // 参数：左、右、上、下、近平面、远平面
    // 这里创建了一个对称的正交投影空间
    glOrtho(-1, 1, 1, -1, 0.1f, 1000.0f);

    // 切换回模型视图矩阵模式
    glMatrixMode(GL_MODELVIEW);

    // 重置模型视图矩阵为单位矩阵
    glLoadIdentity();
}
