/*
 ============================================================================
 Name        : NDCtoRGB2.c
 Author      : Russ Mehring
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "CSCIx239.h"

static int view = 0;//Control ortho or persp views
static int j = 0;
static int k = 0;
static int shader[] = {0,0}; //Shader program storage, initialized array point 0 with non-existent shader prog (0,0)
static int alpha=100;//  Transparency
static int model;        //  Model display list
static int model2;        //  Model display list
static int model3;        //  Model display list
static int skull = 0; //Drop Skull
unsigned int texture[3];  //Texture names
static float zoomFactor = 1.0;//Use for zooming in
static double ar = 1.0; //Aspect Ratio
static double dim = 5.0;//Dimension
static double th = 0.0;  //  Rotation angle
static double ph = 0.0; //Elevation of view angle for gluPerspective
static double s  = 10;
static double b  = 2.6666;
static double r  = 28;
static double values[150000];
static double orbs[4500];
static double * values1;
static double * reference;
//Sphere stuff
/*
 *  Draw vertex in polar coordinates with normal
 */
void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   glTexCoord2d(th/360.0,ph/180.0+0.5);//Bind the texture to each column in the sphere's drawing
   glVertex3d(x,y,z);
}

/*
 *  Draw a cube xyz = translate positions (want 9)
 */
static void Cube(double x, double y, double z)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(0.5, 0.025,1);
   //  Front
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,+1);
   glTexCoord2f(0,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,+1);
   glEnd();
   //  Back
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   glPopMatrix();
}

//Draw skull to 'kill' the orbs
void skullDrop()
{
    const dReal* rot;
	const dReal* pos;
	GLfloat matrix[16];
	glPushMatrix();
	//Translate attractor inside of bouncing ball
	rot = dBodyGetRotation(ball_body4);
	pos = dBodyGetPosition(ball_body4);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];//x
	matrix[13] = pos[1];//y
	matrix[14] = (pos[2] - 0.25);//z
	matrix[15] = 1;
	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);

    //Scale the model
    glScaled(1,1,1);
    //  Draw the model
    glCallList(model3);
	glPopMatrix();
}

//Draw mammoth
void mammoth()
{
    const dReal* rot;
	const dReal* pos;
	GLfloat matrix[16];
	glPushMatrix();
	//Translate attractor inside of bouncing ball
	rot = dBodyGetRotation(ball_body3);
	pos = dBodyGetPosition(ball_body3);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];//x
	matrix[13] = pos[1];//y
	matrix[14] = (pos[2] - 0.25);//z
	matrix[15] = 1;
	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);

    //Scale the model
    glScaled(0.0005,0.0005,0.0005);
    //  Draw the model
    glCallList(model2);
	glPopMatrix();
}

//Draw dinosaur
void dino()
{
    glPushMatrix();
    const dReal* rot;
	const dReal* pos;
	GLfloat matrix[16];
	glPushMatrix();
	//Translate attractor inside of bouncing ball
	rot = dBodyGetRotation(ball_body2);
	pos = dBodyGetPosition(ball_body2);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];//x
	matrix[13] = pos[1];//y
	matrix[14] = (pos[2]);//z
	matrix[15] = 1;
	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);

    //Scale the model
    glScaled(0.25,0.25,0.25);
    //  Draw the model
    glCallList(model);

	glPopMatrix();
}

//Draw spheres with given radius at origin
void sphere(double r) {
	int th2,ph2;
	int inc = 10;//Ball increment
    const dReal* rot;
    const dReal* pos;
    GLfloat matrix[16];


    glPushMatrix();
    //Disable depth test
    glDisable(GL_DEPTH_TEST);//(disabling allows for transparency)
    //Turn on textures
	glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    //  Enable blending
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//Allow modulation for this texture

	//Lorenz Ball
	//Bind texture at this array position to the object
	glBindTexture(GL_TEXTURE_2D, texture[0]);//Bind glass texture
	// Do this instead of calling glTranslate and glRotate to get proper physics, balls that spin etc.
	rot = dBodyGetRotation(ball_body);
	pos = dBodyGetPosition(ball_body);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
	matrix[15] = 1;

	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);
	//Scale as normal
    glScaled(r, r, r);

    //glTranslated(0.0,5.0,0.0);
	//White ball
	glColor4f(1.0,1.0,1.0,0.1*alpha);

	//  Bands of latitude
	for (ph2 =- 90; ph2 < 90; ph2 += inc)
    {
		glBegin(GL_QUAD_STRIP);
		for (th2 = 0; th2 <= 360; th2 += 2*inc)
		{
			//This function sets the normal for the sphere
			Vertex(th2,ph2);//Call vertex function above
			Vertex(th2, ph2 + inc);
		}
		glEnd();
	}
	glPopMatrix();

	//Ball 2
	glPushMatrix();
	//Bind texture at this array position to the object
	glBindTexture(GL_TEXTURE_2D, texture[1]);//Bind glass texture
	// Do this instead of calling glTranslate and glRotate to get proper physics, balls that spin etc.
	rot = dBodyGetRotation(ball_body2);
	pos = dBodyGetPosition(ball_body2);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
	matrix[15] = 1;

	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);
	//Scale as normal
	glScaled(r, r, r);

	//glTranslated(0.0,5.0,0.0);
	//White ball
	glColor4f(1.0,1.0,1.0,0.1*alpha);

	//  Bands of latitude
	for (ph2 =- 90; ph2 < 90; ph2 += inc)
	{
		glBegin(GL_QUAD_STRIP);
		for (th2 = 0; th2 <= 360; th2 += 2*inc)
		{
			//This function sets the normal for the sphere
			Vertex(th2,ph2);//Call vertex function above
			Vertex(th2, ph2 + inc);
		}
		glEnd();
	}
	glPopMatrix();

	//Ball 3
	glPushMatrix();
	//Bind texture at this array position to the object
	glBindTexture(GL_TEXTURE_2D, texture[2]);//Bind glass texture
	// Do this instead of calling glTranslate and glRotate to get proper physics, balls that spin etc.
	rot = dBodyGetRotation(ball_body3);
	pos = dBodyGetPosition(ball_body3);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
	matrix[15] = 1;

	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);
	//Scale as normal
	glScaled(r, r, r);

	//glTranslated(0.0,5.0,0.0);
	//White ball
	glColor4f(1.0,1.0,1.0,0.1*alpha);

	//  Bands of latitude
	for (ph2 =- 90; ph2 < 90; ph2 += inc)
	{
		glBegin(GL_QUAD_STRIP);
		for (th2 = 0; th2 <= 360; th2 += 2*inc)
		{
			//This function sets the normal for the sphere
			Vertex(th2,ph2);//Call vertex function above
			Vertex(th2, ph2 + inc);
		}
		glEnd();
	}
	glPopMatrix();

	//Turn off textures
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);//reEnable z buffering (disabling allows for transparency)
}


//Lorenz attractor stuff
double * attractorVals()
{
	//Coordinates
	 double x = 1;
	 double y = 1;
	 double z = 1;
	 //  Time step
	 double dt = 0.001;
	 double dx = 0;
	 double dy = 0;
	 double dz = 0;
	 double scale = 0.025;
	 int i = 0;
	 //Must traverse by 3's to store each coordinate of xyz next to one another
	 //i must be divisible by 3 for this to work!
	 for (i=0;i<150000;i+=3)
	 {
		 dx = s*(y-x);
		 dy = x*(r-z)-y;
		 dz = x*y - b*z;
		 x += dt*dx;
		 y += dt*dy;
		 z += dt*dz;

		 values[i] = x * scale;
		 values [i+1] = y * scale;
		 values[i+2] = (z * scale) - 0.75;//Shift back on z plane to fit inside orb
	 }

	return values;
}

void points(int value)
{
	//Reference one points along the attractor
	int i = 0;
	//Step array position with k
	j = k;
	for (i = 0; i<4500; i+=3)
	{
		orbs[i] = reference[j];
		orbs[i+1] = reference[j + 1];
		orbs[i+2] = reference[j+2];

		if((j+99) >= 150000)
		{
			j = 0;
		}

		else if ((j+99) < 150000)
		{
			j += 99;
		}
	}
	//Set a value to step through array
	if ((k + 3) >= 300)
	{
		k = 0;
	}
		else if ((k+3)< 300)
		{
			k += 3;
		}

    glutPostRedisplay();
    //Call this function every 100 milliseconds to reset point values
    glutTimerFunc(30, points, 1);
}

//Draw lorenz
static void drawLorenz()
{
	//Coordinates
	int i;
	double x = 1;
	double y = 1;
	double z = 1;
	const dReal* rot;
	const dReal* pos;
	GLfloat matrix[16];
	glPushMatrix();
	//Translate attractor inside of bouncing ball
	rot = dBodyGetRotation(ball_body);
	pos = dBodyGetPosition(ball_body);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];//x
	matrix[13] = pos[1];//y
	matrix[14] = (pos[2]);//z
	matrix[15] = 1;
	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);
	//A simple test using GL_LINE_STRIP, LINE_STRIP instead of LOOP b/c the starting point never == ending point
	//So using LINE would just add a weird connecting line b/w start and end
	glBegin(GL_LINE_STRIP);//start drawing a line loop
	//i must be divisible by 3 for this to work!
	for (i=0;i<150000;i+=3)
	{
		 x = *(values1 + i);
		 y = *(values1 + (i+1));
		 z = *(values1 + (i+2));

		 //Draw x, y, and z coordinates from each set of three in attractor values
		 glVertex3f(x,y,z);
	 }
	 glEnd();//Attractor drawing loop
	 glPopMatrix();
}

//Draw points
static void drawOrbs()
{
	int q;
	const dReal* rot;
	const dReal* pos;
	GLfloat matrix[16];
	glPushMatrix();
	//Translate attractor inside of bouncing ball
	rot = dBodyGetRotation(ball_body);
	pos = dBodyGetPosition(ball_body);
	matrix[0] = rot[0];
	matrix[1] = rot[4];
	matrix[2] = rot[8];
	matrix[3] = 0;
	matrix[4] = rot[1];
	matrix[5] = rot[5];
	matrix[6] = rot[9];
	matrix[7] = 0;
	matrix[8] = rot[2];
	matrix[9] = rot[6];
	matrix[10] = rot[10];
	matrix[11] = 0;
	matrix[12] = pos[0];//x
	matrix[13] = pos[1];//y
	matrix[14] = (pos[2]);//z
	matrix[15] = 1;
	//Multiply against the current matrix to get proper location of ball
	glMultMatrixf(matrix);
	glEnable(GL_POINT_SMOOTH);//Create circles instead of squares
	//Must scale the point size along with zooming feature
	glPointSize(2.5);
	glColor3f(1.0,1.0,1.0);//Allow these to just be white
	//Try drawing some animated dots traversing along the lines
	glBegin(GL_POINTS);
	for (q = 0; q < 4500; q+=3)
	{
	    glVertex3f(*(orbs + q),*(orbs + (1 + q)),*(orbs +(2 + q)));
	}
	glEnd();//Attractor drawing loop
	glPopMatrix();
}

//Create a window
void display(void){
   //clear the screen, we MUST clear the DEPTH_BUFFER_BIT by enabling GL_DEPTH_TEST to get correct depth of the drawing!
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);//Enable z buffering
   glEnable(GL_POLYGON_SMOOTH);
   // enable blending
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   //Load identity matrix
   glLoadIdentity();

   //If we are in perspective view set eye position
   if (!view)
   {
       double Ex = -2*dim*Sin(th)*Cos(ph);
	   double Ey = +2*dim        *Sin(ph);
	   double Ez = +2*dim*Cos(th)*Cos(ph);
	   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
	 //Orthogonal - set world orientation
	 else
	 {
	    glRotatef(ph,1,0,0);
	    glRotatef(th,0,1,0);
	 }

   //Draw 3 spheres with radius 1
   sphere(1);//Draw a sphere

   //Use shader to draw objects inside of transparent orbs
   glUseProgram(shader[1]);

   dino();//Draw dinosaur
   drawLorenz();//Draw lorenz attractor
   mammoth();//Draw mammoth

   //Draw a small floor grid (15 smooshed cubes)
   Cube(-4,0,-4);
   Cube(-2,0,-4);
   Cube(0,0,-4);
   Cube(2,0,-4);
   Cube(4,0,-4);
   Cube(-4,0,0);
   Cube(-2,0,0);
   Cube(0,0,0);
   Cube(2,0,0);
   Cube(4,0,0);
   Cube(-4,0,4);
   Cube(-2,0,4);
   Cube(0,0,4);
   Cube(2,0,4);
   Cube(4,0,4);

   if (skull == 1)
   {
	   skullDrop();
   }
   //Turn of shader
   glUseProgram(0);

   //No shader after this point
   drawOrbs();//Lorenz attractor white orbs

   //Display parameters
   glColor3f(1,0,0);
   glWindowPos2i(5,5);
   Print("Projection= %s", view?"Orthogonal":"Perspective;   Press 'Space' if you hate these objects!");
   //Step the physics simulation
   simLoop();
   //Force an update to the screen
   glFlush();
   //Double buffer magic trick :)
   glutSwapBuffers();
}

void special(int key,int x,int y)
{
   //Right arrow - increase rotation by 5 degree
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //Left arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //up arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //down arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //Enable zooming in!
   else if (key == GLUT_KEY_F1){
   	  dim += 0.25;
   }
   //Enable zooming out!
   else if ((key == GLUT_KEY_F2) && (zoomFactor > 0)){
	  dim -= 0.25;
   }
   //Re-project to get orthogonal view to work
   Project(view,ar,dim);
   //Request display update
   glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y) {
    switch(key){
		case 'm': {
			//Toggle between orthogonal and projection views
			view = 1 - view;
			break;
		}
		case 'M': {
			//Toggle between orthogonal and projection views
			view = 1 - view;
			break;
		}
		//Space to drop skull
		case 32:{
			skull = 1;
			dBodyEnable(ball_body4);//Enable this body so gravity affects it
			break;
		}
		//Esc to close program
		case 27:{
			//Return ODE physics bodies to memory, requires dInitODE() in main() to not crash
			dWorldDestroy(world);
			dCloseODE();
			//Exit the program
			exit(0);
		}
    }
    //Re-project to get orthogonal view to work
    Project(view,ar,dim);
   //Request display update
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width,int height)
{
    //  Calculate width to height ratio
   ar = (height>0) ? (double)width/height : 1;
   //Set viewport as entire window
   glViewport(0,0, width,height);
   //call glOrtho
   Project(view, ar, dim);
}

int main(int argc, char *argv[])
{
   //Initialize GLUT
   glutInit(&argc,argv);
   //Request double buffered true color window with Z-buffer
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   //create a window of a particular size in pixels:
   glutInitWindowSize(1000,1000);
   //Actually make a window called A Lorenz Attractor, W/O this there is no window
   glutCreateWindow("NDC to RGB by Russ Mehring");
   #ifdef USEGLEW
      glewExperimental=TRUE;//May not be needed, but saw in blog as recommended to include
	  //  Initialize GLEW
	  if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
	  if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
   #endif
   //Register the screen, should call our function that actually draws to the screen
   //Contains a pointer that points to the display()function below
   glutDisplayFunc(display);
   glutIdleFunc (display);//when idle, redisplay
   glutReshapeFunc(reshape);

   //Pass control to GLUT for events, i.e. let us control the pixel at a location so that we can set it to 1
   //when we clear the buffer! I.e. enables correct depth perception by setting that pixel to it's value in
   //terms of the camera position (Note: 0 is as close as you can get to the viewer)
   //Enable keyboard interaction with glut
   glutKeyboardFunc(keyPressed);
   glutSpecialFunc(special);
   glutTimerFunc(30, points, 1);//Start points function
   //Calculate lorenz attractor values
   values1 = attractorVals();
   reference = values1;
   //  Create Shader Programs
   shader[1] = CreateShaderProg("shader.vert","shader.frag");
   //Load textures
   texture[0] = LoadTexBMP("glass7.bmp");
   texture[1] = LoadTexBMP("glass6.bmp");
   texture[2] = LoadTexBMP("glass5.bmp");
   //  Load object
   model = LoadOBJ("tyra.obj");
   model2 = LoadOBJ("Mammoth.obj");
   model3 = LoadOBJ("skull.obj");
   //Return to OS i.e. actually draw
   ErrCheck("init");
   // dWorldID world = dWorldCreate();
   dInitODE();//Initialize ode with itself
   //Initialize ode values that we need with our own function (otherwise main() would be enormous)
   initODE();
   //Go to mainloop
   glutMainLoop();
   return 0;
}
