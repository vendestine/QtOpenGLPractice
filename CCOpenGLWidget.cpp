#include "CCOpenGLWidget.h"
#ifdef CCOPENGL_PLATFORM_WIN32  // Windows 平台下引入 GLU 库
#include <GL/glu.h>
#elif CCOPENGL_PLATFORM_MACX    // macOS 平台下引入 GLU 库
#include <OpenGL/glu.h>
#else
#endif

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

CCOpenGLWidget::~CCOpenGLWidget()
{

}
void CCOpenGLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置清除颜色为黑色
    glClearDepth(1.0); // 设置深度缓冲区的清除值
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 设置深度测试的函数
}
void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲区
    glLoadIdentity(); // 重置当前模型视图矩阵

    // 定义一个三角形的三个顶点
    CCFloat3 vertex[] = {
        {0.5f, 0, -0.5f}, // 第一个顶点
        {0.2f, 0.3f, -0.5f}, // 第二个顶点
        {0.8f, 0.3f, -0.5f}, // 第三个顶点
    };

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组

    // 指定顶点数组的格式
    glVertexPointer(3, GL_FLOAT, sizeof(CCFloat3), &vertex[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形

    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
}
void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // 设置视口大小

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    // glOrtho(-1,1,1,-1,-100,100);
    // 设置透视投影
    gluPerspective(120, (double)w / h, 0.1, 1000); // 视野角度、宽高比、近裁剪面、远裁剪面

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵
}
