#version 120   // GLSL 版本声明，对应 OpenGL 2.1

// 从顶点着色器传递过来的颜色变量
varying vec4 color;

void main(void)
{
    // 直接将从顶点着色器接收的颜色作为片段的最终颜色
    gl_FragColor = color;
}
