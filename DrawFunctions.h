#ifndef DRAWFUNCTIONS_H_INCLUDED
#define DRAWFUNCTIONS_H_INCLUDED
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>

void drawPlayer(float px, float py, float pdx, float pdy, float pa);
void drawMap2D(int map[], int mapX, int mapY, int mapS);
void drawFPS(float fps);
void printText(char *text, const GLfloat color[3], float posX, float posY);

#endif