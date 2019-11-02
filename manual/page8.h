/**

 <!-- By guoping.liu -->

 @page chapter08 附录一 计算机图形学基础(OpenGL)

 @section chapter08_1 8.1 坐标系

 OpenGL中使用的坐标系有两种，分别为世界坐标系和屏幕坐标系。世界坐标系即OpenGL内部处理时使用的三维坐标系，
 而屏幕坐标系即为在计算机屏幕上绘图时使用的坐标系。

 通常，OpenGL所使用的世界坐标系为右手型，如下图所示。

 @image html cg-coordinate-system.png

 从计算机屏幕的角度来看，x轴正方向为屏幕从左向右，y轴正方向为屏幕从下向上，z轴正方向为屏幕从里向外。
 而进行旋转操作时需要指定的角度θ的方向则由右手法则来决定，即右手握拳，大拇指直向某个坐标轴的正方向，
 那么其余四指指向的方向即为该坐标轴上的θ角的正方向（即θ角增加的方向），在上图中用圆弧形箭头标出。

 @section chapter08_2 8.2 投影

 将世界坐标系中的物体映射到屏幕坐标系上的方法称为投影。投影的方式包括平行投影和透视投影两种。

 平行投影的投影线相互平行，投影的结果与原物体的大小相等，因此广泛地应用于工程制图等方面。

 透视投影的投影线相交于一点，因此投影的结果与原物体的实际大小并不一致，而是会近大远小, 因此透视投影更接近于真实世界的投影方式。

 @image html cg-perspective.png

 @section chapter08_3 8.3 几何变换

 OpenGL中可以通过矩阵来实现物体的平移、旋转和缩放。

 以下是物体平移的矩阵运算：

 @image html cg-matrix-translate.png

  以下是物体平移的旋转运算：

 @image html cg-matrix-rotate.png

 以下是物体缩放的矩阵运算：

 @image html cg-matrix-scale.png


 OpenGL中实现物体的平移、旋转和缩放的手段还有其他，改变物体顶点的坐标值也能达到同样的效果。

 @section chapter08_4 8.4 纹理


 */
