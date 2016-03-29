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
#endif
