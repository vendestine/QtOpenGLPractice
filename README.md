## part1 opengl固定管线编程
opengl1.5之前一直都是固定管线编程，不需要使用着色器，只需要使用已经封装好的一些openglAPI。
在QT中使用opengl比较简单，让类继承QOpengGLWidget QOpenGLFunctions，其中QT5中默认使用opengl2.1版本，QT6默认使用3.3core版本，我们也可以自定义使用的版本。

- day1: 在qt中使用OpenGL
- day2: hello rectangle, 在qt中画两个正方形
- day3: 视频剪辑子控件，拖动矩形顶点拉伸矩阵
- day4：day3的视频剪辑子控件，矩形贴图
- day5: 透视投影，glu库
- day6: 立方体，平移旋转缩放，利用固定管线glu矩阵和QT自带矩阵两种方式
- day7: uv纹理坐标，矩形单面贴图
- day8: 立方体多面贴图
- day9: uv坐标变换，贴图动画
- day10: 多重纹理，需要opengl扩展glew.h
- day11: 使用显示列表，优化
- day12: 使用vbo，优化
- day13: fbo，离屏渲染

## part2 opengl shader编程
opengl在1.5之后引入了可编程管线，使用着色器更精确地控制渲染过程。

- day1: 使用opengl的原生shader API
- day2: 使用QT封装的shader API， 三种写法
- day3: shader纹理贴图，使用两种方法

## part3 opengl shader特效
opengl特效，主要就是如何通过着色器的变化，产生一些动画效果。

- day1: 两张纹理之间，平滑过渡
- 
