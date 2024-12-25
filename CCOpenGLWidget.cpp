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
    // 创建定时器
    m_pTimer = new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),SLOT(onRenderTimerShot()));
    m_pTimer->start(20);
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 释放定时器
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
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置清屏颜色为黑色
    glClearDepth(1.0); // 设置深度缓冲区的清除值
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 设置深度测试的函数

    glEnable(GL_TEXTURE_2D); // 启用 2D 纹理
    glGenTextures(1, &m_textureID); // 生成纹理 ID
    glBindTexture(GL_TEXTURE_2D, m_textureID); // 绑定纹理 ID

    // 加载纹理图像
    QImage texImage = QImage(":/Images/rabit.png");

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小过滤

    // 生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.rgbSwapped().bits());
    //glTexSubImage2D(GL_TEXTURE_2D,0,0,0,texImage.width(),texImage.height(),GL_RGBA,GL_UNSIGNED_BYTE,texImage.rgbSwapped().bits()); //更新纹理
}
void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲区
    glLoadIdentity(); // 重置模型视图矩阵

    // 定义平面顶点
    CCVertex plainVertices[] =
    {
        { -1.0f, 1.0f, 1.0f,    0,  0 }, // 左上角
        { -1.0f,-1.0f, 1.0f,    0,  1 }, // 左下角
        {  1.0f,-1.0f, 1.0f,    1,  1 }, // 右下角
        {  1.0f, 1.0f, 1.0f,    1,  0 }, // 右上角
    };


    glBindTexture(GL_TEXTURE_2D, m_textureID); // 绑定纹理

    m_angle += 1; // 增加旋转角度

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // 启用纹理坐标数组

    // 设置顶点指针和纹理坐标指针
    glVertexPointer(3,GL_FLOAT,sizeof(CCVertex),&plainVertices[0].x);
    glTexCoordPointer(2,GL_FLOAT,sizeof(CCVertex),&plainVertices[0].u);

    // 创建变换矩阵
    QMatrix4x4 cubeMat;
    QMatrix4x4 cubeMatTrans;
    QMatrix4x4 cubeMatScale;
    QMatrix4x4 cubeMatRot;

    cubeMatTrans.translate(0, 0, -8); // 平移
    cubeMatRot.rotate(m_angle, 0, 0, 1); // 旋转
    cubeMatScale.scale(2, 2, 2); // 缩放
    cubeMat = cubeMatTrans * cubeMatScale * cubeMatRot; // 组合变换

    glLoadMatrixf(cubeMat.data()); // 加载变换矩阵
    glDrawArrays(GL_QUADS, 0, sizeof(plainVertices) / sizeof(CCVertex)); // 绘制平面

    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // 禁用纹理坐标数组
    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组

}
void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // 设置视口

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    // 设置透视投影
    gluPerspective(60, (double)w / h, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW); // 切换回模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵
}
