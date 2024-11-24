#include "CCOpenGLWidget.h"

CCOpenGLWidget::CCOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

CCOpenGLWidget::~CCOpenGLWidget()
{

}
void CCOpenGLWidget::initializeGL()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);  // 设置清除颜色为黑色
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

}
void CCOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 定义第一个四边形的顶点数组
    CCFloat3 vertexRectangle[] = {
        {50,  50, 0, 1, 0, 0},
        {300, 50,  0, 0, 1, 0},
        {50,  300, 0, 0, 0, 1},
        {300, 300, 0, 1, 0, 0},
    };

    // 设置绘制颜色（这行实际上会被顶点颜色覆盖）
    glColor3f(128.0f,128.0f,0.0f);

    // 启用顶点数组和颜色数组
    glEnableClientState(GL_VERTEX_ARRAY); //表示启用顶点数组
    glEnableClientState(GL_COLOR_ARRAY);//启用颜色数组

    // 设置顶点指针
    // 参数：坐标维度，数据类型，步长，数据指针
    glVertexPointer(3, GL_FLOAT, sizeof(CCFloat3), vertexRectangle);

    // 设置颜色指针
    // 参数：颜色维度，数据类型，步长，颜色数据起始指针
    glColorPointer(3, GL_FLOAT, sizeof(CCFloat3), &vertexRectangle[0].r);

    // 绘制三角形带
    // 参数：绘制模式，起始索引，顶点数量
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // 定义另一个四边形的顶点数组
    CCFloat3 vertexRectangle1[]={
        {50,  50, 0, 1, 0, 0},
        {300, 50,  0, 0, 1, 0},
        {50,  300, 0, 0, 0, 1},
        {300, 300, 0, 1, 0, 0},
    };

    // 移动第二个四边形的位置
    for(unsigned int i=0; i< sizeof(vertexRectangle1)/sizeof(vertexRectangle1[0]); i++){
        vertexRectangle1[i].x += 300;  // 向右平移300像素
        vertexRectangle1[i].y += 100;  // 向下平移100像素
    }

    // 设置第二个四边形的顶点指针，颜色指针
    glVertexPointer(3,GL_FLOAT,sizeof(CCFloat3),vertexRectangle1);
    glColorPointer(3,GL_FLOAT,sizeof(CCFloat3),&vertexRectangle1[0].r);

    // 绘制第二个四边形
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    // 禁用顶点数组和颜色数组
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

}
void CCOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 正交投影（X轴从左到右 0->w, Y轴从上到下 h->0）
    // 这样opengl坐标系，就和qt的窗口坐标系一致了
    glOrtho(0,w,h,0,-100,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
