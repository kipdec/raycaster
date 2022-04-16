#ifndef DRAWFUNCTIONS_H_INCLUDED
#define DRAWFUNCTIONS_H_INCLUDED
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void drawPlayer(float px, float py, float pdx, float pdy, float pa);
void drawMap2D(int map[], int mapX, int mapY, int mapS);

#endif