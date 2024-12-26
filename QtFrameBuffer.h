#ifndef QTFRAMEBUFFER_H
#define QTFRAMEBUFFER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QOpenGLFunctions>
#ifdef CCOPENGL_PLATFORM_WIN32
#include <GL/glu.h>
#elif CCOPENGL_PLATFORM_MACX
#include <OpenGL/glu.h>
#else
#endif

class QtFrameBuffer : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    QtFrameBuffer(QWidget *parent = 0);
    ~QtFrameBuffer();

protected:
    void initializeGL(); // 初始化 OpenGL 环境
    void paintGL(); // 渲染 OpenGL 内容
    void resizeGL(int width, int height); // 处理窗口大小变化

private:
    GLuint m_frameBufferID; // 帧缓冲对象 ID
    GLuint m_renderBufferID; // 渲染缓冲对象 ID
    GLuint m_textureID; // 颜色纹理 ID
    GLuint m_picTexture; // 图片纹理 ID
    float m_rotAngle; // 旋转角度
    QTimer* m_renderTimer; // 渲染定时器

    // 顶点结构体
    struct CCVertex {
        float x, y, z; // 顶点坐标
        float u, v;    // 纹理坐标
    };

private slots:
    void        onRenderUpdate();
private:
    void renderTexture(GLuint textID); // 渲染纹理
    void renderTextureOnScreen(); // 在屏幕上渲染纹理
    void saveImage(); // 保存图像
    GLuint createImageTextures(QString &pathString); // 创建图像纹理
    void initFrameBufferObject(); // 初始化帧缓冲对象
};

#endif // QTFRAMEBUFFER_H
