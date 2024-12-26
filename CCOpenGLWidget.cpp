#include "CCOpenGLWidget.h"
#include <QImage>
#include <QDebug>

// 定义立方体的顶点和纹理坐标
CCVertex g_cubeVertices[] =
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

    {  1.0f,-1.0f,-1.0f     ,0,  1 },
    {  1.0f, 1.0f,-1.0f     ,1,  1 },
    {  1.0f, 1.0f, 1.0f     ,1,  0 },
    {  1.0f,-1.0f, 1.0f     ,0,  0 },

    {  -1.0f,-1.0f,-1.0f    ,0,  1 },
    {  -1.0f,-1.0f, 1.0f    ,1,  1 },
    {  -1.0f, 1.0f, 1.0f    ,1,  0 },
    {  -1.0f, 1.0f,-1.0f    ,0,  0 },
};



CCOpenGLWidget::CCOpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    // 设置定时器
    m_RenderTimer = new QTimer();
    connect(m_RenderTimer,SIGNAL(timeout()),SLOT(onRenderTimerUpdate()));
    m_RenderTimer->start(30);

    m_rotAngle=0;   // 初始化旋转角度
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 删除定时器
    if(m_RenderTimer!=NULL){
        m_RenderTimer->stop();
        delete m_RenderTimer;
        m_RenderTimer=NULL;
    }

    glDeleteBuffers(1, &m_vbo); // 删除顶点缓冲对象
    glDeleteTextures(1, &m_texture); // 删除纹理
}
void CCOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // 初始化 OpenGL 函数

    glClearColor(0, 0, 0, 1); // 设置清屏颜色为黑色
    glClearDepth(1.0f); // 设置深度缓冲区的清除值
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 设置深度测试的函数

    glEnable(GL_TEXTURE_2D); // 启用 2D 纹理

    // 加载纹理
    QString filePath = QString(":/Images/5.png");
    m_texture = createCubeTextures(filePath); // 创建立方体纹理

    // 创建顶点缓冲对象
    glGenBuffers(1, &m_vbo); // 生成一个 VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // 绑定 VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_cubeVertices), g_cubeVertices, GL_STATIC_DRAW); // 将顶点数据复制到 VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑 VBO
}

unsigned int CCOpenGLWidget::createCubeTextures(QString &pathString)
{
    unsigned int textureId; // 纹理 ID
    glGenTextures(1, &textureId); // 生成纹理 ID
    glBindTexture(GL_TEXTURE_2D, textureId); // 绑定纹理 ID

    QImage texImage = QImage(pathString.toLocal8Bit().data()); // 加载图像

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置缩小过滤

    // 上传纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.rgbSwapped().bits());

    return textureId; // 返回纹理 ID
}

void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置当前模型视图矩阵

    // 进行平移和旋转变换
    glTranslatef(0.0f, 0.0f, -5); // 向 Z 轴负方向平移
    glRotatef(30, 1, 1, 1); // 绕 (1, 1, 1) 轴旋转 30 度

    glBindTexture(GL_TEXTURE_2D, m_texture); // 绑定纹理

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // 启用纹理坐标数组

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // 绑定 VBO

    float* addrVertex = (float*)0; // 顶点地址
    float* uvAddress = (float*)12; // 纹理坐标地址

    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), addrVertex); // 设置顶点指针
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), uvAddress); // 设置纹理坐标指针

    glDrawArrays(GL_QUADS, 0, 24); // 绘制立方体的 24 个顶点

    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // 禁用纹理坐标数组
}


void CCOpenGLWidget::resizeGL(int width,int height)
{
    glViewport(0, 0, width, height); // 设置视口
    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵

    // 设置透视投影
    gluPerspective(60, double(width) / double(height), 0.1, 1000);

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵

}

void CCOpenGLWidget::onRenderTimerUpdate()
{
    update();  // 触发重绘
}


