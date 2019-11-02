/**

 <!-- By guoping.liu -->

 @page chapter03 第三章 zglEngine的简单应用开发


 本章主要介绍一些简单应用程序的开发，用以初步接触zglEngine的部分概念.


  - @ref chapter03_1
 	- @ref chapter03_11
 	- @ref chapter03_12
  - @ref chapter03_2

 需要重点了解的内容如下：
 -# 初步了解zglEngine的总的工作流程
 -# 熟悉zglEngine的坐标系设置和Focus Distance
 -#


 @section chapter03_1 3.1 X11平台简单应用程序开发

 	- @ref chapter03_11 "1、 最简单的图形"
 	- @ref chapter03_12 "2、 使用zglPlugin开发"


 @subsection chapter03_11 1、 最简单的图形

 本节的目的：
  -# 初步了解zglEngine的总的工作流程
  -# 了解zglPrim和zglVetex的一般工作模式
  -# 熟悉zglEngine的坐标系设置和Focus Distance
  -# 了解zglEngine的颜色设置和其他使用的单位
  -# 初步接触纹理

 本节的示例代码如下，代码之后会有比较详细的说明，由于这是第一次接触，内容会比较多，设计到的要点也会相对重要：

 @code

	#include <stdlib.h>

	#include "zglDevice.h"
	#include "zglEngine.h"

	#include "zglPrim3D.h"
	#include "zglVertex.h"

	int main(int argc, char* argv[])
	{
		zglDevice* device = zglDevice::getDevice();

		zglEngine *glEngine = zglEngine::getEngine();

		int w = 480;
		int h = 320;

		device->init(w, h, "Sample application : zglEngine");
		glEngine->initiate();

		zglVertex vertex[4];
		zglPrim3D prim;

		vertex[0].init(-80, 100, 0, 0.0f, 0.0f, 0xff0000ff);
		vertex[1].init(-80, -100, 0, 0.0f, 1.0f, 0x00ff00ff);
		vertex[2].init(80, 100, 0, 1.0f, 0.0f, 0x0000ffff);
		vertex[3].init(80, -100, 0, 1.0f, 1.0f, 0x000000ff);

		prim.init(DRAW_TRI_STRIP, vertex, 4, NULL);

		glEngine->resize(w, h);
		prim.world().translate(0, 0, 990.0f);

		glEngine->renderScreen();
		device->flipScreen();

		//!< main loop, Android::onDrawFrame is included in the loop.
		//!< User does not need to care about when, how to render the scree.
		device->run();

		glEngine->finish();

		zglEngine::destroyEngine();
		zglDevice::destroyDevice();

		return 0;
	}

 @endcode

 实际运行的截图：

 @image html simple-1.png "图 3-1 示例一"


 以下为比较详细的代码说明：

 @subsubsection chapter03_111 1、启动代码

 @code
	zglDevice* device = zglDevice::getDevice();
	zglEngine *glEngine = zglEngine::getEngine();
 @endcode

 函数 getDevice 和 getEngine 为静态函数，用户可以在任何地方调用这两个函数，
从而得到全局的 zglDevice 和 zglEngine 指针；如果调用这些函数之前，全局指针为空，静态函数也会创建好全局指针，因此不会返回空指针。
(这样的设计正是限制zglEngin使用方式的原因之一：只有一个固定实例）。

 @subsubsection chapter03_112 2、初始化环境（窗口，OpenGL等）

 @code
	device->init(w, h, "Sample application : zglEngine");
	glEngine->initiate();
 @endcode

 zglDevice创建完毕之后，需要执行初始化动作，主要包括创建X窗口，设立XGL环境。尔后才能进行Engine的初始化动作。

 这里的参数分别为X窗口的宽高，以及X窗口的标题。

 @subsubsection chapter03_113 3、初始化图形元素，（投影方式、FocusDistance、坐标系设置、全局坐标系与局部坐标系）

 @code
	zglVertex vertex[4];
	zglPrim3D prim;

	vertex[0].init(-80, 100, 0, 0.0f, 0.0f, 0xff0000ff);
	vertex[1].init(-80, -100, 0, 0.0f, 1.0f, 0x00ff00ff);
	vertex[2].init(80, 100, 0, 1.0f, 0.0f, 0x0000ffff);
	vertex[3].init(80, -100, 0, 1.0f, 1.0f, 0x000000ff);

	prim.init(DRAW_TRI_STRIP, vertex, 4, NULL);
 @endcode

 对于zglPrim类，鉴于灵活性的考虑，zglPrim2D和zglPrim3D不直接拥有zglVertex资源，因此这里准备了外部的zglVertex资源以便
zglPrim使用。

 系统中zglPrim2D和zglPrim3D的区分的依据是物体投影方式的不同，并不是说zglEngine另外拥有2D的模块。

 zglPrim2D是使用<b>平行投影</b>的图形物体。使用平行投影的物体，长度和坐标会服从设计者的要求，以非常准确的方式1:1投射到屏幕上。
一根100像素的线段，不管离屏幕多远，平行投射到屏幕上永远都是100像素。

 zglPrim3D是使用<b>透视投影</b>的图形物体。其特点是，离屏幕近，物体显得大;离屏幕远，物体显得小。

 在zglEngine的坐标系中，坐标系进行了特别的设计，针对zglPrim3D，引入了<b>Focus Distance</b>的概念。其特点是：

 @verbatim

	  在Focus Distance上的物体，会和平行投影的效果一样，严格按照1:1的关系投射到屏幕上。
 @endverbatim

 通常对于初次接触3D的开发者来说，在屏幕上显示一个物体时，往往不能准确地地在屏幕上控制一个物体的大小，一般需要多次调整位置和大小，进行多次的尝试。
而且尝试的最终结果也不一定是100%%精确。

 zglEngine 中引入<b>Focus Distance</b>的概念后，开发者在屏幕上显示物体时，会有一个很好的参考基准，而且对于GUI系统来说，
精准的尺寸正好符合GUI的需求，会大大降低开发难度，提高开发效率。

 <b>Focus Distance</b>的默认值是990.0，这个值可以根据实际需要进行调整。

 本例在窗口中绘制的矩形尺寸为(160x200)，被放置在Focus Distance位置上；在屏幕上最终显示的实际尺寸应该就是(160x120)，有意的开发者可以使用工具测量图3-1中的矩形大小。

 上面四个顶点按照左上(0)、左下(1)、右上(2)、右下(3)的逆时针方向排布，这是符合OpenGL渲染逆时针要求的一种顺序。更全的说明可以参考（图 3-5）。

  @image html simple-1-i.png "图 3-2 顶点顺序"

 顶点zglVertex初始化的过程(zglVertex::init())其实就是一般的赋值的过程，其中的参数是：坐标(x,y,z), 纹理坐标(u,v)和颜色值。

 zglEngine中坐标系的方向设置和一般的OpenGL中有些不同，采用了同一式的坐标设置。

 zglEngine的坐标系方向不管物体的投影方式如何，坐标系的x轴方向永远从左到右，y轴方向永远从下到上; Camera的坐标默认位于坐标系原点(0, 0, 0)，方向默认指向z轴正方向。即z值越大，离Camera越远。默认的Focus点为(0, 0, focus-distance)。

 对于正对着Camera的平面，坐标原点（0, 0）位于平面中央。在本例中，整个窗口的大小是480x320，那么x轴方向的可见范围是(-240, 240)，
方向从左到右; y轴方向的可见范围是(-160, 160), 方向从下到上。

 @image html coordinate-setting.png "图 3-3 坐标系设置"

 这里的顶点坐标的z值均为0, 这是因为zglEngine使用了全局坐标系和局部坐标系的分工。

 全局坐标系用来控制Camera的工作，当Camera变化时，所有透视投影的物体都会受到影响。
局部坐标系主要来控制单一图形物体的工作。物体如果需要简单平移，只需要移动局部坐标系即可;物体如果需要旋转，只需要旋转局部坐标系即可。如果物体需要围绕某一特定的中心旋转，
那么在填充Vertex数组时，刻意把这一旋转点作为局部坐标系的原点即可。

 这一设置，在一定程度上降低了OpenGL中先旋转后平移，与先平移后旋转的迷惑结果。

 @image html coordinate-system.png "图 3-4 全局坐标系和局部坐标系"

 这以后的代码
 @code
	 prim.world().translate(0, 0, 990.0f);
 @endcode
 正是将prim的局部坐标系移动到Focus Distance上。

 如果不希望多使用这句代码，改动Vertex的坐标内容也可以达到同样的效果：

 @code
	vertex[0].init(-80, 100, 990.0f, 0.0f, 0.0f, 0xff0000ff);
	vertex[1].init(-80, -100, 990.0f, 0.0f, 1.0f, 0x00ff00ff);
	vertex[2].init(80, 100, 990.0f, 1.0f, 0.0f, 0x0000ffff);
	vertex[3].init(80, -100, 990.0f, 1.0f, 1.0f, 0x000000ff);
 @endcode

 如果此时prim执行围绕y轴旋转的动作，因为prim的局部坐标系和全局坐标系重合，prim将围绕Camera（全局坐标系）旋转，从而会出现从屏幕中暂时消失的现象。


 zglEngine中，图形相关的部分，长度使用单精度浮点作为标准的单位。不使用定点运算是考虑3D运算很容易使定点溢出。
角度部分以弧度为主。另外，zglEngine中的颜色设置的顺序是RGBA8888。

 以下是示例中的颜色:
	- 0xff0000ff 红色
	- 0x00ff00ff 绿色
	- 0x0000ffff 蓝色
	- 0x000000ff 黑色


 这里是Engine初始化代码和prim的初始化代码：
 @code
	prim.init(DRAW_TRI_STRIP, vertex, 4, NULL);
 @endcode

 Engine的初始化代码必须在Device初始化完成后再执行，Device不准备好应用程序的执行环境，Engine运行会出现问题的。

 zglPrim和zglEngine相对独立，Prim的初始化可以在Engine初始化前完成。

 zglPrim3D的初始化函数的参数分别为：Draw type, 顶点数组以及顶点数目。 zglPrim不会对顶点数组的实际长度做检查，事实上C++代码也没法做;使用别的方法，如vector，
又会限制灵活性，因此，全凭工程师自觉。

 zglPrim3D还有另外的初始化函数，支持Index数组，以便处理更加复杂的情况。

 zglPrim支持OpenGL规定的大部分几何图元类型，与zglEngine中的变量一一对应, 例如DRAW_TRI_STRIP对应GL_TRIANGLE_STRIP：
	 - GL_POINTS
	 - GL_LINES
	 - GL_LINE_STRIP
	 - GL_LINE_LOOP
	 - GL_TRIANGLES
	 - GL_TRIANGLE_STRIP
	 - GL_TRIANGLE_FAN
	 - GL_QUADS

 其中GL_QUADS不属于OpenGL ES规范，zglEngine对此也完全支持，即使是使用 OpenGL ES设备，也可以正常地使用DRAW_QUADS。

 @image html image34.png "图 3-5 基本几何图元类型"

 不管是zglPrim2D还是zglPrim3D，其工作模式都比较相近，都需要外部提供Vertex数组和Index数组，初始化函数的形式也比较相近。

 @subsubsection chapter03_114 4、视口环境设置和图形元素的布局

 以下对zglEngine所使用的视口(ViewPort)大小做一定的限制。

 因为视口的大小对于全局坐标系的矩阵设置有很大的影响，因此，这里的工作显得不可或缺。
 @code
	glEngine->resize(w, h);
	prim.world().translate(0, 0, 990.0f);
 @endcode

 在GLUT和Android中，也有类似的函数来完成必要的尺寸设置。

 如前所述，prim.world()返回的是prim的局部坐标系。translate()执行的正是将prim移动到Focus Distance位置上。

 @subsubsection chapter03_115 5、绘制画面，并刷新屏幕。
 @code
	glEngine->renderScreen();
	device->flipScreen();
 @endcode

 通常的应用程序并不需要如此显式地调用这些函数。zglEngine会根据画面是否需要刷新，从而自动执行画面刷新和屏幕绘制动作。
因为本例较简单，zglEngine总认为画面不需要刷新，因此需要手动刷新一次。

 在以后的示例中，不会见到这样的代码。

 @subsubsection chapter03_116 6、程序主循环

 应用程序的主要循环部分，Device中重要的逻辑循环，zglEngine中的大部分操作都再次完成：
 @code
	device->run();
 @endcode

 在此函数中，设备会读取外部事件，将需要的事件传送到zglEngine中，并且根据时间，通知zglEngine更新。必要时，Device会自己处理特殊的事件，例如关闭应用程序等。

 此函数一直运行，直到退出的条件得到满足为止。需要注意的是，鉴于目前不同平台的特性，Device需要在退出此函数前，销毁相关的窗口和GL资源。

 @subsubsection chapter03_117 7、程序准备退出
 @code
	glEngine->finish();
 @endcode

 通知zglEngine准备退出，zglEngine主要会通知附属于zglEngine的其他成员及时释放资源。

 @code
	zglEngine::destroyEngine();
	zglDevice::destroyDevice();
 @endcode

 程序逐一销毁zglEngine和zglDevice，最后退出整个程序。


 至此，一个小的应用程序就完成。

 @subsubsection chapter03_118 8、改进：增加纹理

 上面的这个例子稍显简单，我们对这个小程序做一些小小的改动，增加图像文件和纹理的处理。

 @code
	#include <stdlib.h>

	#include "zglDevice.h"
	#include "zglEngine.h"

	#include "zglPrim3D.h"
	#include "zglVertex.h"
	#include "zglUtil.h"

	int main(int argc, char* argv[])
	{
		zglDevice* device = zglDevice::getDevice();

		//!< Create the engine when 'zglEngine::getEngine()' the first time invoked.
		zglEngine *glEngine = zglEngine::getEngine();

		int w = 480;
		int h = 320;

		device->init(w, h, "Sample application : zglEngine");

		zglTexture m_box_tex;

		zglVertex vertex[4];
		zglPrim3D prim;

		vertex[0].init(-80, 100, 0, 0.0f, 0.0f, 0xff0000ff);
		vertex[1].init(-80, -100, 0, 0.0f, 1.0f, 0x00ff00ff);
		vertex[2].init(80, 100, 0, 1.0f, 0.0f, 0x0000ffff);
		vertex[3].init(80, -100, 0, 1.0f, 1.0f, 0x000000ff);

		glEngine->initiate();

		const char box[] = "./data/Box.png";
		zglLoadPngTexture(box, &m_box_tex);

		prim.init(DRAW_TRI_STRIP, vertex, 4, &m_box_tex);

		//!< step 2. the screen info. (Android::onSurfaceChanged)
		glEngine->resize(w, h);
		prim.world().translate(0, 0, 990.0f);

		//!< Render the first step.
		glEngine->renderScreen();
		device->flipScreen();

		//!< main loop, Android::onDrawFrame is included in the loop.
		//!< User does not need to care about when, how to render the scree.
		device->run();

		glEngine->finish();

		zglEngine::destroyEngine();
		zglDevice::destroyDevice();

		return 0;
	}
 @endcode

 实际运行的截图：

 @image html simple-2.png "图 3-6 示例一 截图"

  以下为比较详细的代码说明：

 @code
		zglTexture m_box_tex;
		const char box[] = "./data/Box.png";
		zglLoadPngTexture(box, &m_box_tex);
 @endcode

 函数zglLoadPngTexture()封装了png文件的装载过程和纹理的绑定过程。

 zglEngine在Linux/X11平台上开发时，目前只支持bmp文件和png文件的装载，推荐使用png资源。

 OpenGL中纹理的原始尺寸必须满足2^n，并且最小尺寸不能小于64。zglEngine会根据读入的尺寸的大小，对图像做”凑整“的操作，以便新的图像能被
OpenGL所接受。凑整之后的纹理变量会提供新的最大纹理坐标值以供使用。

 @code
	 prim.init(DRAW_TRI_STRIP, vertex, 4, &m_box_tex);
 @endcode

 这里的初始化函数在最后一个参数上发生了变动，外部准备好的纹理直接交给prim使用。

 通常纹理是一张二维的图片，纹理坐标(UV)正是其中某一个像素的坐标相对于全长的百分比，Vertex数组中，纹理坐标(UV)的参数，分别为 (0.0f, 0.0f),
(0.0f, 1.0f), (1.0f, 0.0f), 和(1.0f, 1.0f)。正好按照逆时针方向设置了对纹理使用的像素范围。

 下图是针对同一张纹理，使用新的纹理坐标和新的颜色得到的结果,使用的是纹理的中间部分：

 @code
    vertex[0].init(-80, 100, 0, 52.0f/256, 52.0f/256, 0xffffffff);
    vertex[1].init(-80, -100, 0, 52.0f/256, 204.0f/256, 0xffffffff);
    vertex[2].init(80, 100, 0, 204.0f/256, 52.0f/256, 0xffffffff);
    vertex[3].init(80, -100, 0, 204.0f/256, 204.0f/256, 0xffffffff);
 @endcode

 @image html simple-2-1.png "图 3-7 示例一 截图"



 @subsection chapter03_12 2、 使用zglPlugin开发

 zglPlugin 可以看作是zglEngine中的插件。将zglPlugin交给zglEngine后，zglEngine会根据预先设定的
时序，调用zglPlugin中的接口，从而实现特定的目的。

 zglPlugin适合做单个的应用程序、或者专门完成某一个场景。不同场景之间可以进行切换，从而可以实现更复杂的应用程序。

 在本节中，我们会从一个基本的例子开始，逐步增加新的元素，以认识更到的内容。

 @subsubsection chapter03_121 1、main.cpp代码

 文件main.cpp的代码：
 @code
	#include <stdlib.h>

	#include "zglDevice.h"
	#include "zglEngine.h"

	#include "zglPlugin.h"
	#include "zgldemo.h"

	int main(int argc, char* argv[])
	{
		zglPlugin* demo = NULL;

		zglDevice* device = zglDevice::getDevice();

		zglEngine *glEngine = zglEngine::getEngine();

		glEngine->setClipLength(100.0f, 1500.0f);

		int w = 480;
		int h = 360;

		demo = new zglDemoPlugin();

		device->init(w, h, "Sample Template : zglEngine");

		glEngine->setPlugin(demo);
		glEngine->initiate();

		glEngine->resize(w, h);

		device->run();

		glEngine->finish();
		delete demo;
		demo = NULL;

		zglEngine::destroyEngine();
		zglDevice::destroyDevice();

		return 0;
	}
 @endcode

 相比上一节的例子，本节中增加了一个新的类型 zglPlugin, 程序创建特定的zglPlugin，并赋值给zglEngine。

 同上一节相比，主动刷新画面的代码已经被移走，zglEngine和zglPlugin配合，可以更合理地控制屏幕刷新的节奏，更合理地利用系统资源。

 @subsubsection chapter03_122 2、demo.h 代码
 @code
	#ifndef __zgl_engine_listener_demo_h__
	#define __zgl_engine_listener_demo_h__

	#include "zglPlugin.h"

	#include "zglTexture.h"
	#include "zglCube.h"

	class zglDemoPlugin : public zglPlugin
	{
		static const int m_focus = 990;
	public:

		virtual void onInit(void);

		virtual void onFini(void);

		virtual void onResize(int w, int h);

		virtual bool onUpdate(long time_val);

		virtual bool handleTouchEvent(int type, float x, float y, long time);

	public:

		zglTexture m_frame_tex;
		zglCube m_cube;

	};

	#endif
 @endcode

 这段代码，简单地定义了一个新的zglPlugin子类，定义了必须的五个接口函数。

 1、onInit()

 初始化Plugin中的资源、以及变量等。zglEngine在初始化的时候会顺带调用zglPlugin的onInit()接口。如果zglEngine在
初始化后才获得zglPlugin，那么需要手动地初始化zglPlugin。

 2、onFini()

 释放Plugin中的资源，如果Plugin会多次切换的话，还需要将部分资源的状态恢复，例如visible状态等。

 3、 onResize()

 由zglEngine通知Plugin外部视口的尺寸，Plugin在此函数中将各个图形元素放置到合适的位置。

 4、onUpdate()

 zglEngine在每个工作循环中都会有一次机会要求zglPlugin自我更新，如果zglPlugin本次没有更新，onUpdate就返回false，可以试图让zglEngine不刷新画面。

 需要注意的是，Engine会首先将更新事件传递给Widget，再传递给zglPlugin。当所有的Widget都不需要更新，而且zglPlugin也不需要更新时，zglEngine会不刷新画面。

 5、handleTouchEvent

 处理zglEngine传递来的Touch事件。

 Engine会首先在widget2D和widget3D中寻找正确的事件接收对象，并将Touch事件传递给Widget；当所有的Widget都不处理当前事件时，Engine才会考虑将事件传递给zglPlugin。

 除了上面五个必须的接口函数外，zglDemoPlugin还使用了两个变量，zglTexture和zglCube，其中zglCube是立方体。本例初步显示一个旋转的立方体。

 @subsubsection chapter03_123 3、demo.cpp 代码

 @code
	#include <stdlib.h>

	#include "zglUtil.h"
	#include "zgldemo.h"

	void zglDemoPlugin::onInit(void)
	{
		const char frame[] = "./data/frame.png";
		zglLoadPngTexture(frame, &m_frame_tex);

		m_cube.setMeshType(MESH_TEXTURE);
		m_cube.init(50.0f, 50.0f, 50.0f, &m_frame_tex, 1);
	}

	void zglDemoPlugin::onFini()
	{
	}

	void zglDemoPlugin::onResize(int w, int h)
	{
		m_cube.world().translate(0.0f, 0.0f, 590.0f);
	}

	bool zglDemoPlugin::handleTouchEvent(int type, float x, float y, long time)
	{
		return false;
	}

	bool zglDemoPlugin::onUpdate(long time_val)
	{
		m_cube.world().rotateX(0.04f);
		m_cube.world().rotateY(0.02f);
		m_cube.world().rotateZ(0.03f);
		return true;
	}
 @endcode

 先看函数 onInit():

 函数首先装载一个png文件，并绑定到纹理。这是装载资源的部分。

 然后Plugin对变量进行初始化

 @code
	m_cube.setMeshType(MESH_TEXTURE);
	m_cube.init(50.0f, 50.0f, 50.0f, &m_frame_tex, 1);
 @endcode

 cube首先被设置成MESH_TEXTURE模式。这里的cube，是网格的一种特别形式，MESH_TEXTURE是实体模式，MESH_WIREFRAME是框架模式, MESH_POINT是点阵模式。

 cube在初始化之前设置MESH模式，效率会相对高一些，因为可以避免对模式MESH模式的计算。

 cube的初始化函数前三个参数是长、宽和高。后面的是纹理资源，cube拥有六个面，可同时设置1、2、3、6个纹理，最后的参数表明纹理数。

 函数onUpdate()中，cube绕着自身的局部坐标系（cube中心）旋转。目前还没有增加对时间的响应。

 程序运行时显示一个旋转的立方体。以下是运行的截图，和实际代码相比，增加了一个cube来显示MESH_WIREFRAME模式，和实际代码的运行结果稍有不同：

 @image html simple-3.png "图 3-8 示例二"

 @subsubsection chapter03_124 4、增加GUI Widget

 本节中会增加多个Widget，按钮Widget(zglButton), LogoWidget(zglStatic)和位图文字(zglBitmapTextView)。

 首先在头文件demo.h中增加新的变量：

 @code
	zglFontInfo m_fontInfo;
	zglTexture m_font_tex;
	zglBitmapTextView m_title;

	zglTexture m_gui_tex;

	zglButton m_button;
	zglBitmapTextView m_button_title;

	zglStatic m_logo;
 @endcode

 新增加的按钮Widget和LogoWidget，都需要gui位图信息，这里通过 m_gui_tex 一并提供。

 按钮Widget需要响应实际的事件，因此需要增加新的 zglWidgetListener 。


 ButtonListener主要响应Touch事件，另外，任何一个zglWidget事实上都是zglWidgetListener的子类，所以zglWidget可以充当另外一个zglWidget的Listener，从而截获
其他zglWidget的事件。这一特点如果利用的好的话，可以达到很好的效果。

 zglBitmapTextView 显示的是不等宽的位图字体，相对于通常常见的等宽字体，更加真实。 zglBitmapTextView 需要外部提供字体纹理资源，同时还需要字体资源的宽度信息，

 声明过变量后，在demo.cpp文件中做好资源的初始化动作。
 @code
	void zglDemoPlugin::LoadFonts()
	{
		const char font_info[] = "./data/main.fnt";
		const char font[] = "./data/main.png";

		m_fontInfo.loadFontInfo(font_info);
		zglLoadPngTexture(font, &m_font_tex);
	}

	void zglDemoPlugin::LoadTextures()
	{
		const char gui[] = "./data/gui_bg.png";
		zglLoadPngTexture(gui, &m_gui_tex);

		const char frame[] = "./data/frame.png";
		zglLoadPngTexture(frame, &m_frame_tex);
	}
 @endcode

 因为资源的增加，为便于管理，新增了部分函数，LoadFonts()和LoadTextures()，这里可以忽略。

 以下函数对新增的widget进行初始化
 @code
 	m_logo.init(&m_gui_tex, 50, 50);
	m_logo.setTexCoord(0.0f, 187.0f/512, 50.0f/512, 237.0f/512);
	m_logo.setOffset(-128);
 @endcode

 这里logo是一个静态Widget( zglStatic )，该Widget默认只接收除更新事件， 其他事件，包括Touch事件，都不会响应。初始化函数使用的参数依次是纹理资源和尺寸。
使用setTexCoord()主要是因为logo只使用m_gui_tex的部分像素。

 @code
	m_title.init(&m_font_tex, &m_fontInfo, TEXTVIEW_ALIGN_CENTER);
	m_title.setColor(0xff0000ff);
	m_title.setEnable(false);
 @endcode

 title使用的是不等宽位图，该Widget由多个zglPrim3D组合而成，并且由Widget维护多个zglPrim3D的内容和可见性。初始化函数的参数是字体纹理，字体宽度信息和对齐方式。
而后设置字体颜色，最后设置该Widget状态外Disable，不接收除更新事件外的所有事件，从而避免系统不必要的开销。

 @code
	m_button_title.init(&m_font_tex, &m_fontInfo, TEXTVIEW_ALIGN_CENTER);

	m_button.init(&m_gui_tex, 120, 38);
	m_button.setTexCoord(0.0f, 0.0f, 233.0f/512, 38.0f/512, 235.0f/512, 0.0f);
 @endcode

 以下是本次代码改动后的截图：

 @image html simple-4.png "图 3-9 示例二"

 @subsubsection chapter03_125 5、增加事件响应

 这部分的小改动主要是增加WidgetListener的事件响应和Plugin的事件响应。

 首先是增加一个新的WidgetListener，将WidgetListener的变量设置到某一个或者多个Widget，当该Widget被点击时，就可以处理Event事件了。
 @code
 	class ButtonListener : public zglWidgetListener
	{
		zglDemoPlugin * m_plugin;

	public:
		ButtonListener(zglDemoPlugin* plugin);

		virtual bool onTouchEvent(zglTouchEvent *event);
	};
 @endcode

 在demo.cpp文件中创建Listener指针，并且赋值给Button。

 @code
	if(m_listener == NULL)
	{
		m_listener = new ButtonListener(this);
	}
	m_button.setListener(m_listener);
 @endcode

 最后是ButtonListener的代码：

 @code
	zglDemoPlugin::ButtonListener::ButtonListener(zglDemoPlugin* plugin)
	{
		m_plugin = plugin;
	}

	bool zglDemoPlugin::ButtonListener::onTouchEvent(zglTouchEvent *event)
	{
		if(event->m_event_type == TOUCH_UP)
		{
			m_plugin->updateMode();
			return true;
		}
		return false;
	}
 @endcode

 注意其中的函数updateMode()是第一次出现，主要是测试Button的效果，这个函数用来改变cube的Mesh模式，在实体(Texture)、骨架(wireframe)和点阵(points)三者之间切换。

 @code
	void zglDemoPlugin::updateMode()
	{
		switch(m_mode)
		{
		case MESH_POINTS:
		default:
			m_mode = MESH_TEXTURE;
			break;

		case MESH_TEXTURE:
			m_mode = MESH_WIREFRAME;
			break;

		case MESH_WIREFRAME:
			m_mode = MESH_POINTS;
			break;
		}

		m_cube.setMeshType(m_mode);
	}
 @endcode

 对于Plugin的Touch事件响应函数，这里做一下小的改动，每次事件到来时将logo向左移动10Pixel。

 @code
	bool zglDemoPlugin::handleTouchEvent(int type, float x, float y, long time)
	{
		if(type == TOUCH_UP)
		{
			m_logo.move(-10, 0.0f, 0.0f);
			return true;
		}
		return false;
	}
 @endcode

 以下是本次代码改动，并实际执行后的截图，其中cube的Mesh模式被改变，同时logo也移动了相当一段距离：

 @image html simple-5.png "图 3-10 示例二"

 对于Widget和Plugin的事件传递的先后顺序，Engine会先将事件传递给Widget，如果Widget处理事件，返回True，那么事件传递的过程就会结束，
Plugin将不收到任何事件;如果 Widget返回False，那么事件会继续传递给Plugin。

 在上面的例子中，如果将BottonListener代码的返回值加以修改
 @code
	if(event->m_event_type == TOUCH_UP)
	{
		m_plugin->updateMode();
		return false;
	}
 @endcode
 那么BottonListener和Plugin将同时都有机会处理TOUCH_UP事件，直接的效果是cube的模式和logo的位置同时被改变。

 多个Widget可以同时共享一个WidgetListener, 只需要对每一个参与的Widget设置一个可以区分的ID号，在WidgetListener中 #zglTouchEvent 会
包含事件传递Widget的ID号，从而可以区分哪个Widget该响应事件。

 @subsubsection chapter03_126 6、动画

 编写中 .............


 @section chapter03_2 3.2 Win32平台简单应用程序开发

 目前Win32平台上的开发主要通过<a href="http://www.mingw.org/">MINGW</a>完成，Win32 Device的移植也基本完成，Win32上的开发和Linux上的没有特别明显的差异，
因此MINGW的开发完全可以参考上一节的内容。需要格外留意之处的是Win32平台对OpenGL只支持到OpenGL 1.1， 如果没有<a href="http://glew.sourceforge.net/">glew</a>
的帮助，开发使用Buffer Object时需要格外注意。

 @image html simple-mingw.png "图 3-11 Mingw工作图"

 对于Microsoft Visual Studio开发环境，VC6.0的编译器过于老旧，无法支持目前的代码; 推荐VC2005以上的环境进行开发。

 目前没有VC2005, VC2008开发的需求，因此相关的示例暂缺。


*/
