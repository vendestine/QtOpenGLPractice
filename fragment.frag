#version 120

varying vec2 texCoord;        // 从顶点着色器传递来的纹理坐标
uniform sampler2D uni_texture; // 纹理采样器

void main(void)
{
    vec4 color = texture2D(uni_texture, texCoord); // 使用从顶点着色器传入的 texCoord
    // vec4 color = texture2D(uni_texture, gl_TexCoord[0].st); // 使用 OpenGL 固定管线的纹理坐标
    gl_FragColor = color; // 设置片段的最终颜色
}
