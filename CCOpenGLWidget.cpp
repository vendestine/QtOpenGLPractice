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
    // 设置定时器
    m_pTimer = new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),SLOT(onRenderTimerShot()));
    m_pTimer->start(20);

}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 释放定时器资源
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

    // 加载纹理文件
    QStringList fileList;
    fileList.append(":/Images/1.png");
    fileList.append(":/Images/2.png");
    fileList.append(":/Images/3.png");
    fileList.append(":/Images/4.png");
    fileList.append(":/Images/5.png");
    fileList.append(":/Images/6.png");

    // 创建纹理
    for(int i=0; i<fileList.count(); i++)
    {
        QString textString= fileList.at(i);
        m_texture[i] = createOpenGLTexture(textString);     // 创建纹理并存储
    }


}
void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置当前模型视图矩阵

    // 定义立方体的顶点和纹理坐标
    CCVertex cubeVertices[] =
    {
        {  -1.0f,-1.0f, 1.0f    ,0,  1},
        {  1.0f,-1.0f, 1.0f     ,1,  1},
        {  1.0f, 1.0f, 1.0f     ,1,  0},
        { -1.0f, 1.0f, 1.0f     ,0,  0},

        {  -1.0f,-1.0f,-1.0f    ,0,  1 },
        {  -1.0f, 1.0f,-1.0f    ,0,  0 },
        {  1.0f, 1.0f,-1.0f     ,1,  0 },
        {  1.0f,-1.0f,-1.0f     ,1,  1 },

        {  -1.0f, 1.0f,-1.0f    ,0,  1 },
        {  -1.0f, 1.0f, 1.0f    ,1,  1 },
        {  1.0f, 1.0f, 1.0f     ,1,  0 },
        {  1.0f, 1.0f,-1.0f     ,0,  0 },

        {  -1.0f,-1.0f,-1.0f    ,0,  1 },
        {  1.0f,-1.0f,-1.0f     ,1,  1 },
        {  1.0f,-1.0f, 1.0f     ,1,  0 },
        {  -1.0f,-1.0f, 1.0f    ,0,  0 },

        {  1.0f,-1.0f,-1.0f ,0,  1 },
        {  1.0f, 1.0f,-1.0f ,1,  1 },
        {  1.0f, 1.0f, 1.0f ,1,  0 },
        {  1.0f,-1.0f, 1.0f ,0,  0 },

        {  -1.0f,-1.0f,-1.0f ,0,  1 },
        {  -1.0f,-1.0f, 1.0f ,1,  1 },
        {  -1.0f, 1.0f, 1.0f ,1,  0 },
        {  -1.0f, 1.0f,-1.0f ,0,  0 },
    };


    m_angle += 1; // 增加旋转角度
    m_vValue += 0.01f; // 增加纹理坐标偏移量

    // 更新纹理坐标
    for (int i = 0; i < 24; i++) {
        cubeVertices[i].u -= m_vValue; // 纹理坐标 u 递减
        cubeVertices[i].v += m_vValue; // 纹理坐标 v 递增
    }

    // 启用顶点和纹理坐标数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // 设置顶点和纹理坐标指针
    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), cubeVertices);
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &cubeVertices[0].u);

    // 创建变换矩阵
    QMatrix4x4 cubeMat;
    QMatrix4x4 cubeMatTrans;
    QMatrix4x4 cubeMatScale;
    QMatrix4x4 cubeMatRot;

    // 设置平移、旋转和缩放
    cubeMatTrans.translate(0, 0, -8); // 平移
    cubeMatRot.rotate(m_angle, 1, 1, 1); // 旋转
    cubeMatScale.scale(2, 2, 2); // 缩放
    cubeMat = cubeMatTrans * cubeMatScale * cubeMatRot; // 组合变换矩阵

    glLoadMatrixf(cubeMat.data()); // 加载变换矩阵

    // 绘制立方体的每一面
    for (int i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, m_texture[i]); // 绑定纹理
        glDrawArrays(GL_QUADS, i * 4, 4); // 绘制四边形
    }

    // 禁用顶点和纹理坐标数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

}
void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // 设置视口

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    // 设置透视投影
    gluPerspective(60, (double)w / h, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵
}

GLuint CCOpenGLWidget::createOpenGLTexture(QString& pathString)
{
    GLuint textureID;
    glEnable(GL_TEXTURE_2D); // 启用 2D 纹理
    glGenTextures(1, &textureID); // 生成纹理 ID
    glBindTexture(GL_TEXTURE_2D, textureID); // 绑定纹理 ID

    QImage texImage = QImage(pathString.toLocal8Bit().data()); // 加载纹理图像

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小过滤
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.rgbSwapped().bits()); // 生成纹理

    return textureID; // 返回纹理 ID
}
