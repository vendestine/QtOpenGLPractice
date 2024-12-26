#include "CCOpenGLWidget.h"

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    // 设置定时器
    m_renderTimer = new QTimer();
    connect(m_renderTimer,SIGNAL(timeout()),SLOT(onRenderTimerUpdate()));
    m_renderTimer->start(30);

    m_rotAngle = 0.0f;  // 初始旋转角度为0
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 删除定时器
    if(m_renderTimer != NULL){
        m_renderTimer->stop();
        delete m_renderTimer;
        m_renderTimer=NULL;
    }

    // 删除纹理和显示列表
    glDeleteTextures(6, m_textures); // 删除纹理

    glDeleteLists(m_cubeDisplay, 1); // 删除立方体显示列表
    glDeleteLists(m_texDisplay, 1); // 删除纹理显示列表
}

void CCOpenGLWidget::initializeGL()
{
    glClearColor(0, 0, 0, 1); // 设置清屏颜色为黑色
    glClearDepth(1.0); // 设置深度缓冲区的清除值
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 设置深度测试的函数

    glEnable(GL_BLEND); // 启用混合
    glEnable(GL_TEXTURE_2D); // 启用 2D 纹理

    // 加载纹理
    QString texPath = QString(":/Images/rabit.png");
    m_textures[0] = create2DTexturesWithPath(texPath);

    QString texPath1 = QString(":/Images/1.png");
    m_textures[1] = create2DTexturesWithPath(texPath1);

    QString texPath2 = QString(":/Images/2.png");
    m_textures[2] = create2DTexturesWithPath(texPath2);

    QString texPath3 = QString(":/Images/3.png");
    m_textures[3] = create2DTexturesWithPath(texPath3);

    QString texPath4 = QString(":/Images/4.png");
    m_textures[4] = create2DTexturesWithPath(texPath4);

    QString texPath5 = QString(":/Images/5.png");
    m_textures[5] = create2DTexturesWithPath(texPath5);

    // 创建立方体显示列表
    m_cubeDisplay = 1; // 立方体显示列表 ID
    glNewList(m_cubeDisplay, GL_COMPILE); // 开始编译显示列表
    displayCube(); // 绘制立方体
    glEndList(); // 结束显示列表编译

    // 创建纹理显示列表
    m_texDisplay = 2; // 纹理显示列表 ID
    glNewList(m_texDisplay, GL_COMPILE); // 开始编译显示列表
    displayTexture(); // 绘制纹理
    glEndList(); // 结束显示列表编译
}

void CCOpenGLWidget::displayTexture()
{

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵
    glOrtho(-1, 1, -1, 1, -100, 100); // 设置正交投影

    // 定义四个顶点和纹理坐标
    CCVertex vert[] = {
       { -1, -1, 1, 0.0f, 1.0f },
       { -1, 1, 1, 0.0f, 0.0f },
       { 1, 1, 1, 1.0f, 0.0f },
       { 1, -1, 1, 1.0f, 1.0f },
    };

    glDisable(GL_DEPTH_TEST); // 禁用深度测试
    glBlendFunc(GL_DST_COLOR, GL_ZERO); // 设置混合函数
    glEnable(GL_BLEND); // 启用混合

    glBindTexture(GL_TEXTURE_2D, m_textures[2]); // 绑定纹理

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), &vert[0].x); // 设置顶点指针

    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // 启用纹理坐标数组
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &vert[0].u); // 设置纹理坐标指针

    glDrawArrays(GL_QUADS, 0, 4); // 绘制四边形

    glBlendFunc(GL_ONE, GL_ONE); // 设置混合函数为源与目标颜色相加
    glBindTexture(GL_TEXTURE_2D, m_textures[5]); // 绑定另一个纹理
    glDrawArrays(GL_QUADS, 0, 4); // 再次绘制四边形

    glDisable(GL_BLEND); // 禁用混合
    glEnable(GL_DEPTH_TEST); // 恢复深度测试
}

void CCOpenGLWidget::displayCube()
{
    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵
    gluPerspective(60, (double)10 / (double)8, 0.1, 1000); // 设置透视投影

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵

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


    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // 启用纹理坐标数组
    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), &cubeVertices[0].x); // 设置顶点指针
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &cubeVertices[0].u); // 设置纹理坐标指针

    m_rotAngle += 30; // 增加旋转角度

    // 创建变换矩阵
    QMatrix4x4 cubeMatRot;
    QMatrix4x4 cubeMatTrans;
    QMatrix4x4 cubeMatScale;
    QMatrix4x4 cubeMat;

    cubeMatRot.rotate(m_rotAngle, 1, 1, 1); // 旋转矩阵
    cubeMatTrans.translate(0, 0, -5); // 平移矩阵
    cubeMatScale.scale(1, 1, 1); // 缩放矩阵

    cubeMat = cubeMatTrans * cubeMatScale * cubeMatRot; // 组合变换矩阵

    glPushMatrix(); // 保存当前矩阵状态
    glLoadIdentity(); // 重置矩阵
    glLoadMatrixf(cubeMat.data()); // 加载变换矩阵

    // 绘制立方体的每一面
    for (int i = 0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]); // 绑定当前纹理
        glDrawArrays(GL_QUADS, i * 4, 4); // 绘制四边形
    }

    glPopMatrix(); // 恢复之前的矩阵状态
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // 禁用纹理坐标数组
    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
}

void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置当前模型视图矩阵

    glCallList(m_cubeDisplay); // 调用立方体显示列表
    glCallList(m_texDisplay); // 调用纹理显示列表
}

void CCOpenGLWidget::resizeGL(int width,int height)
{
    // 设置视口
    glViewport(0,0,width,height);
}


void CCOpenGLWidget::onRenderTimerUpdate()
{
    update();
}

GLuint CCOpenGLWidget::create2DTexturesWithPath(QString &path)
{
    GLuint texId; // 纹理 ID
    glGenTextures(1, &texId); // 生成纹理 ID

    QImage texImage = QImage(path); // 加载纹理图像
    glBindTexture(GL_TEXTURE_2D, texId); // 绑定纹理 ID

    // 设置纹理参数
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置缩小过滤
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置放大过滤
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置 S 方向纹理重复方式
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置 T 方向纹理重复方式

    // 生成纹理图像
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.rgbSwapped().bits());

    return texId; // 返回纹理 ID
}
