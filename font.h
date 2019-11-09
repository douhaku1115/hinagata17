#pragma once
#include <stdlib.h>

#include "glut.h"
#include "glm/glm.hpp"

#define FONT_DEFAULT_HEIGHT (100.f)
enum {
	FONT_FONT_ROMAN,
	FONT_FONT_MONO_ROMAN,
	FONT_FONT_MAX
};
void fontBegin();
void fontEnd();
void fontFont(int _font);
void fontPosition(float _x, float _y);
void fontHeight(float _size);
float fontGetWidth(int _character);
float fontGetHeight();
float fontGetWeight();

float fontGetWeightMin();
float fontGetWeightMax();
void fontWeight(float _weight);
//void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue);
void fontDraw(const char *_format,...);

