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
    update();   // 请求重绘

}
void CCOpenGLWidget::initializeGL()
{
    GLenum err = glewInit(); // 初始化 GLEW
    if (GLEW_OK != err) {
        qDebug() << glewGetErrorString(err); // 输出错误信息
    }

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
    for (int i = 0; i < fileList.count(); i++) {
        QString textString = fileList.at(i);
        m_texture[i] = createOpenGLTexture(textString); // 创建纹理并存储
    }


}
void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置当前模型视图矩阵

    // 定义平面的顶点和纹理坐标
    CCVertex planeVertices[] =
    {
        {  -1.0f,-1.0f, 1.0f    ,0,  1},
        {  1.0f,-1.0f, 1.0f     ,1,  1},
        {  1.0f, 1.0f, 1.0f     ,1,  0},
        { -1.0f, 1.0f, 1.0f     ,0,  0},
    };

    // 激活并绑定第一个纹理
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,m_texture[4]);

    // 激活并绑定第二个纹理
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,m_texture[5]);

    // 启用顶点数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT,sizeof(CCVertex),planeVertices); // 设置顶点数组

    // 启用纹理坐标数组
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTextureARB(GL_TEXTURE0_ARB); // 激活第一组纹理坐标
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &planeVertices[0].u); // 设置第一组纹理坐标

    glClientActiveTextureARB(GL_TEXTURE1_ARB); // 激活第二组纹理坐标
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &planeVertices[0].u); // 设置第二组纹理坐标

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_SUBTRACT);  // 设置纹理环境模式为减法

    // 创建变换矩阵
    QMatrix4x4 cubeMat;
    QMatrix4x4 cubeMatTrans;
    QMatrix4x4 cubeMatScale;
    QMatrix4x4 cubeMatRot;

    // 设置平移、旋转和缩放
    cubeMatTrans.translate(0, 0, -8); // 平移
    cubeMatRot.rotate(m_angle, 0, 0, 0); // 旋转（目前未旋转）
    cubeMatScale.scale(2, 2, 2); // 缩放
    cubeMat = cubeMatTrans * cubeMatScale * cubeMatRot; // 组合变换矩阵

    glLoadMatrixf(cubeMat.data()); // 加载变换矩阵

    glDrawArrays(GL_QUADS, 0, sizeof(planeVertices) / sizeof(CCVertex)); // 绘制平面

    // 禁用顶点和纹理坐标数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // 设置视口

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    gluPerspective(60, (double)w / h, 0.1, 1000); // 设置透视投影

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
