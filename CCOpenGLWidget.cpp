#include "CCOpenGLWidget.h"
#include <QDebug>

#ifdef CCOPENGL_PLATFORM_WIN32
#include <GL/glu.h>
#elif CCOPENGL_PLATFORM_MACX
#include <OpenGL/glu.h>
#else
#endif

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_pTimer = new QTimer(); // 创建定时器对象
    connect(m_pTimer, SIGNAL(timeout()), SLOT(onRenderTimerShot())); // 捕获定时器超时信号
    m_pTimer->start(20); // 每20毫秒触发一次
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 清理定时器资源
    if(m_pTimer){
        m_pTimer->stop();
        delete m_pTimer;
        m_pTimer = NULL;
    }
}

void CCOpenGLWidget::onRenderTimerShot()
{
    update(); // 请求重绘

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

    // 定义立方体的顶点和颜色
    CCVertex cubeVert[] =
        {
            { -1.0f, -1.0f,  1.0f, 255, 0, 0, 255 }, // 前面
            {  1.0f, -1.0f,  1.0f, 255, 0, 0, 255 },
            {  1.0f,  1.0f,  1.0f, 255, 0, 0, 255 },
            { -1.0f,  1.0f,  1.0f, 255, 0, 0, 255 },

            { -1.0f, -1.0f, -1.0f, 255, 0, 255, 255 }, // 后面
            { -1.0f,  1.0f, -1.0f, 255, 0, 255, 255 },
            {  1.0f,  1.0f, -1.0f, 255, 0, 255, 255 },
            {  1.0f, -1.0f, -1.0f, 255, 0, 255, 255 },

            { -1.0f,  1.0f, -1.0f, 0, 0, 255, 255 }, // 上面
            { -1.0f,  1.0f,  1.0f, 0, 0, 255, 255 },
            {  1.0f,  1.0f,  1.0f, 0, 0, 255, 255 },
            {  1.0f,  1.0f, -1.0f, 0, 0, 255, 255 },

            { -1.0f, -1.0f, -1.0f, 0, 255, 255, 255 }, // 下面
            {  1.0f, -1.0f, -1.0f, 0, 255, 255, 255 },
            {  1.0f, -1.0f,  1.0f, 0, 255, 255, 255 },
            { -1.0f, -1.0f,  1.0f, 0, 255, 255, 255 },

            {  1.0f, -1.0f, -1.0f, 255, 255, 0, 255 }, // 右面
            {  1.0f,  1.0f, -1.0f, 255, 255, 0, 255 },
            {  1.0f,  1.0f,  1.0f, 255, 255, 0, 255 },
            {  1.0f, -1.0f,  1.0f, 255, 255, 0, 255 },

            { -1.0f, -1.0f, -1.0f, 255, 0, 255, 255 }, // 左面
            { -1.0f, -1.0f,  1.0f, 255, 0, 255, 255 },
            { -1.0f,  1.0f,  1.0f, 255, 0, 255, 255 },
            { -1.0f,  1.0f, -1.0f, 255, 0, 255, 255 }
        };

    // 将所有顶点的z坐标减去5，使立方体远离视点
    for (int i = 0; i < int(sizeof(cubeVert) / sizeof(CCVertex)); i++) {
        cubeVert[i].z -= 5;
    }

    m_angle += 1; // 增加旋转角度

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glEnableClientState(GL_COLOR_ARRAY); // 启用颜色数组

    // 指定顶点和颜色数组的指针
    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), cubeVert);
    glColorPointer(4, GL_FLOAT, sizeof(CCVertex), &cubeVert[0].r);

    // 1. 使用固定管线进行变换
    glLoadIdentity(); // 重置模型视图矩阵
    glTranslatef(3, 3, -3); // 平移
    glRotatef(m_angle, 0, 0, 1); // 旋转
    glScalef(2, 2, 2); // 缩放
    glDrawArrays(GL_QUADS, 0, sizeof(cubeVert) / sizeof(CCVertex)); // 绘制立方体

    // 2. 使用Qt的矩阵构建旋转缩放平移矩阵
    QMatrix4x4 cubeMat;
    QMatrix4x4 cubeMatTrans;
    QMatrix4x4 cubeMatScale;
    QMatrix4x4 cubeMatRot;

    cubeMatTrans.translate(-3, -3, -9); // 平移
    cubeMatRot.rotate(m_angle, 0, 0, 1); // 旋转
    cubeMatScale.scale(1, 1, 1); // 缩放

    // 组合矩阵
    cubeMat = cubeMatTrans * cubeMatScale * cubeMatRot;
    glLoadMatrixf(cubeMat.data()); // 加载矩阵
    glDrawArrays(GL_QUADS, 0, sizeof(cubeVert) / sizeof(CCVertex)); // 绘制立方体

    glDisableClientState(GL_COLOR_ARRAY); // 禁用颜色数组
    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组

}
void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // 设置视口大小

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    // 使用透视投影
    gluPerspective(60, (double)w / h, 0.1, 1000); // 视野角度、宽高比、近裁剪面、远裁剪面

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵
}
