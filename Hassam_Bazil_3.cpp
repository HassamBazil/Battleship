/*********
CTIS164 - Template Source Program
----------
STUDENT : Hassam Bazil
SECTION : 04
HOMEWORK: 3rd Homework
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  35// Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool timer = 1, shoot=0;
typedef struct {
	int x, y,j,k,l, rad, dir, s;
	
	float angle, dis;
	bool collision = 0;


}circles_t;

circles_t circles[4];

typedef struct {
	float sx, sy;
	float angle;

}spaceship_t;

spaceship_t ship{ 0,0,0 };

typedef struct {
	float bx, by;
	float angle;

}bullet_t;

bullet_t bullet{ 0,0,ship.angle };

typedef struct { float x, y;  }pos;

void v(pos P, pos Tr, double angle) {
	double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
	double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
	glVertex2d(xp, yp);
}
bool testCollision(bullet_t br, circles_t t) {
	float dx = t.x - br.bx;
	float dy = t.y - br.by;
	float d = sqrt(dx*dx + dy*dy);
	return d <= t.rad;
}


						  //
						  // to draw circle, center at (x,y)
						  // radius r
						  //

void startingvalues() {
	int i = 0;
	while(i<4)
	{
		circles[i].j = rand() % 256;
		circles[i].k = rand() % 256;
		circles[i].l = rand() % 256;


		

		
		
		circles[i].dir = rand() % 5 + 1;


		circles[i].collision = 0;
		circles[i].angle = rand() % 361;
		circles[i].s = rand() % 2 + 1;
		circles[i].rad = rand() % 25 + 15;

		i++;
	}
	int value = 150;
	for (int k = 0; k < 4; k++)
	{
		circles[k].dis = value;
		value += 50;
	}
}
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
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
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
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
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void spaceship(spaceship_t ship) {

	float sangle = ship.angle * D2R;
	glColor3ub(0, 71, 62);
	glBegin(GL_QUADS);
	v({ 5, -10 }, { 0, 0 }, sangle);
	v({ 5, 10 }, { 0, 0 }, sangle);
	v({ -9,  10}, { 0, 0 }, sangle);
	v({ -9, -10 }, { 0, 0 }, sangle);
	glEnd();
	glBegin(GL_QUADS);
	v({ 5, -3 }, { 0, 0 }, sangle);
	v({ 5, 3 }, { 0, 0 }, sangle);
	v({ 15,  3 }, { 0, 0 }, sangle);
	v({ 15, -3 }, { 0, 0 }, sangle);
	glEnd();

	glBegin(GL_POLYGON);
	v({ 3, 10 }, { 0, 0 }, sangle);
	v({ -5, 10 }, { 0, 0 }, sangle);
	v({ -10, 20 }, { 0, 0 }, sangle);
	v({ -7, 20 }, { 0, 0 }, sangle);
	v({ 3, 10 }, { 0, 0 }, sangle);
	glEnd();

	glBegin(GL_POLYGON);
	v({ 3, -10 }, { 0, 0 }, sangle);
	v({ -5, -10 }, { 0, 0 }, sangle);
	v({ -10, -20 }, { 0, 0 }, sangle);
	v({ -7, -20 }, { 0, 0 }, sangle);
	v({ 3, -10 }, { 0, 0 }, sangle);
	glEnd();

	
	glLineWidth(2);
	 
	glBegin(GL_TRIANGLES);
	v({ 5, 10 }, { 0, 0 }, sangle);
	v({ 11, 0 }, { 0, 0 }, sangle);
	v({ 5, -10 }, { 0, 0 }, sangle);
	glEnd();

	glBegin(GL_POLYGON);
	v({ -9, 10 }, { 0, 0 }, sangle);
	v({ -14, 10 }, { 0, 0 }, sangle);
	v({ -18, 5 }, { 0, 0 }, sangle);
	v({ -9, 5 }, { 0, 0 }, sangle);
	v({ -9, 10 }, { 0, 0 }, sangle);
	glEnd();
	
	
	glBegin(GL_POLYGON);
	v({ -9, -10 }, { 0, 0 }, sangle);
	v({ -14, -10 }, { 0, 0 }, sangle);
	v({ -18, -5 }, { 0, 0 }, sangle);
	v({ -9, -5 }, { 0, 0 }, sangle);
	v({ -9, -10 }, { 0, 0 }, sangle);
	glEnd();

	
	
	glColor3ub(158, 183, 20);
	glBegin(GL_POLYGON);
	v({ -5, -3 }, { 0, 0 }, sangle);
	v({ -5, 3 }, { 0, 0 }, sangle);
	v({ -1, 3 }, { 0, 0 }, sangle);
	v({ 3, 0 }, { 0, 0 }, sangle);
	v({ -1, -3 }, { 0, 0 }, sangle);
	glEnd();

	glColor3ub(107, 34, 17);
	glBegin(GL_LINES);
	v({ -7, 19 }, { 0, 0 }, sangle);
	v({ 2, 19 }, { 0, 0 }, sangle);
	glEnd();
	glBegin(GL_LINES);
	v({ -7, -19 }, { 0, 0 }, sangle);
	v({ 2, -19 }, { 0, 0 }, sangle);
	glEnd();
	glBegin(GL_LINES);
	v({ -9, 0 }, { 0, 0 }, sangle);
	v({ -13, 0 }, { 0, 0 }, sangle);
	glEnd();
	glBegin(GL_LINES);
	v({ 5, 9 }, { 0, 0 }, sangle);
	v({ 12, 9 }, { 0, 0 }, sangle);
	glEnd();
	glBegin(GL_LINES);
	v({ 5, -9 }, { 0, 0 }, sangle);
	v({ 12, -9 }, { 0, 0 }, sangle);
	glEnd();
	glBegin(GL_LINES);
	v({ 15, 0 }, { 0, 0 }, sangle);
	v({22, 0 }, { 0, 0 }, sangle);
	glEnd();
	

	

}

void background(){
	glColor3b(76, 44, 22);
	glBegin(GL_QUADS);
	glVertex2f(-450, 350);
	glVertex2f(450, 350);
	glVertex2f(450, -350);
	glVertex2f(-450, -350);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(WINDOW_WIDTH / 2, 0);
	glVertex2f(-WINDOW_WIDTH / 2, 0);
	glVertex2f(0, WINDOW_HEIGHT / 2);
	glVertex2f(0, -WINDOW_HEIGHT / 2);
	glEnd();
	circle_wire(0, 0, 150);
	
	circle_wire(0, 0, 200);
	circle_wire(0, 0, 250);
	circle_wire(0, 0, 300);
	


}

void drawcircles(circles_t cir) {

	glColor3ub(cir.j, cir.k, cir.l);

	circle(cir.x, cir.y, cir.rad);
	glColor3ub(0, 0, 0);



	vprint(cir.x - 12, cir.y - 3, GLUT_BITMAP_HELVETICA_12, "%0.0f", cir.angle);


}

void b(bullet_t bul) {
	glColor3f(0, 0, 2);
	circle(bullet.bx, bullet.by, 3);

}
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	background();
	b(bullet);
	spaceship(ship);
	int i = 0;

	while (i < 4)
	{
		if (circles[i].collision == 0)
		{
			circles[i].x = circles[i].dis * cos(circles[i].angle * D2R); 
			circles[i].y = circles[i].dis * sin(circles[i].angle * D2R); 
			drawcircles(circles[i]);

		}
		i++;

	}

	
	

	vprint(340 * cos(ship.angle * D2R), 340 * sin(ship.angle * D2R), GLUT_BITMAP_HELVETICA_12, "%0.0f", ship.angle);
	vprint(-440, 333, GLUT_BITMAP_HELVETICA_12, "Homework #3");
	vprint(-440, 320, GLUT_BITMAP_HELVETICA_12, "by Hassam Bazil");

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

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
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

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
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

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) // moving the bullet 
	{
		if (shoot == 0)
			bullet.angle = ship.angle;
		shoot = 1;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.
	int xx = x - winWidth / 2;
	int yy = winHeight / 2 - y;
	float angle = (atan2f(yy, xx))*(180 / PI);
	if (angle < 0)
		angle += 360;
	ship.angle = angle;



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	int cr=0;
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (timer == 1)
	{
		while (cr < 4)
		{
			if (circles[cr].dir == 1)
			{
				circles[cr].angle += circles[cr].s;
			}

			else
			{
				circles[cr].angle -= circles[cr].s;
			}
			if (circles[cr].angle > 360)
			{
				circles[cr].angle -= 360;
			}
			else if (circles[cr].angle < 0)
			{
				circles[cr].angle += 360;
			}
			if (testCollision(bullet, circles[cr]) == true)
			{
				circles[cr].collision = 1;
			}
			cr++;
		}


		if (shoot == 1)			
		{	
			bullet.by += 15 * sin(bullet.angle * D2R);

			bullet.bx += 15 * cos(bullet.angle * D2R);
			
			if (((bullet.by < -350 || bullet.by > 350 || bullet.bx > 450 || bullet.bx < -450) ) )	{
				shoot = 0;
				bullet.bx = 0;
				bullet.by = 0;
			}
		}
	}

	if (circles[0].collision == 1 && circles[1].collision == 1 && circles[2].collision == 1 && circles[3].collision == 1 )
		startingvalues();

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes

	glEnable(GL_BLEND);
	startingvalues();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Shooter");
	srand(time(NULL));
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