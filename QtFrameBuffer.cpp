#include "QtFrameBuffer.h"
#include <QMessageBox>
#include <QDebug>

QtFrameBuffer::QtFrameBuffer(QWidget *parent): QOpenGLWidget(parent)
{
    m_rotAngle = 0.0f;

    m_renderTimer = new QTimer();
    connect(m_renderTimer,SIGNAL(timeout()),SLOT(onRenderUpdate()));
    m_renderTimer->start();
}

QtFrameBuffer::~QtFrameBuffer()
{
    glDeleteTextures(1,&m_picTexture);
    if(m_renderTimer != NULL){
        m_renderTimer->stop();
        delete m_renderTimer;
        m_renderTimer = NULL;
    }
}

void QtFrameBuffer::initializeGL()
{
    initializeOpenGLFunctions(); // 初始化 OpenGL 函数

    glClearColor(0, 0, 0, 1); // 设置清屏颜色为黑色
    glClearDepth(1.0); // 设置深度缓冲区的清除值
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 设置深度测试的函数

    glEnable(GL_TEXTURE_2D); // 启用 2D 纹理

    QString filePath = QString(":/Images/rabit.png"); // 纹理文件路径
    m_picTexture = createImageTextures(filePath); // 创建纹理

    initFrameBufferObject(); // 初始化帧缓冲对象
}

void QtFrameBuffer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置清屏颜色

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID); // 绑定帧缓冲对象

    renderTexture(m_picTexture); // 渲染纹理到帧缓冲

    saveImage(); // 保存图像

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 解绑帧缓冲

    renderTextureOnScreen(); // 在屏幕上渲染纹理

}
void QtFrameBuffer::renderTexture(GLuint texID)
{
    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    gluPerspective(60, double(800) / double(600), 0.1, 1000); // 设置透视投影
    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵

    glClearColor(0, 0, 0, 1); // 设置清屏颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置模型视图矩阵

    glTranslatef(0.0f, 0.0f, -4); // 平移到 Z 轴负方向
    glRotated(m_rotAngle, 1, 1, 1); // 绕 (1, 1, 1) 轴旋转
    m_rotAngle += 3; // 更新旋转角度

    glEnable(GL_TEXTURE_2D); // 启用 2D 纹理
    glBindTexture(GL_TEXTURE_2D, texID); // 绑定纹理

    // 定义立方体的顶点和纹理坐标
    CCVertex g_cubeVertices[] =
    {
        { -1.0f,-1.0f, 1.0f,0.0f, 0.0f },
        { 1.0f,-1.0f, 1.0f,1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f,1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f,0.0f, 1.0f},

        { -1.0f,-1.0f,-1.0f,1.0f, 0.0f },
        { -1.0f, 1.0f,-1.0f,1.0f, 1.0f },
        { 1.0f, 1.0f,-1.0f,0.0f, 1.0f},
        { 1.0f,-1.0f,-1.0f,0.0f, 0.0f},

        { -1.0f, 1.0f,-1.0f,0.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f,0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f,1.0f, 0.0f },
        { 1.0f, 1.0f,-1.0f,1.0f, 1.0f },

        { -1.0f,-1.0f,-1.0f,1.0f, 1.0f },
        { 1.0f,-1.0f,-1.0f,0.0f, 1.0f },
        { 1.0f,-1.0f, 1.0f,0.0f, 0.0f },
        { -1.0f,-1.0f, 1.0f,1.0f, 0.0f },

        { 1.0f,-1.0f,-1.0f,1.0f, 0.0f },
        { 1.0f, 1.0f,-1.0f,1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,0.0f, 1.0f },
        { 1.0f,-1.0f, 1.0f,0.0f, 0.0f },

        { -1.0f,-1.0f,-1.0f,0.0f, 0.0f },
        { -1.0f,-1.0f, 1.0f,1.0f, 0.0f },
        { -1.0f, 1.0f, 1.0f,1.0f, 1.0f },
        { -1.0f, 1.0f,-1.0f,0.0f, 1.0f }
    };

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // 启用纹理坐标数组

    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), g_cubeVertices); // 设置顶点指针
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &g_cubeVertices[0].u); // 设置纹理坐标指针

    glDrawArrays(GL_QUADS, 0, 24); // 绘制立方体

    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // 禁用纹理坐标数组

    glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理
    glDisable(GL_TEXTURE_2D); // 禁用 2D 纹理
}


void QtFrameBuffer::renderTextureOnScreen()
{
    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵
    glOrtho(-1, 1, -1, 1, -100.0, 100.0); // 设置正交投影

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵

    glClearColor(0, 0, 0, 1); // 设置清屏颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置模型视图矩阵

    glTranslatef(0.0f, 0.0f, -2); // 平移
    glScalef(1, 1, 1); // 缩放

    glEnable(GL_TEXTURE_2D); // 启用 2D 纹理
    glBindTexture(GL_TEXTURE_2D, m_textureID); // 绑定纹理

    // 定义屏幕上渲染的四个顶点
    CCVertex g_cubeVertices[] =
    {
        { -1,1, 1, 0, 0 },
        { -1,-1, 1, 0, 1 },
        { 1,1, 1, 1,   0},
        { 1, -1, 1, 1, 1 },


    };

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // 启用纹理坐标数组

    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), g_cubeVertices); // 设置顶点指针
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &g_cubeVertices[0].u); // 设置纹理坐标指针

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制屏幕上的四个顶点

    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // 禁用纹理坐标数组

    glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理
    glDisable(GL_TEXTURE_2D); // 禁用 2D 纹理
}

GLuint QtFrameBuffer::createImageTextures(QString &pathString)
{
    unsigned int textureId;
    glGenTextures(1, &textureId); // 生成纹理 ID
    glBindTexture(GL_TEXTURE_2D, textureId); // 绑定纹理

    QImage texImage = QImage(pathString.toLocal8Bit().data()); // 加载纹理图像

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小过滤
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.rgbSwapped().bits()); // 生成纹理

    return textureId; // 返回纹理 ID
}

void QtFrameBuffer::saveImage()
{

    int w = 800; // 图像宽度
    int h = 600; // 图像高度
    int size = w * h * 4; // 图像数据大小
    unsigned char* data = new unsigned char[size]; // 创建存储图像数据的数组
    memset(data, 0, w * h * 4); // 清空数据

    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data); // 读取当前帧缓冲的像素数据

    QString tempPath = "/Users/ventu/Documents/qt_demo/qt-graph/qt-opengl/14/test.png"; // 保存路径

    QImage image = QImage(data, w, h, QImage::Format_RGB32); // 创建 QImage 对象
    if (image.isNull()) {
        qDebug() << "Receive frame error, width:" << w << ", height:" << h; // 输出错误信息
        return;
    }
    image = image.rgbSwapped(); // RGB 交换
    image.save(tempPath, "PNG", 100); // 保存图像
}

void QtFrameBuffer::initFrameBufferObject()
{
    glGenTextures(1, &m_textureID); // 生成纹理 ID
    glBindTexture(GL_TEXTURE_2D, m_textureID); // 绑定纹理
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // 设置纹理包裹模式
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // 设置纹理包裹模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 设置缩小过滤
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // 生成 Mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // 创建纹理
    glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理

    glGenRenderbuffers(1, &m_renderBufferID); // 生成渲染缓冲对象 ID
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID); // 绑定渲染缓冲
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600); // 设置渲染缓冲的存储格式
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // 解绑渲染缓冲

    glGenFramebuffers(1, &m_frameBufferID); // 生成帧缓冲对象 ID
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID); // 绑定帧缓冲
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0); // 将纹理附加到帧缓冲
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferID); // 将渲染缓冲附加到帧缓冲

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); // 检查帧缓冲状态
    QMessageBox msg; // 消息框
    switch (status) {
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        msg.setText("GL_FRAMEBUFFER_UNSUPPORTED_EXT!"); // 输出错误信息
        msg.exec(); // 显示消息框
        break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 解绑帧缓冲
}

void QtFrameBuffer::resizeGL(int width, int height)
{

    glViewport(0,0,width,height);   // 设置视口

}

void QtFrameBuffer::onRenderUpdate()
{
    update(); // 触发重绘
}


