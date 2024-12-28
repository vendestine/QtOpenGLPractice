#version 120   // GLSL 版本声明，对应 OpenGL 2.1

attribute vec4 attr_position; // 顶点位置属性
attribute vec4 attr_color;    // 顶点颜色属性
uniform mat4 uni_mat;         // 变换矩阵 uniform
varying vec4 color;           // 用于传递颜色到片段着色器的变量

void main(void)
{
    color = attr_color; // 将顶点颜色赋值给 varying 变量
    gl_Position = uni_mat * attr_position; // 使用变换矩阵对顶点位置进行变换
}
