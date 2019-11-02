/**

 <!-- By guoping.liu -->

 @page chapter02 第二章 zglEngine简介

 本章主要内容如下：
	- @ref chapter02_1 "关键类"
	- @ref chapter02_2 "基本特性"
	- @ref chapter02_3 "主要工作流程"
	- @ref chapter02_4 "文件组成"


 @section chapter02_1 2.1 关键类

 本节会介绍zglEngine中几个主要的关键类，这几个关键类的分层示意图如下图所示。

 @image html architect-main.png "图 2.1 关键类"

 目录：
 - @ref chapter02_11
 - @ref chapter02_12
 - @ref chapter02_13
 - @ref chapter02_14
 - @ref chapter02_15
 - @ref chapter02_16
 - @ref chapter02_17

 @subsection chapter02_11 1、zglEngine 主要类

 #zglEngine 的主要工作包括：
	- 维护图形系统的参数控制；
	- 维护图形队列，以及图形队列渲染控制；
	- 维护Widget队列，接收外部事件，并予以分发，同时维护Widget队列的更新。

 #zglEngine 的其他特点包括：
	- 维护特殊的投影矩阵，使系统的设置更符合GUI系统的特性
	- 维护外部输入的处理，例如Fling的瞬时速度。

 @subsection chapter02_12 2、zglDevice 平台

 #zglDevice 是和平台相关的抽象层，整个系统通过zglDevice来屏蔽不同平台之间的差异。

 以下是 zglDevice 的抽象接口：

 @code
		virtual bool init(int width, int height, const char * title) = 0;

		virtual bool fini() = 0;

		virtual bool run() = 0;

		virtual void flipScreen() = 0;
 @endcode

 通常一个典型的应用程序可以这样这样完成：

 @code
	 zglDevice * g_device = new zglXXXDevice();
	 g_device->init(480, 320, "example");

	 g_device->run();//Call g_device->flipScreen();

	 g_device->fini();

	 delete g_device;

 @endcode

 由上面的接口和示例可知， #zglDevice 所定义的接口相当抽象和宽泛，基本上可以覆盖目
 前已知的平台，因此整个Engine的移植工作的可行性非常好，移植过程也相对简单。


 目前已经支持的Device有：
  - Linux: X11 + XGL
  - Win32: MINGW + OpenGL
  - Android: Native + FrameBuffer
  - Android: Framework(Java层，GLSurfaceView+GLThread)

 对于潜在的或者新生的平台，例如<a href="http://meego.com/about">Meego/Qt</a>，
 整个Engine也在关注中，如果有必要，可以在适当的时候予以支持。

 @subsection chapter02_13 3、zglCore后端

 #zglCore 是3D/GPU(Graphic Processing Unit)相关的抽象层，可以屏蔽不同3D/GPU
 设备和图形库的差异。整个Engine所需要GPU提供的功能被分解并封装到zglCore。

 在不同的设备上实现zglCore所需要的功能，即可以在不同的设备上完成同样的3D渲染任务。

 目前已经支持的GPU有：
  - OpenGL 1.4 +,
  - OpenGL ES 1.1

 由于Win32平台对于OpenGL只支持到OpenGL 1.1，因此Win32平台需要使用<a href="http://glew.sourceforge.net/">glew</a>来实现高版本的OpenGL支持。

 @subsection chapter02_14 4、zglPlugin系列类

 #zglPlugin 可以看作是对zglEngine工作时序的监听器(Listener)，或者说是一种特殊的Callback函数的集合。

 同所有的应用程序和第三方库一样，zglEngine有初始化和结束的需求；也同通常的3D应用程序一样，有建模、
 绘制和逻辑控制时序。 zglPlugin作为整个Engine的扩展，在Engine执行特殊的动作时，提供延伸功能的机会，
 使用户可以充分利用Engine的时序和现有功能，添加自己需要执行的逻辑模块。

 #zglPlugin 接口中，处理初始化和结束的接口有：
 - onInit(): 初始化数据、资源，初始化Widget控件
 - onFini(): 释放动态申请的资源，(关闭不相关的Widget控件)

 处理3D相关逻辑的接口，同GLUT库相比，建模的过程可以看作与接口 onInit() 合并，至于图形绘制过程，被zglEngine完全隐藏:
 - onResize(): 根据绘制区域的大小，设置Widget尺寸、位置等

 逻辑控制过程：
 - onUpdate(): 画面参数更新，多用来处理动画
 - handleTouchEvent()：TouchEvent的处理，多用来处理外部输入。

 #zglPluginEx 重载了接口handleTouchEvent(), 主要加强了对TouchEvent的手势解析，用来简化用户的常用操作。

 zglEngine自身带有一个调试用的zglPlugin，可以参考《 @ref chapter02_26 》。

 在同一时刻，目前暂时只支持一个用户提供的zglPlugin，支持多个zglPlugin切换，但不支持多个zglPlugin的动画切换。

 @subsection chapter02_15 5、zglPrim系列类

 #zglPrim 是所有基本图元(Graphics Primitive)的基类。

 zglPrim及其子类，主要负责将由外部维护的顶点(Vertex)数组，纹理、材质等相关图形资源绘制到屏幕上。

 一般来说，对于常用的zglPrim类，zglPrim2D和zglPrim3D不直接拥有zglVertex资源。比较复杂的类，
 主要是zglMesh相关的子类， #zglQuadMesh 、 #zglCube 和 #zglSphere 等，鉴于实际运行的复杂性，
 会自己申请并分配顶点( #zglVertex )资源。

 zglMesh的顶点类型是 #zglVertexEx ，相比 #zglVertex ，增加了向量部分，增加了对光照的支持。

 zglPrim在这一过程中主要负责当前图形的渲染过程的控制，如果有必要也会维护Hardware buffer
 object的资源，将外部输入的顶点数组传送至GPU中，以便提高渲染效率。

 <img src="classzglPrim__inherit__graph.png">

 zglPrimList用来组织所有的Prim实例，将不同类的Prim归类放置到合适的队列中去。
 所有zglPrimList中的Prim将都有机会被绘制到屏幕上，如果某个Prim从zglPrimList中剔除，将不会被绘制，
 直到重新放入zglPrimList。

 @subsection chapter02_16 6、zglWidget系列类

 #zglWidget 是所有 Widget(图形控件)的基类。

 zglWidget主要负责维护维护本身所拥有的顶点(Vertex)数组的内容，并且在必要时，例如尺寸变化时，
 对顶点(Vertex)数组予以更新，同时维护部分的位置信息等;

 zglWidget另一个主要的任务是响应系统的事件，例如Hit test(hitTest()), 自我更新事件(onUpdate())，Touch事件(onTouchEvent())。

 正确处理Hit test将使Widget有机会获得外部事件的输入。自我更新事件可以更新Widget外形，或者更新动画。
 Touch事件可以使Widget直接与外部互动。

 <img src="classzglWidget__inherit__graph.png">

 zglWidgetList用来组织所有的Widget实例，将不同类的Widget归类放置到合适的队列中去。

 所有zglWidgetList中的Widget将都有机会被绘制到屏幕上，并且响应外部事件，同时获得自我更新的机会。
 不在zglWidgetList中的Widget将失去所有绘制机会、获取事件的机会以及自我更新的机会。
 在zglWidgetList中的Widget，如果被设置为Disabled, 将失去获取事件的机会，但仍旧有机会被绘制，同时也可以获得自我更新机会。

 @subsection chapter02_17 7、zglAnimation系列类

 这里的动画支持如下几种动画：
  -# Tween动画：基于时间轴或者根据帧数，需要插值计算进行的动画。
  -# Frame动画：固定的帧数，事先固定的显示方式，逐帧显示的动画。

 动画的更新过程由 #zglWidget 完成，因此 #zglAnimation 不直接支持 #zglPrim 系列类的动画。如果 #zglPrim 需要动画，
 可以直接使用系统提供的插值器进行动画的计算。

 目前已经实现的的实际动画包括如下几种：
  - Alpha
  - Transition

 因为3D物体的旋转比起2D来更加复杂，旋转动画的实现过程中需要考虑平移动画的存在与否，因此不推荐轻易使用Rotation的动画
 (#zglRotateAnimation)，如果有必要的话，可以从 #zglAnimation 继承一个新的类。

 用户可以从 #zglAlphaAnimation 和 #zglTranslateAnimation 中借鉴。而且从代码中可以看到，这两个动画的实现都非常简洁。

 插值器(#zglInterpolator)是动画的重要组成部分。插值器用来在两个不连续的数值之间插入一个新的值，动画过程中比较常
 见的是利用时间轴来计算某一时间点上的插值，用来改变位置、颜色、角度等等。

 目前系统使用的插值器共有两大类，接近40种插值器，基本上可以满足常见的需求。

 一类是参考Android本身带有的插值器，另外一类参考Clutter附带的插值器，这一类的插值器和Adobe Flash的插值器类似。

 @section chapter02_2 2.2 基本特性

 本节会介绍zglEngine中几个主要的特性，也是非常重要的特性，理解这里所列的基本特性对于快速掌握zglEngine非常有帮助。

 目录如下：
 - @ref chapter02_21
 - @ref chapter02_22
 - @ref chapter02_23
 - @ref chapter02_24
 - @ref chapter02_25
 - @ref chapter02_26

 @subsection chapter02_21 1、颜色以及其他单位设置

 zglEninge 中颜色使用32位整型，为RGBA8888。这个和Android上默认的颜色顺序稍有不同，使用时需要注意。
 但对于Android JNI，使用32位位图时，正好无缝连接(ANDROID_BITMAP_FORMAT_RGBA_8888)。

 Android中也会采用RGB_565, RGBA_4444和A_8这几种格式，但基于图像处理的考虑，通常建议使用RGBA_8888。

 角度的单位一般使用浮点数，不支持诸于GLFixed之类的定点变量。

 @subsection chapter02_22 2、坐标系设置

 zglEngine中，将基本图形分为2D和3D两种。

 这里的2D图形(zglPrim2D)，并不是真的指2D的部分，而是使用<b>平行投影</b>的3D图形，因为这些图形在使用效果上更接近于通常概念下的2D图形； 而3D图形(zglPrim3D)则是使用
 <b>透视投影</b>的3D图形。这样的区分和归类，便于理解，同时可以使图形的渲染有章可循。

 平行投影的投影线相互平行，投影的结果与原物体的大小相等，因此广泛地应用于工程制图，游戏菜单等方面。

 透视投影的投影线相交于一点，因此投影的结果与原物体的实际大小并不一致，而是会近处的物体显得大远处的物体显得小。因此透视投影更接近于真实世界的投影方式。

 在具体坐标系的设置上，系统使用了特殊的投影矩阵，使Z轴总位于用户平面的中心点。原点和方向都和OpenGL的默认设置不一样。 具体的示意图如下图所示。
 @image html coordinate-setting.png "图 2.2 坐标系设置"

 这样设置的好处在于统一坐标，使用户在切换平行投影和透视投影时，不需要来回计算不同的坐标。

 一般来讲，目前大多数平台的原始坐标都以左上角为原点，需要做转换后再输入到zglEngine，这是一个很<b>重要的步骤</b>，尤其是对Hit test的结果。参考的公式如下：

 @image html coordinate-format.png "图 2.3 输入坐标转换公式"

 通常的代码为：
 @code
 	int mx = x - (m_width >> 1);
	int my = (m_height >> 1) - y;
 @endcode

 其中，x, y是原始坐标，m_width, m_height是具体平台上3D显示区域的宽高尺寸。

 @subsection chapter02_23 3、局部坐标系和全局坐标系

 类似于太阳和地球的关系，zglEngine将坐标系分为局部坐标系和全局坐标系。

 可以抽象地认为全局坐标系主要用来控制camera的位置、旋转角度；局部坐标系用来控制物体本身的位置、大小、旋转等参数。

 @image html coordinate-system.png "图 2.4 全局坐标系和局部坐标系"

 @subsection chapter02_24 4、分层渲染设置

 首先需要说明的是，Engine的渲染过程不需要用户刻意关注，这里只是介绍Engine诸多代码下的工作方式。

 和典型的3D引擎一样，（1）Engine会首先绘制背景，（2）尔后Engine会绘制3D(透视投影）类图形中不使用半透明效果(Blend off)的图形，
 渲染的顺序视用户创建物体的顺序而定；（3）然后将3D类图形中使用半透明效果的图形按照离观察者距离的顺序，由远到近逐一绘制。用户可以
 通过调节Offset来影响绘制顺序，（4）最后被绘制的是2D（平行投影）类图形，会参考用户指定的顺序绘制，如果用户没有指定绘制顺序，
 则参考照物体创建的顺序绘制。

 @image html render-layer.png "图 2.5 分层渲染设置"

 @subsection chapter02_25 5、Offset设置

 在上一部分讲到，绘制2D类图形时，“会按照用户指定的顺序绘制”。

 在实际运用中，有时会有多个物体处在同一个位置上，用户需要调整这些物体的先后顺序以便达到不同的效果，这时，就可以对这些物体设置不同
 的Offset偏移值来影响这些物体渲染的先后顺序。

 Offset的取值范围为[0, 255]。Offset为255的物体会被优先绘制，Offset为0的物体会最后绘制；Offset小的物体会覆盖Offset大的物体。

 zglEngine绘制完zglPrim3D后再绘制zglPrim2D，Offset是zglPrim2D和zglPrim3D分别计算的，不要尝试调整Offset使某个
 zglPrim3D显示在某个zglPrim2D之上。

 在设置Offset时，需要注意的是，物体本身会有一个根据距离计算出的默认Offset值，用户设置的Offset值和默认Offset值相加后，即为最终的Offset值。
 因此用户设置 Offset值的时候，除非特别的需求，一般不要设置太大的Offset值，对物体的Offset值做适当的调整即可。

 zglPrim2D的默认Offset为128， zglPrim3D的默认Offset值的计算公式如下:

 @verbatim

	offset = 255 * (distance - near) / (far - near)
 @endverbatim

 也可以用在线调试工具实时显示Widget的Offset值。

 @image html render-offset.png "图 2.6 Offset设置"

 @subsection chapter02_26 6、在线调试功能

 zglEngine也提供了在线调试功能，供开发人员测试用。目前主要提供了坐标值显示、Offset显示和Widget边界显示功能，需要开发人员做特殊设定且重新编译后方能提供。

 启动该功能需要修改配置文件Makefile.in.Linux:
 @verbatim
 DEFINES  =  -D_DEBUG_
 @endverbatim

 以下是实际运行时的截图，左边截图用白色字体和多边形显示了Offset值"[ Z = 168 ]"和Widget边界；右边截图显示了鼠标所在位置的坐标。
 @image html debug.png "图 2.7 在线Debug功能"

 @section chapter02_3 2.3 主要工作流程

 本节会介绍zglEngine中几个重要的流程，目录如下：

 @subsection chapter02_31 1、TouchEvent事件传递流程

 作为外部事件之一，TouchEvent主要由用户点击3D用户区域来产生。对于当今触摸屏的发展趋势，TouchEvent作为一个典型的事件存在于zglEngine中，
 Key Event则因为更多的复杂性没有被支持。

 整个TouchEvent的详细传递过程如下，如果不关心细节的话，Step 3.x都可以忽略：
 - 1、用户点击触摸屏，
 - 2、zglEngine接收该点击事件的坐标和事件戳
 - 3、zglEngine首先将事件传递给zglWidgetList
	 - 3.1 zglWidgetList创建zglTouchEvent变量；
	 - 3.2 zglWidgetList根据点击的坐标，对所有zglWidget2D的成员，进行Hit test测试；
	 - 3.3 如果3.2 Hit test测试没有命中，将对所有zglWidget3D的成员，进行Hit test测试。
	 - 3.4 如果3.2 Hit test测试命中,将事件传递给命中的zglWidget2D的成员，将处理结果返回。
	 - 3.5 如果3.3 Hit test测试命中，对命中的所有zglWidget3D的成员，选择Z-depth最小的成员；
	 - 3.6 将事件传递给对3.5中选中的zglWidget3D的成员，将处理结果返回。
	 - 3.7 如果以上Hit test都没有命中过，返回False。
 - 4、如果zglWidgetList(3.4, 3.6)返回的结果的结果为True，事件传递过程结束，返回True。
 - 5、如果zglWidgetList(3.4, 3.6)返回的结果的结果为False，则将事件相关信息传递给zglPlugin。
 - 6、将zglPlugin处理的结果作为整个处理过程的结果返回返回。

 整个TouchEvent的传递过程示意图如下：

 @image html handletouchevent.png "图 2.8 TouchEvent事件传递流程"

 需要留意的是，zglPlugin的优先级较低，不是一直都有机会接收到TouchEvent事件的，而且接收的事件也可能不是成组的。

 @subsection chapter02_32 2、Update过程

 Update不能称为外部事件，Update是GPU不断重复渲染的大循环过程中zglEngine特意予以预留的时间间隙，用来提供Widget
 图形参数更新和动画更新的机会，这同时也是zglPlugin来更新画面的机会。

 整个update的执行过程如下：
 - 1、zglEngine首先通知zglWidgetList更新；
 - 2、zglWidgetList通知存在于zglWidgetList中的所有的Widget变量逐一更新；
 - 3、zglEngine通知zglPlugin更新；
 - 4、zglEngine根据zglWidgetList更新和zglPlugin更新的结果来判断画面是否有过变化。

 整个Update的过程示意图如下：
 @image html handleupdateevent.png "图 2.9 Update过程"


 @section chapter02_4 2.4 文件列表

 @subsection chapter02_41 1、主要的文件目录如下表所示
 @verbatim
	 .
	  |-samples     - 所有示例代码
	  |-src			- 源代码
	  |-demo		- 示例执行文件目录
	  |-enhancement - 部分过期的代码，或者是加强的代码
	  |-include		- 头文件
	  |-3rdparty	- 第三方库
	  |  |-Box2D
	  |-lib			- 库文件输出目录
	  |-manual
	  |  |-html		- 本手册所在目录
 @endverbatim

 @subsection chapter02_42 2、主要的辅助文件
 @verbatim
		clean.sh
		Makefile  					- Makefile文件
		Makefile.in.Linux				- Linux平台的Makefile配置文件
		Makefile.in.MINGW32_NT-5.1			- MINGW32的Makefile配置文件
		README						- 说明文件
 @endverbatim

 @subsection chapter02_43 3、主要的头文件文件
 @verbatim
		zglCore.h					- OpenGL/GPU后端的操作定义
		zglDevice.h					- 平台抽象层
		zglWin32Device.h				- Win32平台移植相关实现
		zglX11Device.h					- X11平台的相关实现
		zglAndroidNativeDevice.h			- Android Framebuffer实现
		zglEngine.h					- Engine
		zglLight.h					- 光和雾
		zglMaterial.h					- 材质
		zglTexture.h					- 纹理
		zglMatrix.h					- 矩阵操作
		zglVector.h					- 向量操作
		zglDebugText.h					- 内嵌点阵字体，等宽，调试用
		zglEngineDebugger.h				- 内嵌的在线调试模块
		zglVertex.h					- 顶点
		zglPlugin.h					- 插件
		zglScreen.h					- 视口的封装
		zglPrim.h					- Primitive
		zglPrim2D.h					- 平行投影的Primitive
		zglPrim3D.h					- 透视投影的Primitive
		zglPrimParticle.h				- 粒子系统的图形部分
		zglPrimList.h					- Primitive 列表
		zglMesh.h					- 网格
		zglQuadMesh.h					- 四边形网格
		zglCube.h					- 立方体
		zglSphere.h					- 球体
		zglWidget.h					- widget基本类
		zglWidget2D.h					- 使用平行投影Primitive实现的Widget
		zglWidget3D.h					- 使用透视投影Primitive实现的Widget
		zglWidgetListener.h				- Widget的监听器
		zglWidgetList.h					- Widget列表
		zglWidgetParticle.h				- 粒子widget
		zglWidgetGroup.h				- Widget集合，形成复杂widget
		zglSlider.h					- Slider控件widget
		zglImageView.h					- 图形显示widget
		zglBitmapTextView.h				- 不等宽点阵字体
		zglButton.h					- 按钮widget
		zglStatic.h					- 静态控件widget
		zglAnimation.h					- 动画基本类
		zglAnimationGroup.h				- 动画集合，形成复杂动画
		zglAnimator.h					- 动画
		zglAlphaAnimation.h				- alpha动画
		zglTranslateAnimation.h				- 平移动画
		zglRotateAnimation.h				- 旋转动画
		zglInterpolator.h				- 插值器，常用插值器
		zglEaseInterpolator.h				- Flash常用插值器
		zglImage.h					- 图像类，图像处理
		zglImageLoader.h				- 图像转载， bmp， png.
		zglBasicType.h					- 基本类型定义
		zglEventType.h					- 事件定义
		zglUtil.h					- 常用辅助函数
 @endverbatim


 */
