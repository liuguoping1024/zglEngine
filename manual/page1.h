/**
  <!-- By guoping.liu -->

 @page chapter01 第一章 zglEngine概览


@section chapter01_1 1.1 简介

zglEngine是一个易于使用，同时功能强大和高效的3D GUI/图形引擎;

zglEngine设计了一套高效简洁的API，为GUI应用程序开发提供了完整的事件传递功能和完整的动画功能，
为快速开发GUI应用程序和小型游戏提供了全部的3D图形特性支持。


@section chapter01_2 1.2 主要特性

 @subsection chapter01_21 1、简洁高效，而且易于学习、使用和扩展;
	- 更精简易懂、更轻巧易用的设计，更高效的API接口;
	- 设计中不预设限制条件，从而有更多更灵活的扩展方式。

 @subsection chapter01_22 2、完整而且高效的GUI事件系统，丰富的动画系统;
	 - 事件的处理更加合理;
	 - 提供了多种丰富的动画实现手段和丰富的动画插值器。

 @subsection chapter01_23 3、为GUI系统特别定制的3D坐标系统以及特殊的渲染设置等等;
	 - 专注于GUI相关的处理，3D坐标系针对GUI系统的特点专门定制;
	 - 特殊的渲染设置，更加适合GUI系统。

 @subsection chapter01_24 4、跨平台，目前已经在X11、Win32以及Android上运行;
 	 - 移植方便，目前已经在X11、Win32以及Android上运行，同时，为将来的Meego/Qt也做好了准备;
	 - 无缝支持多种3D后端支持，目前已经支持OpenGL和OpenGL ES;
	 - 开发者无须刻意关注不同平台和不同后端的差异性，尤其对于嵌入式平台的开发，从而可以大大提高开发速度。

 @subsection chapter01_25 5、特别针对嵌入式平台，与Android平台贴合紧密
 	 - 针对嵌入式平台优化;
	 - 完全吻合Anroid 3D的时序，可以与Android平台紧密集成;
	 - 全面支持OpenGL ES 1.x.

@section chapter01_3 1.3 主要架构

3D 开发主要分为三个步骤：建模、渲染、逻辑控制
 - 建模，指通过一些基本图元如点、线、三角形、多边形将物体画出来
 - 渲染，即使用 OpenGL等图形接口将模型在计算机上画出来。
 - 逻辑控制，根据时间或者事件计算模型各个顶点的坐标,颜色等

zglEngine的所有绘制的对象均基于基本图元(Graphics primitives)，不管是简单或者复杂的
GUI Widget，都会分解成为基本图元，最终交由GPU绘制，以便充分利用3D设备的图形渲染能力;

而同时，对这些图元进行操作的逻辑控制部分都封装在GUI Widget中。外界对Widget的刺激，最终
会影响到Widget的行为，进而改变基本图元的各个属性，最后通过GPU渲染的效果表现出来。


@image html overview.png

在具体实现上，整个系统为典型的MVC模型(<a href="http://en.wikipedia.org/wiki/
Model%E2%80%93view%E2%80%93controller">Model–view–controller</a>)，数据与表示层分离。

整个系统可以分成三层，其中除了与平台和3D/GPU相关的部分之外，是整个系统比较关键的两层：
@subsection chapter01_31 1、Graphics层

主要管理当前所有图元，并且将图元按照特定的顺序送往GPU进行绘制等。

这其实担当了M-V-C模型中的View(表示层)的功能。


@subsection chapter01_32 2、Widget层

管理所有的Widget，传递以及处理外部事件，维护widget的实时更新等。

这其实担当了M-V-C模型中的Modle和controller的功能。

@image html overview-detail.png

其他主要的组件有：
 -# Engine：响应外部请求，维护Widget系统和Graphics系统的运行。
 -# Math：主要是处理系统中Vector， Matrix等算术运算。
 -# Light， Fog， Material： 光照，材质
 -# Animation： 动画模块，完整的动画更新机制和丰富的插值器
 -# Debug： 在线调试模块，提供坐标等实时信息。


@section chapter01_4 1.4 屏幕截图

<pre>
<table border="1" width="80%">
  <tr>
    <th>@image html linux-logo.png "linux"</th>
    <th>@image html windows-logo.png "windows"</th>
    <th>@image html android-logo.png "android"</th>
  </tr>
</table>
</pre>


@subsection chapter01_41 1、 Linux 屏幕截图

<pre>
<table border="1" width="80%">
  <tr>
    <th>@image html 10-linux.png</th>
    <th>@image html 14-linux.png</th>
    <th>@image html 16-linux.png</th>
  </tr>
</table>
</pre>


@subsection chapter01_42 2、 Windows 屏幕截图

<pre>
<table border="1" width="80%">
  <tr>
    <th>@image html 10-win32.png</th>
    <th>@image html 14-win32.png</th>
    <th>@image html 16-win32.png</th>
  </tr>
</table>
</pre>


@subsection chapter01_43 3、 Android 屏幕截图
<pre>
<table border="1" width="80%">
  <tr>
    <th>@image html 10-android.png</th>
    <th>@image html 14-android.png</th>
    <th>@image html 16-android.png</th>
  </tr>
</table>
</pre>

 以上截图由DDMS获取。

*/
