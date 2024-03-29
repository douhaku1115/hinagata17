#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include "font.h"
#include "Rect.h"
#include "Ball.h"
#include <time.h>
#include "VerticalPaddle.h"
#include "audio.h"
#include "Rect.h"
#include "tex.h"
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256

using namespace glm;
#define BALL_MAX 2

bool keys[256];
VerticalPaddle paddle;
Ball balls[BALL_MAX];
Rect rect1 = Rect(vec2(100, 100), vec2(100, 200));

void display(void) {
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);//(GLenum mode);
	glLoadIdentity();
	gluOrtho2D(
		0,//GLdouble left,
		SCREEN_WIDTH,//GLdouble right,
		SCREEN_HEIGHT,	//GLdouble bottom,
		0);	//GLdouble top);
	glMatrixMode(GL_MODELVIEW);//GLenum mode
	glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);  //GLenum cap);
	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,//(GLenum sfactor,
		GL_ONE_MINUS_DST_ALPHA);//GLenum dfactor);
	
	Rect(vec2(128, 128)).draw();

	fontBegin();
	fontHeight(FONT_DEFAULT_HEIGHT);
	fontWeight(fontGetWeightMax());
	fontFont(FONT_FONT_ROMAN);
	fontPosition(0,0);
	glColor3ub(0xff, 0xff, 0xff);
	//fontDraw("012345\n");
	//fontDraw("ABCDEF\n");
	fontEnd();

	glutSwapBuffers();
};

void idle(void){
	audioUpdate();
	/*for (int i = 0; i < BALL_MAX; i++){
		balls[i].update();

		if (paddle.intersectBall(balls[i])) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x *= -1;
		}

		if (balls[i].m_position.y < 0) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.y = fabs(balls[i].m_speed.y);
		}
		if (balls[i].m_position.y >= windowSize.y) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.y = -fabs(balls[i].m_speed.y);
		}
		if (balls[i].m_position.x >= windowSize.x) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x = -fabs(balls[i].m_speed.x);
		}
		if (balls[i].m_position.x < 0) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x = fabs(balls[i].m_speed.x);
		}
	}*/
	float f = 2;
	if (keys['w']) rect1.m_position.y -= f;
	if (keys['s']) rect1.m_position.y += f;
	if (keys['a']) rect1.m_position.x -= f;
	if (keys['d']) rect1.m_position.x += f;
	
	glutPostRedisplay();
}
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}
void reshape(int width, int height) {
	printf("rehape:width:%d height:%d\n",width,height);
	glViewport(0,0,width,height);
	//GLint x, GLint y, GLsizei width, GLsizei height);
	}
void keyboard(unsigned char key, int x, int y) {
	if (key == 0x1b)
		exit(0);
	printf("keyboard:\'%c\'(%#x)\n", key, key);
	keys[key] = true;
	
}
void keyboardUp(unsigned char key, int x, int y) {
	printf("keyboardUp:\'%c\'(%#x)\n", key, key);
	keys[key] = false;
}
void passiveMotion(int x, int y) {
	paddle.m_position = vec2(x, y);
	printf("passoveMotion::x:%d y:%d\n",x,y);
}
int main(int argc, char* argv[]) {
	audioInit();
	srand(time(NULL));

	

	glutInit(&argc, argv);

	glutInitDisplayMode(GL_DOUBLE);
	glutInitWindowPosition(640,0);
	{
		int height = 720;
		int width = 720*4/3;
		glutInitWindowSize(width, height);
	}
	glutCreateWindow("a");
	//texFromBPM("test1.bmp");
	glutDisplayFunc(display);
	//glutTimerFunc(0, timer, 0);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);//void (GLUTCALLBACK *func)(int width, int height));
	glutKeyboardFunc(keyboard);//GLUTCALLBACK *func)(unsigned char key, int x, int y));
	glutKeyboardUpFunc(keyboardUp);//void (GLUTCALLBACK *func)(unsigned char key, int x, int y));
	//glutPassiveMotionFunc(passiveMotion); //void (GLUTCALLBACK * func)(int x, int y));
    //glutMotionFunc(motion); void (GLUTCALLBACK * func)(int x, int y));
	glutIgnoreKeyRepeat(GL_TRUE);//int ignore
	glutPassiveMotionFunc(passiveMotion);//void (GLUTCALLBACK *func)(int x, int y));
	glutMainLoop();
}