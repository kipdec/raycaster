#ifndef RAYCALCULATIONS_H_INCLUDED
#define RAYCALCULATIONS_H_INCLUDED

float dist(float ax, float ay, float bx, float by, float any);
struct Ray calcHorizontalLines(
    int r, 
    float ra, 
    float px, 
    float py, 
    float mapX, 
    float mapY, 
    int map[]
);
struct Ray calcVerticalLines(
    int r, 
    float ra, 
    float px, 
    float py, 
    float mapX, 
    float mapY, 
    int map[]
);
#endif