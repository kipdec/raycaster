#include "DrawFunctions.h"

void drawPlayer(float px, float py, float pdx, float pdy, float pa)
{
    glColor3f(1,1,0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px,py);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px + pdx * 5, py + pdy * 5);
    glEnd();
}

void drawMap2D(int map[], int mapX, int mapY, int mapS){
    int x, y, xo, yo;
    for(y = 0; y < mapY; y++){
        for(x = 0; x < mapX; x++){
            if(map[y*mapX+x] == 1) {
                glColor3f(1,1,1);
            } else {
                glColor3f(0,0,0);
            }
            xo = x*mapS;
            yo = y*mapS;
            glBegin(GL_QUADS);
            glVertex2i(xo + 1,yo + 1);
            glVertex2i(xo + 1, yo+mapS - 1);
            glVertex2i(xo+mapS -1, yo+mapS - 1);
            glVertex2i(xo+mapS-1, yo+ 1);
            glEnd();
        }
    }
}