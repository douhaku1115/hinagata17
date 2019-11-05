#pragma once
#include <stdlib.h>

#include "glut.h"
#include "glm/glm.hpp"

#define FONT_DEFAULT_HEIGHT (100.f)
void fontBegin();
void fontEnd();

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

