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

    // 根据纹理坐标和过渡值决定采样纹理
    if (texCoord.x <= uni_fValue) {
        // 左侧区域：从第一个纹理采样
        // 通过调整纹理坐标实现平移效果
        newColor = texture2D(uni_texture0,
            vec2(texCoord.x + (1.0 - uni_fValue), texCoord.y)
        );
    }
    else {
        // 右侧区域：从第二个纹理采样
        // 通过调整纹理坐标实现平移效果
        newColor = texture2D(uni_texture1,
            vec2(texCoord.x - uni_fValue, texCoord.y)
        );
    }

    // 输出最终颜色
    gl_FragColor = newColor;
}
