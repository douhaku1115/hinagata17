
#include <stdio.h>
#include <stdarg.h>
#include "font.h"

static void* font = GLUT_STROKE_ROMAN;
using namespace glm;
static vec2 screenSize = { 256,256 };
static float weight=1;
static vec2 position;
static vec2 origin;
static float height = FONT_DEFAULT_HEIGHT;
static unsigned char color[3];

void fontBegin() {
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);//GLbitfield mask

	glMatrixMode(GL_PROJECTION);//(GLenum mode);
	glLoadIdentity();

	
	gluOrtho2D(
		0,//GLdouble left,
		screenSize.x,//GLdouble right,
		screenSize.y,	//GLdouble bottom,
		0);	//GLdouble top);
	glMatrixMode(GL_MODELVIEW);//GLenum mode
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}
void fontEnd() {
	glPopMatrix();
	glPopAttrib();
}
void fontFont(int _font) {
	switch (_font) {
	case FONT_FONT_ROMAN: font = GLUT_STROKE_ROMAN; break;
	case FONT_FONT_MONO_ROMAN: font = GLUT_STROKE_MONO_ROMAN; break;


	}
}
void fontScreenSize(float _width, float _height) {
	screenSize = vec2(_width, _height);
}
void fontPosition(float _x, float _y) {
	origin=position = vec2(_x, _y);
}
void fontHeight(float _height) {
	height = _height;
}
float fontGetHeight() {
	return height;
}
float fontGetLineHeight() {
	return height +FONT_DEFAULT_LINE_SPACE* 
		height/ FONT_DEFAULT_HEIGHT;
}
float fontGetWidth(int _character) {
	return  glutStrokeWidth(
		font,	        //void *font,
		_character	)    //int character);
		* height / FONT_DEFAULT_HEIGHT;
}
float fontGetLenght(const unsigned char* _string) {
	return glutStrokeLength(
		font,//void* font,
		_string//const unsigned char* string);
	) * height / FONT_DEFAULT_HEIGHT;
}

float fontGetWeightMin() {
	GLfloat weight[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE,weight);//GLenum pname, GLfloat *params)
	return weight[0];
}
float fontGetWeightMax() {
	GLfloat weight[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, weight);//GLenum pname, GLfloat *params)
	return weight[1];
}

void fontWeight(float _weight) {
	weight = _weight;

}
float fontGetWeight() {
	return weight;
}
/*void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue) {
	color[0] = _red;
	color[1] = _green;
	color[2] = _blue;
}*/

void fontDraw(const char *_format, ...) {
	va_list argList;
	va_start(argList, _format);
	char str[256];
	vsprintf_s(str, _format, argList);
	va_end(argList);

	glLineWidth(weight);//GLfloat width)
	//glColor3ub(color[0], color[1], color[2]);
	
	char* p = str;
		
		for (; (*p != '\0') && (*p != '\n'); p++) {
			glPushMatrix();
			{
				glTranslatef(position.x, position.y + height, 0);
				float s = height / FONT_DEFAULT_HEIGHT;
				glScalef(s, -s, s);
				glutStrokeCharacter(font, *p);
				position.x += fontGetWidth(*p);
			}
			glPopMatrix();
		}

	if (*p == '\n') {
		position.x = origin.x;
		position.y += fontGetLineHeight();
		//glTranslatef(0, height + weight*2, 0);
		fontDraw(++p);
	}
}
