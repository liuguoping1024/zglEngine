/**

 <!-- By guoping.liu -->

 @page chapter04 第四章 zglEngine的Android平台开发

 本章比较详细地解释在Android上使用zglEngine开发应用程序的相关知识，主要包括
 - @ref chapter04_1
 - @ref chapter04_2
 - @ref chapter04_3
 - @ref chapter04_4
 - @ref chapter04_5
 - @ref chapter04_6

 在前几章没有读完，对整个zglEngine还所知不多之前，不建议直接阅读本章的内容。

 @section chapter04_1 4.1 Android上使用OpenGL ES开发的实现

 在Android平台上，通常使用GLSurfaceView来实现OpenGL ES的应用(RenderScript也可以实现OpenGL ES应用)。

 通常Android中的View的刷新和绘制都是在UI线程中完成的，它的特点是实时性不强，用户发出了刷新画面的请求，画面真正
 刷新的时机基本上不可预测，更无从控制。

 SurfaceView拥有SurfaceHolder类，可以使用getHolder方法直接获取Canvas，可以直接从内存或者DMA等硬件接口取
 得图像数据。如果可以的话，surfaceView可以新起一个的单独的线程中，在线程中重新绘制画面。这一特点适用于需要主动更
 新画面的应用, 目前SurfaceView用来绘制OpenGL ES/3D内容和Video内容。

 GLSurfaceView继承自SurfaceView，它把SurfaceHolder作为nativeWindow与OpenGL ES环境绑定，从而可以将
 OpenGL ES渲染的结果显示在Canvas上。



 GLSurfaceView提供了下列特性：
 - 管理一个surface，这个surface就是一块特殊的内存，能直接混合(composite)到android的视图系统中。
 - 管理一个EGL display，它能让opengl把内容渲染到上述的surface上。
 - 用户自定义渲染器(render)来执行实际的渲染动作。
 - 让渲染器在独立的线程里运作，和UI线程分离。
 - 支持按需渲染(on-demand)和连续渲染(continuous)。
 - 一些可选工具，如绑定、调试、错误检查等。

 GLSurfaceView的类图如下图所示，其中GLThread为独立线程， EGLHelper负责Config, Display, Surface等的创建。

 @image html glsurfaceview-class.png "图 4-1 Android GLSurfaceView类图"

 用户需要自己实现Render， 以便完成实际所需要的渲染工作。

 @section chapter04_2 4.2 Android上使用OpenGL ES开发的时序图

 下图显示了SurfaceView, GLSurfaceView, GLThread, 以及用户Render互相协作的时序图，主要的过程都在GLThread中完成。虽然过程比较复杂，但作为用户来讲，需要了解的并不多。

 @image html glsurfaceview-sequence.png "图 4-2 Android GLSurfaceView序列图"

 初始化过程其实仅需要使用setRenderer(Renderer)设置一个渲染器(render)即可，有必要时，也可以修改GLSurfaceView的一些默认配置。

 以下代码是一个典型的Render的实现。

 @code
	public class ClearActivity extends Activity {
		@Override
		protected void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);
			mGLView = new GLSurfaceView(this);
			mGLView.setRenderer(new ClearRenderer());
			setContentView(mGLView);
		}

		@Override
		protected void onPause() {
			super.onPause();
			mGLView.onPause();
		}

		@Override
		protected void onResume() {
			super.onResume();
			mGLView.onResume();
		}

		private GLSurfaceView mGLView;
	}

	class ClearRenderer implements GLSurfaceView.Renderer {
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			// Do nothing special.
		}

		public void onSurfaceChanged(GL10 gl, int w, int h) {
			gl.glViewport(0, 0, w, h);
		}

		public void onDrawFrame(GL10 gl) {
			gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		}
	}
 @endcode

 以下是Android上使用OpenGL ES开发的部分开源资料：

 - [1] http://code.google.com/p/nehe-android/
 - [2] http://code.google.com/p/android-gl/
 - [3] http://code.google.com/p/min3d/
 - [4] http://code.google.com/p/floatingimage/

 @section chapter04_3 4.3 Android上使用zglEngine开发的时序图


 zglEngine在Android平台上的开发同X11/Win32开发不同之处很多，Android平台本质上是一个移动平台，设计的思路和PC机相当不同，

 - 首先，Android的一个应用程序所能访问的资源、所能利用的API都是有一定限制的，尤其是Font, image等，
 - 其次在工作时序上，Android的应用程序还需要做好在一个生命周期内多次Pause/Resume的处理。
 - 再一方面需要说明的是，Android平台上开发不再需要zglDevice，GLSurfaceView::GLThread事实上充当了Device的角色。

 进入本章前，开发者最好能有过Android开发的经验，并且能适当地接触Android上使用OpenGL ES开发的经历，对于GLSurfaceView的使用有一定的了解。

 zglEngine在本质上是一个被动的系统，需要外部的事件加以驱动。

 GLSurfaceView已经维护了一个完整的GLThread和Runnable的队列，基本上满足了zglEngine的需求。这里所要做的是将GLThread的
 时序和zglEngine的时序联系起来，主要的接口有：
 - onDrawFrame： 屏幕绘制;
 - onSurfaceCreated： Engine创建，程序初始化;
 - onSurfaceChanged： 设置视口
 - onSurfaceDestroy： 处理程序多次Pause/Resume的资源问题
 - onUpdate： 画面数据更新

 其中后面两个是新增加的，onUpdate()是为了更好地利用zglEngine的特性、提高程序效率添加的。如果本次画面更新没有变化，不需要刷新时，onDrawFrame()会被直接跳过。至于onSurfaceDestroy()，处理资源相关，状态相关的信息，以便多次Pause/Resume的操作。

 下图展示的是一个典型的zglEngine和Android的系统融合、协作的时序和过程， 对于前面提到的新增函数，图中也特意做了标识。

 @image html glsurfaceview-sequence2.png "图 4-3 Android zglEngine序列图"

 <b>特别再次说明:</b>

 Android上应用zglEngine的一个特点, 也是难点之一:
 @verbatim

 要做好在一个生命周期内多次Pause/Resume的处理，包括纹理资源，buffer资源，widget状态，状态机等。
 @endverbatim

 以下是一个典型的应用程序输出的LOG，比较精确地反应了zglEngine在Android平台工作的时序。
 @verbatim

 I/ActivityManager(  799): Start proc com.ziilabs.zglEngineApp.demo1 for activity com.ziilabs.zglEngineApp.demo1/.main: pid=1565 uid=10077 gids={}
 D/ZiiSensors( 1565): In sensors_get_sensors_list
 D/ZiiSensors( 1565): Request to open data
 I/GLThread( 1565): noticed surfaceView surface lost tid=23
 E/GL11SurfaceView( 1565): API:: void surfaceCreated(SurfaceHolder holder)
 I/GLThread( 1565): noticed surfaceView surface acquired tid=23
 E/GL11SurfaceView( 1565): surfaceCreated
 E/GL11SurfaceView( 1565): API:: void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
 I/Main thread( 1565): onWindowResize waiting for render complete.
 D/libEGL  ( 1565): loaded /system/lib/egl/libGLES_ziilabs.so
 I/Main thread( 1565): onWindowResize waiting for render complete.
 D/gralloc ( 1565): gralloc_device_open Mod:0x815031ac Dev:0x205dc0 gpu0
 D/Ziilabs ( 1565): Creating Window 800 x 480
 I/RenderView( 1565): onSurfaceCreated
 D/dalvikvm( 1565): Trying to load lib /data/data/com.ziilabs.zglEngineApp.demo1/lib/libzglEngine_jni.so 0x462aaab8
 D/dalvikvm( 1565): Added shared lib /data/data/com.ziilabs.zglEngineApp.demo1/lib/libzglEngine_jni.so 0x462aaab8
 E/ZGLEngine-JNI( 1565): zglEngine, version 201104291756, Copyright (C) ZiiLABS Pte Ltd.
 E/Plugin  ( 1565): onInit
 D/ZGLEngine-JNI( 1565): AndroidBitmap 512 x 512 x 32
 D/ZGLEngine-JNI( 1565): AndroidBitmap 512 x 512 x 32
 I/ZGLEngine-JNI( 1565): AndroidBitmap 128 x 20 x 32
 I/ZGLEngine-JNI( 1565): AndroidBitmap 256 x 32 x 32
 I/RenderView( 1565): onSurfaceChanged: 800, 480
 E/Plugin  ( 1565): onResize
 I/Main thread( 1565): onWindowResize waiting for render complete.
 I/ActivityManager(  799): Displayed activity com.ziilabs.zglEngineApp.demo1/.main: 599 ms (total 599 ms)
 W/KeyCharacterMap( 1565): No keyboard for id 0
 W/KeyCharacterMap( 1565): Using default keymap: /system/usr/keychars/qwerty.kcm.bin
 I/GLThread( 1565): releasing EGL surface because paused tid=23
 I/GL11SurfaceView( 1565): mEglHelper.destroySurface(1)
 I/RenderView( 1565): onSurfaceDestroy: false
 I/GL11SurfaceView( 1565): destroySurface
 E/GL11SurfaceView( 1565): API:: void surfaceDestroyed(SurfaceHolder holder)
 I/GLThread( 1565): noticed surfaceView surface lost tid=23
 E/GL11SurfaceView( 1565): surfaceDestroyed
 I/RenderView( 1565): onSurfaceDestroy: true
 E/Plugin  ( 1565): onFini
 I/GL11SurfaceView( 1565): mEglHelper.destroySurface(3)
 I/GL11SurfaceView( 1565): destroyContext
 I/GL11SurfaceView( 1565): eglTerminate
 I/GL11SurfaceView( 1565): requestExitAndWait
 @endverbatim


 @section chapter04_4 4.4 Android平台的zglEngine开发模式

 zglEngine在Android平台目前有如下几种开发应用模式:

 - @ref chapter04_4_1
 - @ref chapter04_4_2

 @subsection chapter04_4_1 4.4.1 通用接口开发模式

 这种开发模式的特点是应用程序的逻辑主要在Java层实现。

 具体做法是将zglEngine C++类的接口一对一映射到Java类的接口，通常也称之为Java语言绑定。用户使用绑定好的Java类进行开发，对于用户来讲，这种开发方式同传统的Android应用程序开发方式没有太大区别。

 @subsection chapter04_4_2 4.4.2 NDK应用程序开发模式

 这种模式的特点是整个应用程序的逻辑主要用C++来实现。

 具体做法是先用C++实现将整个应用程序的逻辑，或者是特殊控件的逻辑，尔后只通过几个特定的接口与Android进行资源和事件的通讯。

 @section chapter04_5 4.5 Android平台通用接口开发模式

 zglEngine 通过JNI的方式，对部分主要的模块进行了Java绑定，从而是开发者可以以开发普通的Android应用程序的做法来开发程序。

 zglEngine对Android中附带的GLSurcefaceView进行了小的改动，一来扩充功能，二来提高代码的稳定性，主要改动如下：
 -# 对Display, Config, Context等，尽量多次使用，减少GC，提高效率。
 -# 对Surface的申请，该为多次尝试，提高稳定性
 -# 对Renderer接口，以便更好地符合Android应用程序的生命，同时更适合zglEngine。

 本例通过RenderView.java和DemoPlugin.java两个文件来演示通用开发模式。其中RenderView主要实现了GLSurfaceView的Render接口，以及事件传递等。DemoPlugin主要负责应用程序的逻辑。

 这里需要说明的是，下面的代码增加了多点触控的功能，另外从DemoPlugin代码中可以看到如何增加新的接口到Plugin。

 @subsection chapter04_5_1 4.5.1、 RenderView

 以下是RenderView的主要代码，后面有比较详细的说明。
 @code
	 public class RenderView extends GL11SurfaceView implements
		 GL11SurfaceView.Renderer, ScaleGestureDetector.OnScaleGestureListener {
		 private final static String TAG = "RenderView";

		 private Engine m_engine;

		 private ScaleGestureDetector m_ScaleGestureDetector = null;

		 float m_scale;

		 int m_width;
		 int m_height;

		 private Context m_ctx;
		 DemoPlugin1 m_app = null;

		 public RenderView(Context context) {
			 super(context);

			 m_ctx = context;

			 m_ScaleGestureDetector = new ScaleGestureDetector(m_ctx, this);

			 //设置Config,这一Config能在大多数Android设备上运行
			 setEGLConfigChooser(8, 8, 8, 8, 16, 0);
			 setRenderer(this);
			 setRenderMode(RENDERMODE_WHEN_DIRTY);
		 }

		 public RenderView(Context context, AttributeSet attrs) {
			 super(context, attrs);

			 m_ctx = context;

			 //设置Config,这一Config能在大多数Android设备上运行
			 setEGLConfigChooser(8, 8, 8, 8, 16, 0);

			 setRenderer(this);
			 setRenderMode(RENDERMODE_WHEN_DIRTY);
		 }

		 public void onDrawFrame(GL11 gl) {
			 m_engine.renderScreen();//绘制渲染区域
		 }

		 public void onSurfaceCreated(GL11 gl, EGLConfig config) {
			 Log.e(TAG, "onSurfaceCreated");

			 if (m_engine == null) {
				 m_engine = new Engine();//创建Engine

				 m_app = new DemoPlugin1(m_ctx);//创建Plugin

				 m_engine.setPlugin(m_app);//设置Plugin
				 m_engine.init();
			 }
		 }

		 public void onSurfaceChanged(GL11 gl, int width, int height) {
			 m_width = width;
			 m_height = height;
			 m_engine.resize(width, height);
		 }

		 //bquit为真时，表示GLThread将真的结束运行, GLSurfaceView也会被终止
		 public void onSurfaceDestroy(boolean bquit) {
			 Log.i(TAG, "onSurfaceDestroy: " + bquit);

			 if (realDestroy) {
				 m_engine.fini();
				 m_engine = null;
			 }
		 }

		 public boolean onUpdate(GL11 gl, long time) {
			 m_engine.update(time);
			 requestRender();
			 return true;
		 }

		 //传递事件
		 @Override
		 public boolean onTouchEvent(MotionEvent event) {
			 final int x = (int) event.getX();
			 final int y = (int) event.getY();
			 final long time = event.getEventTime();
			 int action = event.getAction();

			 switch (action) {
				 case MotionEvent.ACTION_DOWN:

				 queueEvent(new Runnable() {
					 public void run() {
						 handleTouchEvent(TouchEvent.TOUCH_DOWN, x, y, time);
					 }
				 });
				 break;

				 case MotionEvent.ACTION_MOVE:

				 queueEvent(new Runnable() {
					 public void run() {
					 handleTouchEvent(TouchEvent.TOUCH_MOVE, x, y, time);
					 }
				 });
				 break;

				 case MotionEvent.ACTION_UP:
				 queueEvent(new Runnable() {
					 public void run() {
						 handleTouchEvent(TouchEvent.TOUCH_UP, x, y, time);
					 }
				 });
				 break;
			 }

			 m_ScaleGestureDetector.onTouchEvent(event);
			 return true;
		 }

		 private boolean handleTouchEvent(int type, int x, int y, long time) {
			 float mx = x - (m_width >> 1);
			 float my = (m_height >> 1) - y;

			 return m_engine.handleTouchEvent(type, (int) mx, (int) my, time);
		 }

		 //以下是多点触控的处理

		 @Override
		 public boolean onScale(ScaleGestureDetector detector) {
			 float scale = detector.getScaleFactor();

			 m_scale = scale * m_scale;
			 m_app.updateScale(m_scale);
			 return true;
		 }

		 @Override
		 public boolean onScaleBegin(ScaleGestureDetector detector) {
			 m_scale = 1.0f;
			 m_app.updateScale(m_scale);
			 return true;
		 }

		 @Override
		 public void onScaleEnd(ScaleGestureDetector detector, boolean cancel) {
			 m_scale = 1.0f;
			 m_app.updateScale(m_scale);
		 }
	 }
 @endcode

 对于GLSurfaceView, RENDERMODE_WHEN_DIRTY是一个很重要的模式。在实际工作时，RENDERMODE_WHEN_DIRTY会尽量节省CPU的占用事件，尽量推荐使用RENDERMODE_WHEN_DIRTY。
 @code
 setRenderMode(RENDERMODE_WHEN_DIRTY);
 @endcode

 由此带来画面可能不更新的问题，解决方案是在多发送一次requestRender()的请求，如果有外部事件输入，也需要多发送一次requestRender()的请求。
 @code
	 public boolean onUpdate(GL11 gl, long time) {
		 m_engine.update(time);
		 requestRender();
		 return true;
	 }
 @endcode

 对于函数handleTouchEvent()，onTouchEvent将外部事件接收，以Runnable的方式将事件发送给zglEngine。

 这里需要说明一个线程同步问题，因为存在GLThread和UI线程， 其他线程向GLThread发送请求时一定要通过Runnable queue, 而由GLThread返回的，最好也通过Runnable或者Listener进行同步，否则，可能会因为不同步造成画面出现不正常的黑线，黑块等，因此通常建议OpenGL ES的部分不要和Android中2D的部分混合工作。

 需要注意的是坐标的转换关系，其他的转换公式基本上是错误的。

 @code
 float mx = x - (m_width >> 1);
 float my = (m_height >> 1) - y;
 @endcode

 @subsection chapter04_5_2 4.5.2、 DemoPlugin

 这里的DemoPlugin正是之前提到的zglPlugin的Java绑定，使用方法可以参考前文 @ref chapter03_12 "《使用zglPlugin开发》"
 @code
	 public class DemoPlugin1 extends Plugin  {

		 private final static int m_default_z = 990;
		 private Image m_image;
		 private Texture m_texture_1;
		 private Texture m_texture_2;
		 private ImageView m_imageView;
		 private StringTexture m_titleTexture;

		 private StringTexture m_strinTexture;
		 private Button m_button_1;
		 private Button m_button_2;

		 private ImageView m_photo;

		 private ShortcutView m_shortcut;

		 public DemoPlugin1(Context context) {
			 super(context);

			 m_imageView = null;
		 }

		 @Override
		 public void onInit() {
			 Log.e("Plugin", "onInit");

			 if (m_imageView == null)
			 {
				 m_image = new Image();
				 m_texture_1 = new Texture();
				 m_texture_2 = new Texture();
				 m_imageView = new ImageView();
				 m_photo = new ImageView();

				 m_button_1 = new Button();
				 m_button_2 = new Button();

				 m_shortcut = new ShortcutView();

				 WidgetListener l = new WidgetListener() {
				 public boolean onTouchEvent(TouchEvent event) {
						 if (event.m_event_type == 0) {
							 Log.w("WidgetListener", "The Widget id = " + event.m_widget);
							 return true;
						 }
					 return false;
					 }
				 };

				 m_button_1.setWidgetListener(l);
				 m_button_2.setWidgetListener(l);
				 m_shortcut.setWidgetListener(l);

				 m_shortcut.setID(8888);
				 m_button_1.setID(1001);
				 m_button_2.setID(1002);
			 }

			 Bitmap bmp = util.getTextureFromPngResource(mContext, R.drawable.frac);

			 m_texture_1.makeTexture(bmp);
			 bmp.recycle();

			 bmp = util.getTextureFromPngResource(mContext, R.drawable.gui_bg);
			 m_texture_2.makeTexture(bmp);
			 bmp.recycle();

			 StringTexture.Config cg = new StringTexture.Config();
			 cg.fontSize = 13;
			 cg.height = 20;
			 cg.width = 128;
			 cg.sizeMode = StringTexture.Config.SIZE_EXACT;
			 cg.xalignment = StringTexture.Config.ALIGN_HCENTER;
			 cg.yalignment = StringTexture.Config.ALIGN_VCENTER;
			 m_strinTexture = new StringTexture("Android应用程序", cg);
			 m_strinTexture.updateTexture();

			 StringTexture.Config cg2 = new StringTexture.Config();
			 cg2.fontSize = 32;
			 cg2.height = 32;
			 cg2.width = 256;
			 cg2.sizeMode = StringTexture.Config.SIZE_EXACT;
			 cg2.xalignment = StringTexture.Config.ALIGN_HCENTER;
			 cg2.yalignment = StringTexture.Config.ALIGN_VCENTER;
			 m_titleTexture = new StringTexture("Android测试程序", cg2);
			 m_titleTexture.updateTexture();

			 m_imageView.init(m_titleTexture, 256, 32);
			 m_imageView.setTexCoord(0.0f, 0.0f, m_titleTexture.getU(),
			 m_titleTexture.getV());

			 m_photo.init(m_texture_1, 200, 200);

			 m_button_1.init(m_texture_2, 120, 38);
			 m_button_2.init(m_texture_2, 120, 38);

			 m_button_1.setTexCoord(0.0f, 0.0f, 233.0f / 512, 38.0f / 512,
			 235.0f / 512, 0.0f);
			 m_button_2.setTexCoord(0.0f, 0.0f, 233.0f / 512, 38.0f / 512,
			 235.0f / 512, 0.0f);

			 m_shortcut.setFocusColor(0xff0000ff);
			 m_shortcut.init(m_texture_1, m_strinTexture, 200, 94);
			 m_shortcut.setTitleTexCoord(0.0f, 0.0f, m_strinTexture.getU(),
			 m_strinTexture.getV());
			 m_shortcut.setTitleWidth(128);
		 }

		 @Override
		 public void onFini() {
		 }

		 @Override
		 public void onResize(int w, int h) {
			 Log.e("Plugin", "onResize");

			 m_imageView.locate(0.0f, h * 0.4f, m_default_z);

			 m_button_1.locate( -w / 4 , 0.0f, m_default_z);
			 m_button_2.locate(w / 4 , 0.0f, m_default_z);

			 m_shortcut.locate(0.0f, h * 0.1f, m_default_z);

			 m_photo.locate(0.0f, - h * 0.25f, m_default_z);
		 }

		 //永远更新
		 @Override
		 public boolean onUpdate(long time_val) {
			 return true;
		 }

		 //不处理事件
		 @Override
		 public boolean handleTouchEvent(int type, int x, int y, long time) {
			 return false;
		 }

		 //新增接口，多点触控的处理
		 public void updateScale(float s)
		 {
			 m_photo.setDimension((int)(200 * s), (int)(200 * s));
		 }
	 }
 @endcode

 我们在前文显示文字的时候，使用的是 zglBitmapTextView , zglEngine没有提供诸于Freetype之类的文字解决方案，主要是考虑到目标平台主要是手持设备，在这些设备上基本会提供比较完备的文字解决方案，zglEngine只要有足够的开放性和兼容性就可以达到”借力“的效果；而且从实际情况来说，单一的应用程序是否具备访问系统字库的权限，也存在很大的变数，因此zglEngine目前没有提供文字解决方案。

 在zglEngine中，X11/Win32平台都具备了初步的图像装载能力。但在Java绑定的部分中，图像装载能力被直接删除，主要是考虑到，移动平台上单一应用程序访问应用程序图像资源的权限和能力都可能会受限制，尤其是Android的apk设置，所以，图像装载这部分，也采用了”借力“的方案。

 图像装载通过Resource来完成，通过适当地封装以便于使用。

 @code
 Bitmap bmp = util.getTextureFromPngResource(mContext,R.drawable.frac);
 @endcode

 StringTexture也借用了Android中绘图部分，例如Paint等。首先将字符串根据指定的字体转化为位图，然后将位图传递给zglImage，最后绑定到zglTexture，从而可以当作普通的纹理为各图形元素使用。这里的函数getTextureFromPngResource()将资源文件统一使用RGBA8888的格式，建议统一使用该接口来装载资源。

 代码中展示了多个Widget共享使用单一WidgetListener的例子，每个Widget设置不同的ID，从而可以最终在WidgetListener中加以区分。

 @image html eclipse-1.png "图 4-4 Android运行截图"

 以上是本例的截图，Widget事件响应的Log可以通过DDMS或者abd logcat获取, 注意其中Widget ID：

 @verbatim

 03-11 11:34:40.510: WARN/WidgetListener(711): The Widget id = 8888
 03-11 11:34:43.330: WARN/WidgetListener(711): The Widget id = 8888
 03-11 11:34:49.060: WARN/WidgetListener(711): The Widget id = 1002
 03-11 11:34:50.690: WARN/WidgetListener(711): The Widget id = 1001
 03-11 11:34:51.310: WARN/WidgetListener(711): The Widget id = 1001
 03-11 11:34:52.140: WARN/WidgetListener(711): The Widget id = 1002
 03-11 11:34:52.540: WARN/WidgetListener(711): The Widget id = 8888
 @endverbatim

 @section chapter04_6 4.6 Android平台NDK应用程序开发模式

 Android平台上NDK开发涉及的内容较多，本节只关注zglEngine相关的开发知识，与此相关的JNI知识并不是被考虑的范围。JNI的开发资料可以从<a href="http://java.sun.com/docs/books/jni/">java.sun.com</a>站点上获取。

 本节以程序Stackup中的实现为例，以例子中的部分文件的配置、代码，大致说明一些要注意的要点。

 由于这里NDK实现的整个应用程序，或者是关键应用函数集，都采用C++完成，因此，这里涉及的程序逻辑可以先在Linux，window或者其他平台上开发，开发中需要事先把与Android相关的部分隔离开来，然后完成对应平台上主要逻辑、主要界面的调试，这样后再考虑与Android平台的集成。后期与Android平台的集成，可以视为一次特殊的移植(Porting)工作。

 在本节中Stackup已经完成Linux平台上的调试，程序的主要逻辑、资源、界面布局等都已经调试完毕，以下是在Linux平台上的运行截图：

 @image html eclipse-2.png "图 4-5 Linux运行截图"


 @subsection chapter04_6_1 4.6.1 配置文件Application.mk

 通常情况下，使用NDK时可以不指定Application.mk，但是由于Android需要使用“libjnigraphics.so”来访问Bitmap, 而这个库从Android 2.2(android-8)才开始提供在NDK中，所以我们需要指定Platform以免出现不必要的错误。另外由于Android平台所使用ARM CPU等级也越来越高，因此也有必要使用好的指令集，故而也有必要指定ARMv5TE或者ARMv7-A的指令集。
 @code
	 APP_PLATFORM := android-8

	 # Build both ARMv5TE and ARMv7-A machine code

	 APP_ABI := armeabi armeabi-v7a
 @endcode

 @subsection chapter04_6_2 4.6.2 配置文件Android.mk

 Android.mk是重要的编译文件，重要程度相当与Makefile。Android通过各级的Android.mk文件来形成完整的编译系统。

 这里以一个典型的zglEngine的应用程序的Android.mk文件作为实例：
 @code
	 LOCAL_PATH := $(call my-dir)

	 include $(CLEAR_VARS)

	 #输出的.so文件，全名为libzglGameStackup_jni.so
	 LOCAL_MODULE := zglGameStackup_jni

	 #这里包含的是JNI固有的文件，Engine用来传递事件和完成特定时序，处理得当的话，此处文件可以省掉。
	 LOCAL_SRC_FILES := \
			zgl_jni.cpp \
			zgl_jni_engine.cpp

	 #应用程序的C++文件
	 LOCAL_SRC_FILES += \
			zgl_jni_demo_resource.cpp \
			game_level_jni.cpp \
			game_helper_jni.cpp \
			zglStatemachine.cpp \
			game_brick_factory.cpp \
			game_brick_info.cpp \
			game_brick.cpp \
			game_helper.cpp \
			game_level.cpp \
			game_parameter.cpp \
			game_plugin.cpp \
			game_resource.cpp \
			game_util.cpp \
			game_window.cpp \
			game_world.cpp

	 #zglEngine的相关文件
	 LOCAL_SRC_FILES += \
			zglxxx.cpp

	 #所需要的库
	 LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -ljnigraphics -lbox2d

	 include $(BUILD_SHARED_LIBRARY)
 @endcode

 在这里的配置文件中，源代码相关的部分, "LOCAL_SRC_FILES", 分三次完成，主要是考虑代码的来源不同，有应用程序本身的，有来自与zglEngine的，还有JNI本身有关系的，这样组织后便于文件管理。

 输出文件"LOCAL_MODULE"的名字要与Java层中Load该so的代码名字一致，否则就会出现问题。

 @code
	static {
		try {
			System.loadLibrary("zglGameStackup_jni");
		} catch (UnsatisfiedLinkError ule) {
			System.err.println("WARNING: zglEngine: Could not load libzglGameStackup_jni.so");
		}
	}
 @endcode

 对于程序所需要的库 "LOCAL_LDLIBS", GLESv1_CM是OpenGL ES相关库，log是调试用的log库，jnigraphics提供Bitmap的访问，最后的box2d是第三方的物理引擎库(http://www.box2d.org/)，也是Stackup中一个极其重要的库。

 @subsection chapter04_6_3 4.6.3 代码与Android平台的融合

 从上面配置文件中"LOCAL_SRC_FILES"中的文件来看，在Linux上的几个主要的以game开头的文件都基本上予以保留，尔后新增了三个文件来与Android交互。

 - zgl_jni_demo_resource.cpp: 传递所需的png图片到底层C++代码，因为这些图片资源打包到apk中，得借助外力才能访问。Linux下的代码是直接访问文件。
 - game_level_jni.cpp:关卡信息，关卡信息主要保存在特定的xml中，而不是像Linux代码一样采用硬编码，增加灵活性。
 - game_helper_jni.cpp:当前关卡的进度访问，用于保存进度，C++代码部分不能访问数据库，也得借用外力。

 此外，game_helper.cpp中有部分接口的实现有改动，主要是音量和声音相关的操作。

 这样改动后，整个程序的NDK部分基本就告结束，由于主要的逻辑在Linux上完成，因此相关开发有比较高质量和比较快速度的保证。

 与此相关的Java层的实现，除了必备的事件传递，主要需要完成的工作是图片信息的访问、xml关卡信息的读取、关卡进度的数据保存、以及音量声音的操作，这样后一个完整的应用程序最终成形，以下是该程序在Android上运行的截图。

 @image html eclipse-3.png "图 4-5 Android运行截图"

 参考文档：

 - [1] http://www.glprogramming.com/red/
 - [2] http://java.sun.com/docs/books/jni/


 */
