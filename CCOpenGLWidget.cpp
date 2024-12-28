#include "CCOpenGLWidget.h"
#include <QDebug>

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    // 初始化着色器属性 ID 和着色器程序指针
    m_attrPosID     = -1;
    m_attrColorID   = -1;
    m_uniMatID      = -1;
    m_pShaderProgram = NULL;
}

CCOpenGLWidget::~CCOpenGLWidget()
{
    // 释放着色器程序
    if(m_pShaderProgram != NULL){
        delete m_pShaderProgram;
        m_pShaderProgram= NULL;
    }

}
void CCOpenGLWidget::initializeGL()
{
    // 初始化 OpenGL 函数
    initializeOpenGLFunctions();

    // 设置清除颜色和深度测试
    glClearColor(0, 0, 0, 1);   // 设置清屏颜色为黑色
    glClearDepth(1.0);          // 设置深度缓冲区
    glEnable(GL_DEPTH_TEST);    // 启用深度测试
    glDepthFunc(GL_LEQUAL);     // 设置深度测试函数

    initializeGLSLShader(); // 初始化着色器
}

void CCOpenGLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲区
    glLoadIdentity();  // 重置模型视图矩阵

    // 定义三角形顶点（位置和颜色）
    CCVertex6 triangleVert[] = {
        {0,     0.5,   1,  1, 0, 0},  // 顶点1：红色
        {-0.5, -0.5,   1,  0, 1, 0},  // 顶点2：绿色
        {0.5,  -0.5,   1,  0, 0, 1}   // 顶点3：蓝色
    };

    // 创建透视投影矩阵
    QMatrix4x4 matrix;
    matrix.perspective(60, (float)800 / (float)600, 0.1, 1000); // 视野角度、宽高比、近远裁剪面
    matrix.translate(0, 0, -3); // 向 z 轴负方向平移

    m_pShaderProgram->bind(); // 绑定着色器程序

#if 0  // 方法1 使用opengl原生api
    // 将一个 4x4 矩阵传递给着色器中的ID是m_uniMatID的uniform变量
    glUniformMatrix4fv(m_uniMatID,1,GL_FALSE,matrix.data());

    // 启用顶点属性数组
    glEnableVertexAttribArray(m_attrPosID);
    glEnableVertexAttribArray(m_attrColorID);

    // 设置顶点属性数组（位置，颜色）
    glVertexAttribPointer(m_attrPosID,3,GL_FLOAT,GL_FALSE,sizeof(CCVertex6),triangleVert);
    glVertexAttribPointer(m_attrColorID,3,GL_FLOAT,GL_FALSE,sizeof(CCVertex6),&triangleVert[0].r);

    glDrawArrays(GL_TRIANGLE_STRIP,0,3);

    // 禁用顶点属性数组（位置，颜色）
    glDisableVertexAttribArray(m_attrPosID);
    glDisableVertexAttribArray(m_attrColorID);
#endif

#if 0  // 方法2 使用QOpenGLShaderProgram 的部分 OpenGL 函数
    // 设置着色器中ID是m_uniMatID的uniform 变量
    m_pShaderProgram->setUniformValue(m_uniMatID,matrix);

    // 启用顶点属性数组
    m_pShaderProgram->enableAttributeArray(m_attrPosID);
    m_pShaderProgram->enableAttributeArray(m_attrColorID);

    // 设置顶点属性数组
    m_pShaderProgram->setAttributeArray(m_attrPosID,GL_FLOAT,triangleVert,3,sizeof(CCVertex6));
    m_pShaderProgram->setAttributeArray(m_attrColorID,GL_FLOAT,&triangleVert[0].r,3,sizeof(CCVertex6));

    // 绘制三角形
    glDrawArrays(GL_TRIANGLES,0,3);

    // 禁用顶点属性数组
    m_pShaderProgram->disableAttributeArray(m_attrPosID);
    m_pShaderProgram->disableAttributeArray(m_attrColorID);
#endif

#if 1 // 方法3 使用 QOpenGLShaderProgram 的字符串名称方法 （最常用）
    // 设置 uniform 矩阵
    m_pShaderProgram->setUniformValue("uni_mat", matrix);

    // 启用顶点属性数组
    m_pShaderProgram->enableAttributeArray("attr_position");
    m_pShaderProgram->enableAttributeArray("attr_color");

    // 设置顶点属性数组
    m_pShaderProgram->setAttributeArray("attr_position", GL_FLOAT, triangleVert, 3, sizeof(CCVertex6));
    m_pShaderProgram->setAttributeArray("attr_color", GL_FLOAT, &triangleVert[0].r, 3, sizeof(CCVertex6));

    // 绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 禁用顶点属性数组
    m_pShaderProgram->disableAttributeArray("attr_position");
    m_pShaderProgram->disableAttributeArray("attr_color");
#endif

    m_pShaderProgram->release(); // 释放着色器程序
}


void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // 设置视口大小

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵

    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
    glLoadIdentity(); // 重置投影矩阵
}


void CCOpenGLWidget::initializeGLSLShader()
{
    // 创建顶点着色器
    QOpenGLShader* vertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool bCompileVS = vertexShader->compileSourceFile(":/Shaders/vertex.vert"); // 编译顶点着色器
    if(bCompileVS == false){
        qDebug() << "VS Compile ERROR:" << vertexShader->log(); // 输出编译错误
    }

    // 创建片段着色器
    QOpenGLShader* fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    bool bCompileFS = fragmentShader->compileSourceFile(":/Shaders/fragment.frag"); // 编译片段着色器
    if(bCompileFS == false){
        qDebug() << "FS Compile ERROR:" << fragmentShader->log(); // 输出编译错误
    }

    // 创建着色器程序
    m_pShaderProgram = new QOpenGLShaderProgram();
    m_pShaderProgram->addShader(vertexShader); // 添加顶点着色器
    m_pShaderProgram->addShader(fragmentShader); // 添加片段着色器

    // 链接着色器程序
    bool linkStatus = m_pShaderProgram->link();
    if(linkStatus == false){
        qDebug() << "LINK ERROR:" << m_pShaderProgram->log(); // 输出链接错误
    }

    // 释放着色器
    if(vertexShader != NULL){
        delete vertexShader; // 删除顶点着色器
        vertexShader = NULL; // 将指针设为 NULL
    }

    if(fragmentShader != NULL){
        delete fragmentShader; // 删除片段着色器
        fragmentShader = NULL; // 将指针设为 NULL
    }

    // 方法1和2 需要手动获取属性和 uniform 的位置
    // m_attrPosID = m_pShaderProgram->attributeLocation("attr_position");
    // m_attrColorID = m_pShaderProgram->attributeLocation("attr_color");
    // m_uniMatID = m_pShaderProgram->uniformLocation("uni_mat");

    // qDebug()<<"Pos Attr:"<<m_attrPosID <<"Color Attr:"<<m_attrColorID<<"mat uniform:"<<m_uniMatID;

}

void CCOpenGLWidget::printOpenGLError()
{
    GLenum err = glGetError(); // 获取 OpenGL 错误代码
    if (err != GL_NO_ERROR)
    {
        qDebug() << "glError:" << err; // 输出错误代码
    }
}
