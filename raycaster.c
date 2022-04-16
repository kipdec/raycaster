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

typedef struct {
    int w, a, d, s;
} ButtonKeys; ButtonKeys Keys;



float degToRad(float a) { return a * M_PI /180.0;}
float FixAng(float a) { if (a > 359) { a -=370;} if (a<0){a += 360;} return a;}

float dist(float ax, float ay, float bx, float by, float any){
    return ( sqrt((bx-ax) * (bx-ax) + (by-ay) * (by-ay)));
}

Ray drawVeriticalLines(int r, float ra){
    int mx,my,mp,dof; float rx,ry,xo,yo;
    dof = 0;
    float disV = 1000000, vx = px, vy =py;
    float nTan = -tan(ra);
    if(ra > P2 && ra < P3){ 
        rx = (((int) px >> 6) << 6) - 0.0001; 
        ry=(px-rx) * nTan+py;
        xo = -64; 
        yo =-xo * nTan;
    }; // Looking up
    if(ra < P2 || ra > P3){ 
        rx = (((int) px >> 6) << 6) + 64; 
        ry=(px-rx) * nTan+py;
        xo = 64; 
        yo =-xo * nTan;
    }; // Looking up
    if(ra == 0 || ra == PI){
        rx = px;
        ry = py;
        dof = 8;
    }
    while(dof < 8){
        mx = (int) (rx) >> 6;
        my = (int)(ry) >> 6;
        mp = my * mapX + mx;
        if(mp > 0 && mp < mapX * mapY && map[mp] == 1){
            // hit
            vx = rx; 
            vy = ry;
            disV = dist(px, py, vx, vy, ra);
            dof = 8;
        } else {
            rx += xo;
            ry += yo;
            dof += 1;
        }
    }
    Ray ray = { vx, vy, disV };
    return ray;
}

void drawRays2D()
{
    int r,mx,my,mp,dof; float rx,ry,ra,xo,yo,disT;
    ra=pa - DR * 30; 
    for(r=0; r<60; r++){
        if(ra< 0){ra += 2*PI;} if(ra > 2 * PI){ ra -= 2 * PI;}
        Ray hRay = calcHorizontalLines(
            r, 
            ra,
            px,
            py,
            mapX,
            mapY,
            map
        );
        Ray vRay = drawVeriticalLines(r, ra);
        float shade = 1;
        if(vRay.dis < hRay.dis){
            rx = vRay.x;
            ry = vRay.y;
            disT = vRay.dis;
            shade = 0.5;
        } 
        if(hRay.dis < vRay.dis) {
            rx = hRay.x;
            ry = hRay.y;
            disT = hRay.dis;
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
        if(shade == 1){
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

float frame1, frame2, fps;

void display(){
    // frames per second
    frame2 = glutGet(GLUT_ELAPSED_TIME);
    fps = (frame2 - frame1);
    frame1 = glutGet(GLUT_ELAPSED_TIME);
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
    drawRays2D();
    drawPlayer(px,py,pdx,pdy,pa);
    glutSwapBuffers();
}

void ButtonDown(unsigned char key, int x, int y){
    if(key == 'a'){ Keys.a = 1;}
    if(key == 'd'){Keys.d = 1;}
    if(key == 's'){Keys.s = 1;}
    if(key == 'w'){Keys.w = 1;}
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
    glClearColor(0.3, 0.3, 0.3, 0);
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