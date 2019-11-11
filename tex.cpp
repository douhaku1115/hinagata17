#include "tex.h"
#include "stdio.h"
#include <Windows.h>
#include "glut.h"

int texFromBPM(const char* _fileName, unsigned char* _colorKey) {
	FILE* pFile;
	fopen_s(&pFile,_fileName,"rb");
	if (pFile == nullptr) {
		printf("%s open failed!\n", _fileName);
		return 1;
	}
	printf("%s open \n", _fileName);
	BITMAPFILEHEADER bf;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, pFile);
	printf("bfSize :%d\n", bf.bfSize);
	BITMAPINFOHEADER bi;
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pFile);
	printf("biWidth: %d biHeight : %d\n", bi.biWidth, bi.biHeight);
	printf("biCount: %d ", bi.biBitCount);
	typedef struct {
		unsigned char r, g, b;

	}RGB;
	RGB *bits = (RGB*)malloc(sizeof RGB * bi.biWidth * bi.biHeight);
	fread(bits, sizeof(RGB), bi.biWidth* bi.biHeight,pFile);

	for (int y=0;y<bi.biHeight;y++)
		for (int x = 0; x < bi.biWidth; x++) {
			RGB* pBit = &bits[y * bi.biWidth + x];
			unsigned char temp = pBit->r;
			pBit->r = pBit->b;
			pBit->b = temp;
		}
	for (int y = 0; y < bi.biHeight/2; y++)
		for (int x = 0; x < bi.biWidth; x++) {
			RGB* pBit0 = &bits[y * bi.biWidth + x];
			RGB* pBit1 = &bits[(bi.biHeight - 1 - y) * bi.biWidth + x];
			RGB temp = *pBit0;
			*pBit0 = *pBit1;
			*pBit1 = temp;
		}
	typedef struct {
		unsigned char r, g, b, a;

	}RGBA;
	RGBA* pixels = (RGBA*)malloc(sizeof RGBA * bi.biWidth * bi.biHeight);
	for (int y = 0; y < bi.biHeight ; y++)
		for (int x = 0; x < bi.biWidth; x++) {
			memcpy(
				&pixels[y * bi.biWidth + x],
				&bits[y * bi.biWidth + x],
				sizeof RGB);

			RGBA* pPixel = &pixels[y * bi.biWidth + x];
			pPixel->a = ((_colorKey != nullptr)
				&& (pPixel->r == _colorKey[0])
				&& (pPixel->g == _colorKey[1])
				&& (pPixel->b == _colorKey[2]))
				? 0x00
				: 0xff;
		}

	glTexImage2D(
		GL_TEXTURE_2D,	//GLenum target,
		0,	//GLint level, 
		GL_RGBA,	//GLint internalformat, 
		bi.biWidth,	//GLsizei width, 
		bi.biHeight,	//GLsizei height, 
		0,	//GLint border, 
		GL_RGBA,	//GLenum format, 
		GL_UNSIGNED_BYTE,	//GLenum type, 
		pixels);//const GLvoid * pixels););
	
	glTexParameteri(
		GL_TEXTURE_2D,//GLenum target, 
		GL_TEXTURE_MAG_FILTER,//GLenum pname, 
		GL_NEAREST);	//GLint param);
	glTexParameteri(
		GL_TEXTURE_2D,//GLenum target, 
		GL_TEXTURE_MIN_FILTER,//GLenum pname, 
		GL_NEAREST);	//GLint param);
	free(bits);
	free(pixels);
	fclose(pFile);
	return 0; 
}
int texFromBPM(const char* _fileName,
	unsigned char _colorKeyR,
	unsigned char _colorKeyG,
	unsigned char _colorKeyB) {
	unsigned char colorKey[] = {
		_colorKeyR,
		_colorKeyG,
		_colorKeyB };
	return texFromBPM(_fileName, colorKey);
}
