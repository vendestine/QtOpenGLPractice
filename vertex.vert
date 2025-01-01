#version 120  // GLSL 1.20 版本声明

// 属性变量
attribute vec4 attr_position;  // 顶点位置坐标属性
attribute vec2 attr_uv;        // 顶点纹理坐标属性

// Uniform 变量
uniform mat4 uni_mat;  // 变换矩阵

// 传递变量
varying vec2 texCoord;  // 纹理坐标

void main(void)
{
    // 直接传递纹理坐标到片段着色器
    texCoord = attr_uv;

    // 使用变换矩阵计算顶点最终位置
    gl_Position = uni_mat * attr_position;
}
