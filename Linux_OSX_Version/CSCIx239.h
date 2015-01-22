#ifndef CSCIx239
#define CSCIx239

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
//Be sure to add the ode_singled.dll file to the same place as the executable!
#include <ode/ode.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif
//Variables
//Declare physics globals, for whatever reason it fails if these are global
dWorldID world;
dSpaceID spacePhy;
dJointGroupID contactgroup;
dGeomID ground; //Holds the geometry of the plane, or the ground location
//Ball1 geometries
dMass ball_mass;
dGeomID ball_geom;
dBodyID ball_body;
//Sphere geometries
dMass ball_mass2;
dGeomID ball_geom2;
dBodyID ball_body2;
//Sphere geometries
dMass ball_mass3;
dGeomID ball_geom3;
dBodyID ball_body3;
//Sphere geometries
dMass ball_mass4;
dGeomID ball_geom4;
dBodyID ball_body4;

struct VECTOR
{
    float x;
    float y;
    float z;
};
struct VECTOR tempVect;
//Physics geometries macros
#define GEOMSPERBODY 1  //Maximum number of geometries per body
#define DENSITY 0.01 //Density of our objects
#define MAX_CONTACTS 12
//Functions
void         Print(const char* format , ...);
void         Fatal(const char* format , ...);
void         ErrCheck(const char* where);
unsigned int LoadTexBMP(const char* file);
void         Project(int view, double asp,double dim);
int          LoadOBJ(const char* file);
int          FramesPerSecond(void);
double       Elapsed(void);
void         CreateShader(int prog,const GLenum type,const char* file);
void         PrintProgramLog(int obj);
int          CreateShaderProg(const char* VertFile,const char* FragFile);
void         initODE();
void         simLoop();
void         closeODE();//close ode returning memory alloc
void         nearCallback (void *data, dGeomID o1, dGeomID o2);


#ifdef __cplusplus
}
#endif

#endif

