/*********
   CTIS164 - Homework 3
----------
STUDENT : Bedirhan Kara
SECTION : 3
HOMEWORK: HW3 Rotating Target Shoting Game
----------
PROBLEMS: No problem.
----------
ADDITIONAL FEATURES: Bullet is a complex shape which is a star, I have 4 orbits and 4 target, you can see the angle of shape by rotating mouse, targets are not simple circles,
					 you can see your total scor on the right upper corner, the points on the targets is the value of target,also targets moving with different speeds and directions,
					 also targets are not simple circles.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

double posOfX, posOfY;
double angleOfMouse, angleOfBullet, time;

typedef struct {
	double x, y;
}pos_t;

typedef struct {
	int speed; // by using speed parameter we can change the speed of bullet more clear
	pos_t pos;
}bullet_t;

typedef struct {
	pos_t pos;
	float r, g, b;
	bool active = false;
	double radious;
	double orbit, açý;
	int scr;
}targets_t;

targets_t targ[4];

pos_t gun = {0,0};

float a = 1, b = 1;

bullet_t bom = { 1,{a * sin(angleOfBullet),b * cos(angleOfBullet)} };

bool fire = false, // to check fired or not
start = false, // to check started or not and all shapes hitted
dewamke = false; // to understand program was started

int turn = 70, // to put targets correct wires to turn
score = 0, // to stop program when all targets gone
realScore = 0; // to count how many target we shot

void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}
																								 
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}													
																	
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}												
																														
void XandY()																												
{																														
	glColor3f(0.050, 0.870, 0.807);
	glBegin(GL_LINES);																									
	glVertex2f(400, 0);																									
	glVertex2f(-400, 0);
	glVertex2f(0, -400);
	glVertex2f(0, 400);
	glEnd();
}																										

void vv(pos_t P, pos_t Tr, double angle) {
	double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
	double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
	glVertex2d(xp, yp);
}

void cc(pos_t P, pos_t Tr, double angle, double radious)
{
	double vertexX = (P.x * cos(angle) - P.y * sin(angle));
	double vertexY = (P.x * sin(angle) + P.y * cos(angle));
	circle(vertexX, vertexY, radious);
}

void drawSmallGun(pos_t p)
{
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	vv({ 0, 30 }, p, angleOfMouse);
	vv({ 0, -30 },p, angleOfMouse);
	vv({ 30, 0 }, p, angleOfMouse);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	vv({ -25, 10 }, p, angleOfMouse);
	vv({ 0  , 10 }, p, angleOfMouse);
	vv({ 0  , -10 }, p, angleOfMouse);
	vv({ -25, -10 }, p, angleOfMouse);
	glEnd();

	glColor4f(1, 0, 0, 1);
	cc({ -38.5,0 }, p, angleOfMouse, 5);

	glLineWidth(2);
	glBegin(GL_QUADS);
	vv({ -30, 24 }, p, angleOfMouse);
	vv({ -25, 10 }, p, angleOfMouse);
	vv({ -36, 10 }, p, angleOfMouse);
	vv({ -36, 25 }, p, angleOfMouse);
	glEnd();
	glColor4f(0, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	vv({ -30, 24 }, p, angleOfMouse);
	vv({ -25, 10 }, p, angleOfMouse);
	vv({ -36, 10 }, p, angleOfMouse);
	vv({ -36, 25 }, p, angleOfMouse);
	glEnd();

	glColor4f(1, 0, 0, 1);
	glBegin(GL_QUADS);
	vv({ -30, -24 }, p, angleOfMouse);
	vv({ -25, -10 }, p, angleOfMouse);
	vv({ -36, -10 }, p, angleOfMouse);
	vv({ -36, -25 }, p, angleOfMouse);
	glEnd();
	glColor4f(0, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	vv({ -30, -24 }, p, angleOfMouse);
	vv({ -25, -10 }, p, angleOfMouse);
	vv({ -36, -10 }, p, angleOfMouse);
	vv({ -36, -25 }, p, angleOfMouse);
	glEnd();

	glColor4f(1, 0, 0, 1);
	glBegin(GL_QUADS);
	vv({ -38, 5 }, p, angleOfMouse);
	vv({ -15, 5 }, p, angleOfMouse);
	vv({ -15,-5 }, p, angleOfMouse);
	vv({ -38,-5 }, p, angleOfMouse);
	glEnd();

	glColor4f(0, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	vv({ -38, 5 }, p, angleOfMouse);
	vv({ -15, 5 }, p, angleOfMouse);
	vv({ -15,-5 }, p, angleOfMouse);
	vv({ -38,-5 }, p, angleOfMouse);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	vv({ -15, 5 },  p, angleOfMouse);
	vv({ -30, 24 }, p, angleOfMouse);
	vv({ -25, 10 }, p, angleOfMouse);

	vv({ -15, -5 },  p, angleOfMouse);
	vv({ -30, -24 }, p, angleOfMouse);
	vv({ -25, -10 }, p, angleOfMouse);
	glEnd();

	glColor4f(0.4, 0.4, 0.4, 1);
	glBegin(GL_QUADS);
	vv({ -25, 10}, p, angleOfMouse);
	vv({ -35, 10}, p, angleOfMouse);
	vv({ -35, 9 }, p, angleOfMouse);
	vv({ -25, 9 }, p, angleOfMouse);

	vv({ -25, -10 }, p, angleOfMouse);
	vv({ -35, -10 }, p, angleOfMouse);
	vv({ -35, -9 }, p, angleOfMouse);
	vv({ -25, -9 }, p, angleOfMouse);

	glEnd();

	glColor4f(1,0,0, 1);
	glBegin(GL_QUADS);
	vv({15,15 },  p, angleOfMouse);
	vv({15,-15 }, p, angleOfMouse);
	vv({40,-10 }, p, angleOfMouse);
	vv({ 40,10 }, p, angleOfMouse);
	
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	vv({ 40,-10 }, p, angleOfMouse);
	vv({ 40,10 }, p, angleOfMouse);
	vv({ 60, 0 }, p, angleOfMouse);
	glEnd();

	glColor4f(0, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	vv({ 40,-10 }, p, angleOfMouse);
	vv({ 40,10 }, p, angleOfMouse);
	vv({ 60, 0 }, p, angleOfMouse);
	glEnd();

	// flag
	glColor3f(1, 1, 1);
	cc({ 0,0 }, p, angleOfMouse, 20);

	glColor3f(1, 0, 0);
	cc({ 6,0 }, p, angleOfMouse, 15);

	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	vv({ -10, 20 }, p, angleOfMouse);
	vv({ 0  , 20 }, p, angleOfMouse);
	vv({ 0  , 10 }, p, angleOfMouse);
	vv({ -10, 10 }, p, angleOfMouse);

	vv({ -10, -10 }, p, angleOfMouse);
	vv({ 0  , -10 }, p, angleOfMouse);
	vv({ 0  , -20 }, p, angleOfMouse);
	vv({ -10, -20 }, p, angleOfMouse);

	glEnd();

	//side gun parts
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	vv({ 20, 28 },  p, angleOfMouse);
	vv({ 0  , 28 }, p, angleOfMouse);
	vv({ 0  , 32 }, p, angleOfMouse);
	vv({ 20, 32 },  p, angleOfMouse);

	vv({ 20, -28 },  p, angleOfMouse);
	vv({ 0  , -28 }, p, angleOfMouse);
	vv({ 0  , -32 }, p, angleOfMouse);
	vv({ 20, -32 },  p, angleOfMouse);

	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	vv({ 20, 28 }, p, angleOfMouse);
	vv({ 20, 32 }, p, angleOfMouse);
	vv({ 23, 30 }, p, angleOfMouse);

	vv({ 20, -28 }, p, angleOfMouse);
	vv({ 20, -32 }, p, angleOfMouse);
	vv({ 23, -30 }, p, angleOfMouse);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_TRIANGLES);
	vv({ 15, 0 }, p, angleOfMouse);
	vv({ 25, 0 }, p, angleOfMouse);
	vv({ 10, 5 }, p, angleOfMouse);
				  		
	vv({ 15, 0 }, p, angleOfMouse);
	vv({ 25, 0 }, p, angleOfMouse);
	vv({ 10, -5}, p, angleOfMouse);
				  		
	vv({ 15, 0 }, p, angleOfMouse);
	vv({ 20, 7 }, p, angleOfMouse);
	vv({ 20, -7}, p, angleOfMouse);
	glEnd();
}

void drawStar(bullet_t fires)
{
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	vv({ 15, 0 }, fires.pos, angleOfBullet);
	vv({ 25, 0 }, fires.pos, angleOfBullet);
	vv({ 10, 5 }, fires.pos, angleOfBullet);
								 
	vv({ 15, 0 }, fires.pos, angleOfBullet);
	vv({ 25, 0 }, fires.pos, angleOfBullet);
	vv({ 10, -5 },fires.pos, angleOfBullet);
							 
	vv({ 15, 0 }, fires.pos, angleOfBullet);
	vv({ 20, 7 }, fires.pos, angleOfBullet);
	vv({ 20, -7 },fires.pos, angleOfBullet);
	glEnd();
}

void cursorFollower()
{
	glBegin(GL_LINES);
	glColor4f(1, 0.5, 0.5,0.3);
	glVertex2f(0, 0);
	glVertex2f(posOfX, posOfY);
	glEnd();
}

void wires()
{
	glColor3f(0, 0, 1);
	circle_wire(0, 0, 140);
	glColor3f(0, 1, 1);
	circle_wire(0, 0, 210);
	glColor3f(1, 0, 1);
	circle_wire(0, 0, 280);
	glColor3f(1, 1, 0);
	circle_wire(0, 0, 350);
}

void initializeTargets(int i) // for initialize tagget shapes
{
	targ[i].active = true;
	targ[i].açý = rand() % 360;
	targ[i].pos.x = turn * sin((targ[i].açý)*D2R); // to move harmonic
	targ[i].pos.y = turn * cos((targ[i].açý)*D2R); // to move harmonic
	targ[i].radious = rand() % ((24 + 1) - 15) + 15;  // random radious
	targ[i].r = rand() % 255;  	//for random colors
	targ[i].g = rand() % 255;	//for random colors
	targ[i].b = rand() % 255;	//for random colors
	targ[i].scr = rand() % 3+1;
}

bool doesHit(int cx, int cy, int px, int py, int radius) {
	float distance = sqrt((px - cx)*(px - cx) + (py - cy) * (py - cy)); // formul of distance between 2 point

	if (distance < 8+radius) // if distance between bullet and center of the target is less than radius that means bullet hit
		return true;
	else
		return false;
}
//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0,0,0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	XandY();  // axes
	cursorFollower(); // to see where to fire
	drawSmallGun(gun); // display gun 
	wires(); // orbits
	drawStar(bom); // bullet and the share of gun
	vprint(-390,385, GLUT_BITMAP_9_BY_15, "Homework #3");
	vprint(-390, 370, GLUT_BITMAP_8_BY_13, "By Bedirhan Kara");

	if (start && score!=4)
	{
		vprint(300, 370, GLUT_BITMAP_TIMES_ROMAN_24, "Score: %d ", realScore);
		vprint(380 * cos(angleOfMouse), 380 * sin(angleOfMouse), GLUT_BITMAP_HELVETICA_10, "%.0f ", angleOfMouse / D2R);

		for (int k = 1; k < 5; k++)
		{
			if (targ[k].active)
			{
				glColor3ub(targ[k].b, targ[k].g, targ[k].r);
				circle_wire(targ[k].pos.x, targ[k].pos.y, targ[k].radious);

				glColor3ub(targ[k].r, targ[k].g, targ[k].r);
				circle_wire(targ[k].pos.x, targ[k].pos.y, targ[k].radious-5);

				glColor3ub(targ[k].g, targ[k].r, targ[k].b);
				circle_wire(targ[k].pos.x, targ[k].pos.y, targ[k].radious-10);

				glColor3ub(targ[k].r, targ[k].g, targ[k].b);
				circle_wire(targ[k].pos.x, targ[k].pos.y, targ[k].radious-15);

				glColor3ub(0,0,0);
				circle(targ[k].pos.x, targ[k].pos.y, targ[k].radious-16);

				glColor3f(1,1,1);
				vprint(targ[k].pos.x-6,targ[k].pos.y-7, GLUT_BITMAP_TIMES_ROMAN_24, "%d ", targ[k].scr);
			}
		}
	}
	else
		vprint(-100, -250, GLUT_BITMAP_TIMES_ROMAN_24, "Click left button to start");
	
		
	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.

	if (button == GLUT_LEFT_BUTTON)
	{
		fire = true; // for fire
		start = true; // for start the game
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

void onMove(int x, int y) {
	// Write your codes here.
	posOfX = x - winWidth / 2;
	posOfY = winHeight / 2 - y;

	if (winHeight / 2 - y > 0)
		angleOfMouse = atan2(winHeight / 2 - y, x - winWidth / 2); // atan2 will give us the correct angle of mouse and by changing the position of mouse angle we can make shape follow mouse 
	else
		angleOfMouse = 360 * D2R + atan2(winHeight / 2 - y, x - winWidth / 2); // adding 360 because when slope is negative it is not correct 

	if (!fire) // using not fire because if dont use when mouse position change, moving bullets angle will be change
	{
		if (winHeight / 2 - y > 0)
			angleOfBullet = atan2(winHeight / 2 - y, x - winWidth / 2);
		else
			angleOfBullet = 360 * D2R + atan2(winHeight / 2 - y, x - winWidth / 2);
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (start)
		time += 0.1; // using time for not to fire on first click

	if (fire&&bom.pos.x<400&&bom.pos.x>-400&&bom.pos.y<400&&bom.pos.y>-400&&time>1) // using screen boards to rerotate and fire the bullet
	{
		bom.pos.x += cos(angleOfBullet) * 10;
		bom.pos.y += sin(angleOfBullet) * 10;
	}
	else
	{
		angleOfBullet = angleOfMouse; //to correct position of bullet proper way 
		fire = false; // to be able to fire again when bullet beyond the screen
		bom.pos.x = 1;
		bom.pos.y = 1;
	}

	for (int k = 1; k < 5; k++)
	{
		if (targ[k].active) // movement mechanism
		{
			targ[k].pos.x = (k + 1) * turn * sin((targ[k].açý)*D2R);
			targ[k].pos.y = (k + 1) * turn * cos((targ[k].açý)*D2R);

			if (doesHit(targ[k].pos.x, targ[k].pos.y, bom.pos.x+5, bom.pos.y+5, targ[k].radious)) // checking if hit or not in the target active check because if we seperate these two
			{																					 // we will gain point every pass from the last position of hitten target or gain 3 point 
				score++;																    	// for each so we write hit check in active check.
				realScore+=targ[k].scr;
				targ[k].active = false;
			}
		}
	}					

	targ[1].açý -= 2.22;    // move targets with diffirent speeds and directions
	targ[2].açý += 1.33;	// move targets with diffirent speeds and directions
	targ[3].açý -= 0.96;	// move targets with diffirent speeds and directions
	targ[4].açý += 0.58;	// move targets with diffirent speeds and directions 

	

	if (score == 4) // to understand all targets are gone
	{
		time=0; // using it for not to fire first click and restart clicks
		start = false;
		score = 0;
		for(int k=1;k<5;k++)
			initializeTargets(k);  // to change properties of targets again
	}
	
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 1; i < 5; i++) // to initialize all targets and provide not to change colors constantly
		initializeTargets(i);
}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW3: Rotating Targets By Bedirhan Kara");
	
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}