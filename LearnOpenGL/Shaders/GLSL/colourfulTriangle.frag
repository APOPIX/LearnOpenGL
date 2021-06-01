#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main()
{
    /*生成的彩色三角形是片段着色器中进行的所谓片段插值(Fragment Interpolation)的结果。
     当渲染一个三角形时，光栅化(Rasterization)阶段通常会造成比原指定顶点更多的片段。
     光栅会根据每个片段在三角形形状上所处相对位置决定这些片段的位置。*/
    FragColor = vec4(ourColor, 1.0);
}
