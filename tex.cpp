#include "tex.h"
#include "stdio.h"
#include <Windows.h>
#include "glut.h"

int texFromBPM(const char* _fileName) {
	FILE* pFile;
	fopen_s(&pFile,_fileName,"rb");
	if (pFile == nullptr) {
		printf("%s open failed!\n", _fileName);
		return 1;
	}
	printf("%s open \n", _fileName);
	BITMAPFILEHEADER bf;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, pFile);
	//printf("bfSize :%d\n", bf.bfSize);
	BITMAPINFOHEADER bi;
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pFile);
	printf("biWidth: %d biHeight : %d\n", bi.biWidth, bi.biHeight);
	printf("biCount: %d ", bi.biBitCount);
	typedef struct {
		unsigned char r, g, b, a;

	}RGBA;
	RGBA *pixels = (RGBA*)malloc(sizeof RGBA * bi.biWidth * bi.biHeight);
	fread(pixels, sizeof(RGBA), bi.biWidth* bi.biHeight,pFile);

	//GLint param);
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
	free(pixels);
	fclose(pFile);
	return 0; 
}