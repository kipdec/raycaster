#include "RayCalculations.h"
#include "structs.h"
#include "constants.h"
#include "math.h"

float dist(float ax, float ay, float bx, float by, float any){
    return ( sqrt((bx-ax) * (bx-ax) + (by-ay) * (by-ay)));
}

struct Ray calcHorizontalLines(
    int r, 
    float ra, 
    float px, 
    float py, 
    float mapX, 
    float mapY, 
    int map[]
){
    int mx,my,mp,dof; float rx,ry,xo,yo;
    dof = 0;
    float disH = 1000000, hx = px, hy =py;
    float aTan = -1/((double)tan(ra));
    if(ra > PI){ 
        ry = (((int) py >> 6) << 6) - 0.0001; 
        rx=(py-ry) * aTan+px;
        yo = -64; 
        xo =-yo * aTan;
    }; // Looking up
    if(ra < PI){ 
        ry = (((int) py >> 6) << 6) + 64; 
        rx=(py-ry) * aTan+px;
        yo = 64; 
        xo =-yo * aTan;
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
            hx = rx; 
            hy = ry;
            disH = dist(px, py, hx, hy, ra);
            dof = 8;
        } else {
            rx += xo;
            ry += yo;
            dof += 1;
        }
    }
    struct Ray ray = { hx, hy, disH };
    return ray;
}