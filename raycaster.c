#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include "data.h"
#include "DrawFunctions.h"
#include "structs.h"
#include "constants.h"
#include "RayCalculations.h"

// Stopped at 8:20 in part 2
float px, py, pdx, pdy, pa;
int flashLightOn = 0;

typedef struct {
    int w, a, d, s;
} ButtonKeys; 

ButtonKeys Keys;

float maxDist = 500;
float maxBrightness = 0.5;
int tick = 0;

float degToRad(float a) { return a * M_PI /180.0;}
float FixAng(float a) { if (a > 359) { a -=370;} if (a<0){a += 360;} return a;}

void checkFlashLight(){
    if(flashLightOn == 1){
        maxDist = 1000;
    } else {
        maxDist = 300;
    }
}

void drawRays2D()
{

    // ra = Ray Angle
    // pa = Player Angle
    int r,mx,my,mp,dof; float rx,ry,ra,xo,yo,disT;

    // offset the ray angle 30 degrees from the player
    ra=pa - DR * 30; 

    // Shoot out 60 rays. 60 deg fov
    for(r=0; r<60; r++){

        // Guards
        if(ra< 0){ra += 2*PI;} 
        if(ra > 2 * PI){ ra -= 2 * PI;}

        // Calculate horizontal and vertical lines
        struct Ray hRay = calcHorizontalLines(
            r, 
            ra,
            px,
            py,
            mapX,
            mapY,
            map
        );

        struct Ray vRay = calcVerticalLines(
            r, 
            ra,
            px,
            py,
            mapX,
            mapY,
            map
        );

        int isVray = 1;
        // If we're loooking at a v wall
        if(vRay.dis < hRay.dis){
            rx = vRay.x;
            ry = vRay.y;
            disT = vRay.dis;
        } 

        // if we're looking at an h wall
        if(hRay.dis < vRay.dis) {
            rx = hRay.x;
            ry = hRay.y;
            disT = hRay.dis;
            isVray = 0;
        }

        // Used to keep track of shading
        float curMaxDist = maxDist;
        float curMaxBrightness = maxBrightness;
        if(flashLightOn == 1 && r > 15 && r < 45){
           curMaxDist = 1000; 
           float dCenter = fabs(30 - r);
           curMaxBrightness = (30 - dCenter) / 30;
        }
        float shade = curMaxBrightness - (disT/curMaxDist);
        if(shade > 1) { shade = 1; };
        if(shade < 0) { shade = 0; }
        if(isVray == 1){
            shade -= 0.1;
        }


        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();
        //--- Draw 3d Walls ---
        float ca = pa - ra; if (ca < 0){ ca += 2*PI;} if(ca > 2 * PI){ca -= 2 * PI;}
        disT = disT * cos(ca);
        int lineH = (mapS * 320) / disT;
        float ty_step=32.0 / (float) lineH;
        float ty_off=0;
        if(lineH> 320){ 
            ty_off=(lineH-320) / 2.0;
            lineH = 320;}
        int lineO = 160 - (lineH>>1);

        int y;
        float ty=ty_off*ty_step;
        float tx;
        if(isVray == 0){
            tx= (int)(rx/2.0)%32;
            if(ra < PI){tx = 31 - tx;}
        } else {
            tx= (int)(ry/2.0)%32;
            if(ra > PI / 2 && ra < (3*PI) / 2){tx = 31 - tx;}
        }
        for(y=0; y < lineH; y++){
            float c  = All_Textures[(int)(ty)*32 + (int)(tx)]*shade;
            glColor3f(c,c,c);
            glPointSize(8);glBegin(GL_POINTS);glVertex2i(r * 8 + 530, y + lineO); glEnd();
            ty += ty_step;
        }

        ra += DR;
    }
}

float oldFrame;
float frame;
float fps = 0;
int numFrames = 0;

void display(){
    if(numFrames == 0){
        oldFrame = glutGet(GLUT_ELAPSED_TIME);
    }
    // frames per second
    if(Keys.a == 1){ pa -= 0.1; if (pa < 0){ pa += 2*PI; } pdx = cos(pa) * 5; pdy = sin(pa) *5;}
    if(Keys.d == 1){ pa += 0.1; if (pa > 2*PI){ pa -= 2*PI; } pdx = cos(pa) * 5; pdy = sin(pa) *5;}
    
    int xo = 0; if(pdx < 0){xo =-20; } else { xo = 20;}
    int yo = 0; if (pdy < 0){yo =-20;} else {yo = 20;}

    int ipx = px / 64.0, ipx_add_xo=(px+xo)/64.0, ipx_sub_xo=(px-xo) /64.0;
    int ipy = py / 64.0, ipy_add_yo=(py+yo)/64.0, ipy_sub_yo=(py-yo) /64.0;
    if(Keys.w == 1){ 
        if(map[ipy*mapX + ipx_add_xo] == 0) { px += pdx;}
        if(map[ipy_add_yo*mapX +ipx] == 0) { py += pdy; }
    }
    if(Keys.s == 1){ 
        if(map[ipy*mapX + ipx_sub_xo] == 0){px -= pdx;} 
        if(map[ipy_sub_yo*mapX + ipx] == 0){py -= pdy;}
    }
    glutPostRedisplay();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D(map, mapX, mapY, mapS);
    //checkFlashLight();
    drawRays2D();
    drawPlayer(px,py,pdx,pdy,pa);
    drawFPS(fps);
    glutSwapBuffers();
    numFrames++;
    frame = glutGet(GLUT_ELAPSED_TIME);
    if(frame - oldFrame > 1000){
        fps = (numFrames * 1000) / (frame - oldFrame);
        numFrames = 0;
    }


    tick++;
}

void ButtonDown(unsigned char key, int x, int y){
    if(key == 'a'){ Keys.a = 1;}
    if(key == 'd'){Keys.d = 1;}
    if(key == 's'){Keys.s = 1;}
    if(key == 'w'){Keys.w = 1;}
    if(key == 'f'){
        if(flashLightOn == 1){
            flashLightOn = 0;
        } else {
            flashLightOn = 1;
        }
    }
    glutPostRedisplay();
}
void ButtonUp(unsigned char key, int x, int y){
    if(key == 'a'){ Keys.a = 0;}
    if(key == 'd'){Keys.d = 0;}
    if(key == 's'){Keys.s = 0;}
    if(key == 'w'){Keys.w = 0;}
    glutPostRedisplay();
}

void buttons(unsigned char key, int x, int y){
    if(key=='a'){ pa -= 0.1; if (pa < 0){ pa += 2*PI; } pdx = cos(pa) * 5; pdy = sin(pa) *5;}
    if(key=='d'){ pa += 0.1; if (pa > 2*PI){ pa -= 2*PI; } pdx = cos(pa) * 5; pdy = sin(pa) *5;}
    if(key=='w'){ px += pdx; py += pdy; }
    if(key=='s'){ px -= pdx; py -= pdy; }
    glutPostRedisplay();
}

void init(){
    glClearColor(0, 0, 0, 0);
    gluOrtho2D(0, 1024, 512, 0);
    px=300; py=300;pdx = cos(pa) * 5; pdy = sin(pa) *5;
}

void resize(int w, int h){
    glutReshapeWindow(1024, 512);
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Kip DeCastro");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(ButtonDown);
    glutKeyboardUpFunc(ButtonUp);
    glutMainLoop();
}