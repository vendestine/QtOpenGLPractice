#include "CCOpenGLWidget.h"
#include <QDebug>

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    // 初始化属性 ID
    m_attrPosID = -1; // 初始化属性 ID
    m_attrColorID = -1; // 初始化颜色属性 ID
    m_uniMatID = -1; // 初始化矩阵 uniform ID

    m_pShaderProgram = NULL; // 初始化着色器程序指针

    // 创建渲染定时器
    m_pRenderTimer = new QTimer(); // 创建渲染定时器
    connect(m_pRenderTimer, SIGNAL(timeout()), SLOT(onRenderTimerUpdate())); // 连接定时器超时信号到槽函数
    m_pRenderTimer->start(30); // 每 30 毫秒触发一次定时器
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 删除着色器程序
    if(m_pShaderProgram != NULL){
        delete m_pShaderProgram;
        m_pShaderProgram= NULL;
    }

    // 停止并删除定时器
    if(m_pRenderTimer != NULL){
        m_pRenderTimer->stop();
        delete m_pRenderTimer;
        m_pRenderTimer = NULL;
    }

    // 删除纹理
    glDeleteTextures(2,m_texture);

}
void CCOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // 初始化 OpenGL 函数

    // 设置清除颜色和深度
    glClearColor(0, 0, 0, 1); // 清除颜色为黑色
    glClearDepth(1.0); // 清除深度为 1.0
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 设置深度测试函数
    glEnable(GL_BLEND); // 启用混合

    // 加载纹理
    QString filePath6 = QString(":/Images/6.png");
    m_texture[0] = createImageTextures(filePath6); // 创建第一个纹理

    QString filePath5 = QString(":/Images/5.png");
    m_texture[1] = createImageTextures(filePath5); // 创建第二个纹理

    initializeGLSLShader(); // 初始化着色器

    // 获取最大纹理单元数（可选）
    // int maxSize = 0;
    // glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSize);
    // qDebug() << "MAX TEXTURE SIZE:" << maxSize;
}

void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置当前模型视图矩阵

    // 定义四个顶点的三角形带
    static CCVertex triangleVert[] = {
        {-1, 1,  1, 0, 0}, // 左上
        {-1, -1,  1, 0, 1}, // 左下
        {1,  1,  1, 1, 0}, // 右上
        {1,  -1,  1, 1, 1}, // 右下
    };

    // 创建透视投影矩阵
    QMatrix4x4 matrix;
    matrix.perspective(60, (float)800 / (float)600, 0.1, 1000); // 设置透视投影
    matrix.translate(0, 0, -3); // 平移矩阵

    // 动态更新值
    m_nValue += 0.01f; // 更新动画值
    if (m_nValue > 1) {
        m_nValue = 0.0f; // 循环重置
    }

    // 绑定着色器程序
    m_pShaderProgram->bind();

    // 设置 uniform 变量
    m_pShaderProgram->setUniformValue("uni_mat", matrix); // 设置矩阵 uniform
    m_pShaderProgram->setUniformValue("uni_fValue", m_nValue); // 设置动态值 uniform

    // 启用顶点和纹理坐标属性
    m_pShaderProgram->enableAttributeArray("attr_position");
    m_pShaderProgram->enableAttributeArray("attr_uv");

    // 设置属性数组
    m_pShaderProgram->setAttributeArray("attr_position", GL_FLOAT, triangleVert, 3, sizeof(CCVertex)); // 设置顶点位置属性数组
    m_pShaderProgram->setAttributeArray("attr_uv", GL_FLOAT, &triangleVert[0].u, 2, sizeof(CCVertex)); // 设置纹理坐标属性数组

    // 绘制第一个纹理
    m_pShaderProgram->setUniformValue("uni_texture0", 0); // 设置纹理 uniform
    glActiveTexture(GL_TEXTURE0); // 激活纹理单元 0
    glBindTexture(GL_TEXTURE_2D, m_texture[0]); // 绑定第一个纹理
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制三角形带

    // 绘制第二个纹理
    m_pShaderProgram->setUniformValue("uni_texture1", 1); // 设置纹理 uniform
    glActiveTexture(GL_TEXTURE1); // 激活纹理单元 1
    glBindTexture(GL_TEXTURE_2D, m_texture[1]); // 绑定第二个纹理
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制三角形带

    // 禁用属性数组
    m_pShaderProgram->disableAttributeArray("attr_position");
    m_pShaderProgram->disableAttributeArray("attr_uv");

    // 释放着色器程序
    m_pShaderProgram->release();
}


void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // 设置视口

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵
}


void CCOpenGLWidget::initializeGLSLShader()
{
    // 创建并编译顶点着色器
    QOpenGLShader* vertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool bCompileVS = vertexShader->compileSourceFile(":/Shaders/vertex.vert"); // 编译顶点着色器
    if (bCompileVS == false) {
        qDebug() << "VS Compile ERROR:" << vertexShader->log(); // 打印编译错误
    }

    // 创建并编译片段着色器
    QOpenGLShader* fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    bool bCompileFS = fragmentShader->compileSourceFile(":/Shaders/fragment.frag"); // 编译片段着色器
    if (bCompileFS == false) {
        qDebug() << "FS Compile ERROR:" << fragmentShader->log(); // 打印编译错误
    }

    // 创建着色器程序
    m_pShaderProgram = new QOpenGLShaderProgram();
    m_pShaderProgram->addShader(vertexShader); // 添加顶点着色器
    m_pShaderProgram->addShader(fragmentShader); // 添加片段着色器

    // 链接着色器程序
    bool linkStatus = m_pShaderProgram->link();
    if (linkStatus == false) {
        qDebug() << "LINK ERROR:" << m_pShaderProgram->log(); // 打印链接错误
    }

    // 清理着色器对象
    if (vertexShader != NULL) {
        delete vertexShader; // 删除顶点着色器
        vertexShader = NULL; // 设置指针为 NULL
    }

    if (fragmentShader != NULL) {
        delete fragmentShader; // 删除片段着色器
        fragmentShader = NULL; // 设置指针为 NULL
    }

    // 可选: 获取属性和 uniform 的位置
    // m_attrPosID = m_pShaderProgram->attributeLocation("attr_position");
    // m_attrColorID = m_pShaderProgram->attributeLocation("attr_color");
    // m_uniMatID = m_pShaderProgram->uniformLocation("uni_mat");
}

GLuint CCOpenGLWidget::createImageTextures(QString &pathString)
{
    unsigned int textureId;
    glGenTextures(1, &textureId); // 生成纹理 ID
    glBindTexture(GL_TEXTURE_2D, textureId); // 绑定纹理 ID

    // 加载纹理图像
    QImage texImage = QImage(pathString.toLocal8Bit().data());

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小过滤
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.rgbSwapped().bits()); // 生成纹理

    return textureId; // 返回纹理 ID
}

void CCOpenGLWidget::printOpenGLError()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
         qDebug()<<"glError:"<<err;
    }
}

void CCOpenGLWidget::onRenderTimerUpdate()
{
    update();
}
