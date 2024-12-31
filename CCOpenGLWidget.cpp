#include "CCOpenGLWidget.h"
#include <QDebug>

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    m_attrPosID = -1; // 初始化属性 ID
    m_attrColorID = -1; // 初始化颜色属性 ID
    m_uniMatID = -1; // 初始化矩阵 uniform ID

    m_pShaderProgram = NULL; // 初始化着色器程序指针
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 删除着色器程序
    if(m_pShaderProgram != NULL){
        delete m_pShaderProgram;
        m_pShaderProgram= NULL;
    }

    // 删除纹理
    glDeleteTextures(2,m_texture);

}
void CCOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // 初始化 OpenGL 函数

    glClearColor(0, 0, 0, 1); // 设置清除颜色为黑色
    glClearDepth(1.0); // 设置深度缓冲区的清除值
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 设置深度测试函数
    glEnable(GL_BLEND); // 启用混合

    // 加载纹理
    QString filePath6 = QString(":/Images/6.png");
    m_texture[0] = createImageTextures(filePath6); // 创建第一个纹理

    QString filePath5 = QString(":/Images/5.png");
    m_texture[1] = createImageTextures(filePath5); // 创建第二个纹理

    initializeGLSLShader(); // 初始化着色器

    int maxSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSize); // 获取最大纹理单元数
    qDebug() << "MAX TEXTURE SIZE:" << maxSize; // 打印最大纹理单元数
}

void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
    glLoadIdentity(); // 重置当前模型视图矩阵

    // 定义一个四个顶点的三角形带
    static CCVertex triangleVert[] = {
        {-1, 1,  1, 0, 0},
        {-1, -1,  1, 0, 1},
        {1,  1,  1, 1, 0},
        {1,  -1,  1, 1, 1},
    };

    QMatrix4x4 matrix; // 创建一个 4x4 矩阵
    matrix.perspective(60, (float)800 / (float)600, 0.1, 1000); // 设置透视投影
    matrix.translate(0, 0, -3); // 平移矩阵

#if 0
    m_pShaderProgram->bind(); // 绑定着色器程序

    GLuint matLocation = m_pShaderProgram->uniformLocation("uni_mat"); // 获取矩阵 uniform 的位置
    glUniformMatrix4fv(matLocation, 1, GL_FALSE, matrix.data()); // 设置矩阵 uniform

    GLuint textureLocation = m_pShaderProgram->uniformLocation("uni_texture"); // 获取纹理 uniform 的位置

    glEnableClientState(GL_VERTEX_ARRAY); // 启用顶点数组
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); // 启用纹理坐标数组

    glVertexPointer(3, GL_FLOAT, sizeof(CCVertex), triangleVert); // 设置顶点指针
    glTexCoordPointer(2, GL_FLOAT, sizeof(CCVertex), &triangleVert[0].u); // 设置纹理坐标指针

    // 绘制第一个纹理
    glActiveTexture(GL_TEXTURE0); // 激活纹理单元 0
    glBindTexture(GL_TEXTURE_2D, m_texture[0]); // 绑定第一个纹理
    glUniform1i(textureLocation, 0); // 设置纹理 uniform
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制三角形带

    glBlendFunc(GL_ONE, GL_ONE); // 设置混合函数
    // 绘制第二个纹理
    glActiveTexture(GL_TEXTURE1); // 激活纹理单元 1
    glBindTexture(GL_TEXTURE_2D, m_texture[1]); // 绑定第二个纹理
    glUniform1i(textureLocation, 1); // 设置纹理 uniform
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制三角形带

    glDisableClientState(GL_VERTEX_ARRAY); // 禁用顶点数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // 禁用纹理坐标数组

    m_pShaderProgram->release(); // 释放着色器程序
#endif

#if 1
    m_pShaderProgram->bind(); // 绑定着色器程序

    // 启用属性数组
    m_pShaderProgram->enableAttributeArray("attr_position"); // 启用顶点位置属性
    m_pShaderProgram->enableAttributeArray("attr_uv"); // 启用纹理坐标属性

    // 设置属性数组
    m_pShaderProgram->setUniformValue("uni_mat", matrix); // 设置矩阵 uniform
    m_pShaderProgram->setAttributeArray("attr_position", GL_FLOAT, triangleVert, 3, sizeof(CCVertex)); // 设置顶点位置属性数组
    m_pShaderProgram->setAttributeArray("attr_uv", GL_FLOAT, &triangleVert[0].u, 2, sizeof(CCVertex)); // 设置纹理坐标属性数组

    // 绘制第一个纹理
    m_pShaderProgram->setUniformValue("uni_texture", 0); // 设置纹理 uniform
    glActiveTexture(GL_TEXTURE0); // 激活纹理单元 0
    glBindTexture(GL_TEXTURE_2D, m_texture[0]); // 绑定第一个纹理
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制三角形带

    // 绘制第二个纹理（混合模式）
    glBlendFunc(GL_ONE, GL_ONE); // 设置混合函数
    m_pShaderProgram->setUniformValue("uni_texture", 1); // 设置纹理 uniform
    glActiveTexture(GL_TEXTURE1); // 激活纹理单元 1
    glBindTexture(GL_TEXTURE_2D, m_texture[1]); // 绑定第二个纹理
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 绘制三角形带

    // 禁用属性数组
    m_pShaderProgram->disableAttributeArray("attr_position"); // 禁用顶点位置属性
    m_pShaderProgram->disableAttributeArray("attr_uv"); // 禁用纹理坐标属性

    m_pShaderProgram->release(); // 释放着色器程序
#endif

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
    QOpenGLShader* vertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this); // 创建顶点着色器
    bool bCompileVS = vertexShader->compileSourceFile(":/Shaders/vertex.vert"); // 编译顶点着色器
    if (bCompileVS == false) {
        qDebug() << "VS Compile ERROR:" << vertexShader->log(); // 打印编译错误
    }

    // 创建并编译片段着色器
    QOpenGLShader* fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this); // 创建片段着色器
    bool bCompileFS = fragmentShader->compileSourceFile(":/Shaders/fragment.frag"); // 编译片段着色器
    if (bCompileFS == false) {
        qDebug() << "FS Compile ERROR:" << fragmentShader->log(); // 打印编译错误
    }

    // 创建着色器程序
    m_pShaderProgram = new QOpenGLShaderProgram(); // 创建着色器程序
    m_pShaderProgram->addShader(vertexShader); // 添加顶点着色器
    m_pShaderProgram->addShader(fragmentShader); // 添加片段着色器

    // 链接着色器程序
    bool linkStatus = m_pShaderProgram->link(); // 链接着色器程序
    if (linkStatus == false) {
        qDebug() << "LINK ERROR:" << m_pShaderProgram->log(); // 打印链接错误
    }

    // 清理着色器对象
    if (vertexShader != NULL) {
        delete vertexShader;
        vertexShader = NULL;
    }

    if (fragmentShader != NULL) {
        delete fragmentShader;
        fragmentShader = NULL;
    }

//    m_attrPosID = m_pShaderProgram->attributeLocation("attr_position");
//    m_attrColorID = m_pShaderProgram->attributeLocation("attr_color");
//    m_uniMatID = m_pShaderProgram->uniformLocation("uni_mat");

//    qDebug()<<"Pos Attr:"<<m_attrPosID <<"Color Attr:"<<m_attrColorID<<"mat uniform:"<<m_uniMatID;

}

GLuint CCOpenGLWidget::createImageTextures(QString &pathString)
{
    unsigned int textureId;
    glGenTextures(1, &textureId); // 生成纹理 ID
    glBindTexture(GL_TEXTURE_2D, textureId); // 绑定纹理 ID

    QImage texImage = QImage(pathString.toLocal8Bit().data()); // 加载纹理图像

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小过滤
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage.width(), texImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage.rgbSwapped().bits()); // 生成纹理

    return textureId; // 返回纹理 ID
}

void CCOpenGLWidget::printOpenGLError()
{
    GLenum err = glGetError(); // 获取 OpenGL 错误代码
    if (err != GL_NO_ERROR) {
        qDebug() << "glError:" << err; // 打印错误
    }
}
