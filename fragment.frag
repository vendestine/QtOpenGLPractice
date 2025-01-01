#version 120  // GLSL 1.20 版本声明

// 从顶点着色器接收的纹理坐标
varying vec2 texCoord;

// Uniform 变量
uniform float uni_fValue;        // 动态过渡值
uniform sampler2D uni_texture0;  // 第一个纹理
uniform sampler2D uni_texture1;  // 第二个纹理

void main(void)
{
    // 初始化颜色为透明黑色
    vec4 newColor = vec4(0, 0, 0, 1);

    if (texCoord.x <= uni_fValue) {
        // 左侧区域：从第二个纹理采样，并根据 uni_fValue 调整透明度
        newColor = texture2D(uni_texture1, vec2(texCoord.x + (1.0 - uni_fValue), texCoord.y))* uni_fValue;
    }
    else {
        // 右侧区域：从第一个纹理采样，保持原始颜色
        newColor = texture2D(uni_texture0, vec2(texCoord.x, texCoord.y));
    }

    // 输出最终颜色
    gl_FragColor = newColor;
}
