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
	printf("bfSize :%d\n", bf.bfSize);
	BITMAPINFOHEADER bi;
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pFile);
	printf("biWidth: %d biHeight : %d\n", bi.biWidth, bi.biHeight);
	printf("biCount: %d ", bi.biBitCount);
	typedef struct {
		unsigned char r, g, b;

	}RGB;
	RGB *pixels = (RGB*)malloc(sizeof RGB * bi.biWidth * bi.biHeight);
	fread(pixels, sizeof(RGB), bi.biWidth* bi.biHeight,pFile);

	for (int y=0;y<bi.biHeight;y++)
		for (int x = 0; x < bi.biWidth; x++) {
			RGB* pPixel = &pixels[y * bi.biWidth + x];
			unsigned char temp = pPixel->r;
			pPixel->r = pPixel->b;
			pPixel->b = temp;
		}
	for (int y = 0; y < bi.biHeight/2; y++)
		for (int x = 0; x < bi.biWidth; x++) {
			RGB* pPixel0 = &pixels[y * bi.biWidth + x];
			RGB* pPixel1 = &pixels[(bi.biHeight - 1 - y) * bi.biWidth + x];
			RGB temp = *pPixel0;
			*pPixel0 = *pPixel1;
			*pPixel1 = temp;
		}
	//GLint param);
	glTexImage2D(
		GL_TEXTURE_2D,	//GLenum target,
		0,	//GLint level, 
		GL_RGB,	//GLint internalformat, 
		bi.biWidth,	//GLsizei width, 
		bi.biHeight,	//GLsizei height, 
		0,	//GLint border, 
		GL_RGB,	//GLenum format, 
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