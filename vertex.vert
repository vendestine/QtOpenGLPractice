#version 120

attribute vec4 attr_position; // 顶点位置属性
attribute vec2 attr_uv;       // 顶点纹理坐标属性
uniform mat4 uni_mat;         // 变换矩阵 uniform
varying vec2 texCoord;        // 用于传递到片段着色器的纹理坐标

void main(void)
{
    texCoord = attr_uv; // 将顶点的纹理坐标传递给片段着色器
    gl_TexCoord[0] = gl_MultiTexCoord0; // 将多重纹理坐标赋值给内置变量
    gl_Position = uni_mat * attr_position; // 计算顶点的最终位置
}
