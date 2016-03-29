#ifndef FILTERS_H
#define FILTERS_H

#include <cmath>
#include "png_canvas.h"

using namespace std;

//Defaults to dilation
//structural element is a diamond
class MorphFilter{
public:
    MorphFilter(int size, bool erosion = false){
        this->size = size;
        this->erosion = erosion;
        generate_filter();
    }
    
    int apply(int x, int y, bool** image, int width, int height){
        int result = (int)erosion;
        int p, q;
        for(int i = 0; i<size; i++){
            p = i - size/2;
            if((x + p) < 0 or (x + p) >= width) p = -p;
            for(int j = 0; j<size; j++){
                q = j - size/2;
                if((y + q) < 0 or (y + q) >= height) q = -q;
                if(erosion){
                    if(filter[i][j] == 1)
                        result *= image[x + p][y + q];
                }
                else{
                    if(filter[i][j] == 1)
                        result += image[x + p][y + q];
                }
            }
        }
        return result > 0;
    }
    void print(){
        for(int i=0; i<size; i++){
            for(int j=0; j<size; j++){
                cout<<filter[i][j]<<"\t";
            }
            cout<<endl;
        }
    }


private:
    int** filter;
    int size;
    bool erosion; //false denotes dilation, true denotes erosion

    void generate_filter(){
        filter = new int*[size];
        for(int i=0; i<size; i++) filter[i] = new int[size];
        int p,q;
        for(int i=0; i<size; i++){
            p = i - size/2;
            for(int j=0; j<size; j++){
                q = j - size/2;
                if((abs(p) + abs(q)) <= size/2){
                    filter[i][j] = 1;
                }
                else filter[i][j] = 0;
            }
        }
        filter[size/2][size/2] = 0;
    }
};
class RippleTransform{
public:
    RippleTransform(int tx, int ty, int ax, int ay){
        this->tx = tx;
        this->ty = ty;
        this->ax = ax;
        this->ay = ay;
    }
    double tix(int xp, int yp){
        return (double)xp + (double)ax*sin(2*M_PI*yp/tx);
    }
    double tiy(int xp, int yp){
        return (double)yp + (double)ay*sin(2*M_PI*xp/ty);
    }
private:
    int tx, ty, ax, ay;
};
class TwirlyTransform{
public:
    TwirlyTransform(int rmax, int xc, int yc, int alpha){
        this->rmax = rmax;
        this->xc = xc;
        this->yc = yc;
        this->alpha = (double)alpha*M_PI/180;
        cout<<alpha<<endl;
    }
    double tix(int xp, int yp){
        int dx = xp - xc, dy = yp - yc;
        double r = sqrt(dx*dx + dy*dy);
        if(r > rmax)    return xp;

        double beta = atan2(dy,dx) + alpha*((rmax - r)/rmax); 
        return xc + r*cos(beta);
    }
    double tiy(int xp, int yp){
        int dx = xp - xc, dy = yp - yc;
        double r = sqrt(dx*dx + dy*dy);
        if(r > rmax)    return yp;

        double beta = atan2(dy,dx) + alpha*((rmax - r)/rmax); 
        return yc + r*sin(beta);
    }
private:
    int rmax, xc, yc;
    double alpha;
};
#endif
