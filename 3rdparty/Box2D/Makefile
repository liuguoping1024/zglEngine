
include Makefile.in.$(shell uname)

OUTNAME  = ../../lib/libBox2D.a

INCLUDES := $(INCLUDES) -I../

SRC_CXX=\
	Dynamics/Joints/b2DistanceJoint.cpp \
	Dynamics/Joints/b2FrictionJoint.cpp \
	Dynamics/Joints/b2GearJoint.cpp \
	Dynamics/Joints/b2Joint.cpp \
	Dynamics/Joints/b2LineJoint.cpp \
	Dynamics/Joints/b2MouseJoint.cpp \
	Dynamics/Joints/b2PrismaticJoint.cpp \
	Dynamics/Joints/b2PulleyJoint.cpp \
	Dynamics/Joints/b2RevoluteJoint.cpp \
	Dynamics/Joints/b2RopeJoint.cpp \
	Dynamics/Joints/b2WeldJoint.cpp \
	Dynamics/Contacts/b2CircleContact.cpp \
	Dynamics/Contacts/b2Contact.cpp \
	Dynamics/Contacts/b2ContactSolver.cpp \
	Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
	Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
	Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
	Dynamics/Contacts/b2LoopAndCircleContact.cpp \
	Dynamics/Contacts/b2LoopAndPolygonContact.cpp \
	Dynamics/Contacts/b2PolygonContact.cpp \
	Dynamics/b2Body.cpp \
	Dynamics/b2ContactManager.cpp \
	Dynamics/b2Fixture.cpp \
	Dynamics/b2Island.cpp \
	Dynamics/b2World.cpp \
	Dynamics/b2WorldCallbacks.cpp \
	Common/b2BlockAllocator.cpp \
	Common/b2Math.cpp \
	Common/b2Settings.cpp \
	Common/b2StackAllocator.cpp \
	Collision/Shapes/b2CircleShape.cpp \
	Collision/Shapes/b2EdgeShape.cpp \
	Collision/Shapes/b2LoopShape.cpp \
	Collision/Shapes/b2PolygonShape.cpp \
	Collision/b2BroadPhase.cpp \
	Collision/b2CollideCircle.cpp \
	Collision/b2CollideEdge.cpp \
	Collision/b2CollidePolygon.cpp \
	Collision/b2Collision.cpp \
	Collision/b2Distance.cpp \
	Collision/b2DynamicTree.cpp \
	Collision/b2TimeOfImpact.cpp


OBJ=$(SRC_CXX:.cpp=.o) $(SRC_C:.c=.o)

all:    $(OBJ)
	$(LIB) $(OUTNAME) $(OBJ) 

clean:
	rm -f *.a $(OBJ) 
	rm -f $(OUTNAME)
	
%.o: %.cpp
	$(CPP) -c $(CXXFLAGS) $(DEFINES) $(INCLUDES) $*.cpp -o $@

.PHONY : clean


