#version 120  // GLSL 版本声明，对应 OpenGL 2.1

// 从顶点着色器传递过来的纹理坐标
varying vec2 texCoord;

// Uniform 变量
uniform float uni_fValue;           // 动态插值值
uniform sampler2D uni_texture0;     // 第一个纹理采样器
uniform sampler2D uni_texture1;     // 第二个纹理采样器

void main(void)
{
    // 从第一个纹理采样
    vec4 color0 = texture2D(uni_texture0, texCoord);

    // 从第二个纹理采样
    vec4 color1 = texture2D(uni_texture1, texCoord);

    // 线性插值混合两个纹理颜色
    // 根据 uni_fValue 在 0-1 之间平滑过渡
    vec4 newColor = color0 * (1.0 - uni_fValue) + color1 * uni_fValue;

    // 设置最终片段颜色
    gl_FragColor = newColor;
}
