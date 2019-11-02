#zglEngine - README
===============================================================================

zglEngine是一个基于OpenGL ES 1.1的跨平台GUI引擎，由于年代久远，多不维护，因此挑选较老的一个比较完整的版本，稍作整理后开源。


#REQUIREMENTS
-------------------------------------------------------------------------------

zglEngine currently requires:
 - OpenGL >= 1.5 (OpenGL 1.x)
 - or OpenGL >= 2.0+ (OpenGL es 2.0)

On X11, the development package requires:
 - libx11-dev
 - mesa-common-dev

On X11/Win32, the development library is requires:
 - libpng 1.2.x (http://www.libpng.org/)
 - zlib (http://www.zlib.net/)

libpng & zlib is no neccessary for Android.


#ARCHITECT

设计图-1

![Image of architect-main](https://github.com/liuguoping981/zglEngine/raw/master/manual/image/architect-main.png)

设计图-2

![Image of overview-detail](https://github.com/liuguoping981/zglEngine/raw/master/manual/image/overview-detail.png)
