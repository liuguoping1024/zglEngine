
# zglEngine.a Makefile

include ../Makefile.in.$(shell uname)


OUTNAME  = ../lib/libzglEngine.a

INCLUDES := $(INCLUDES) -I../include

SRC_CXX=\
	zglAlphaAnimation.cpp \
	zglDebugText.cpp \
	zglLight.cpp \
	zglMaterial.cpp \
	zglPrim.cpp \
	zglRotateAnimation.cpp \
	zglTranslateAnimation.cpp \
	zglWidgetList.cpp \
	zglEngine.cpp \
	zglPlugin.cpp \
	zglPluginEx.cpp \
	zglPrimList.cpp \
	zglUtil.cpp \
	zglWidgetListener.cpp \
	zglAnimation.cpp \
	zglEngineDebugger.cpp \
	zglMatrix.cpp \
	zglPrimParticle.cpp \
	zglVector.cpp \
	zglWidgetParticle.cpp \
	zglAnimationGroup.cpp \
	zglMesh.cpp \
	zglQuadMesh.cpp \
	zglVertex.cpp \
	zglX11Device.cpp \
	zglAnimator.cpp \
	zglImage.cpp \
	zglOglCore.cpp \
	zglScreen.cpp \
	zglWidget2D.cpp \
	zglBitmapTextView.cpp \
	zglImageLoader.cpp \
	zglOglesCore.cpp \
	zglSlider.cpp \
	zglWidget3D.cpp \
	zglButton.cpp \
	zglImageView.cpp \
	zglPrim2D.cpp \
	zglStatic.cpp \
	zglStaticView.cpp \
	zglWidget.cpp \
	zglCube.cpp \
	zglSphere.cpp \
	zglInterpolator.cpp \
	zglEaseInterpolator.cpp \
	zglPrim3D.cpp \
	zglPrimQuad.cpp \
	zglTexture.cpp \
	zglWidgetGroup.cpp \
	zglWin32Device.cpp

OBJ=$(SRC_CXX:.cpp=.o) $(SRC_C:.c=.o)

all:    $(OBJ)
	$(LIB) $(OUTNAME) $(OBJ) 
	ranlib $(OUTNAME)

clean:
	rm -f *.a $(OBJ) 
	rm -f $(OUTNAME)
	
%.o: %.cpp
	$(CPP) -c $(CXXFLAGS) $(DEFINES) $(INCLUDES) $*.cpp -o $@

.PHONY : clean

