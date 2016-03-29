/* image_processor_bw.cpp
   CSC 205 - Spring 2016
   
   Template for a grayscale PNG image processor.
   
   B. Bird - 03/02/2016
*/

#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "lodepng.h"
#include "filters.h"
#include "png_canvas.h"

using namespace std;
int width;
int height;
int interpolate(PNG_Canvas_BW& image, double x, double y){
    int x0 = floor(x);
    int y0 = floor(y);
    int x1 = ceil(x);
    int y1 = ceil(y);
    double xs = x - x0;
    double ys = y - y0;
    double p0 = (double)((x0>=width or y0>=height)? 128:image[x0][y0])  *(1 - xs) + (double)((x1>=width or y0>=height)? 128:image[x1][y0])*xs;
    double p1 = (double)((x0>=width or y1>=height)? 128:image[x0][y1])*(1 - xs) + (double)((x1>=width or y1>=height)? 128:image[x1][y1])*xs;
    return p0*(1 - ys) + p1*ys;
}
void process_image(PNG_Canvas_BW& image, string filename, int tx = 50, int ty = 50, int ax = 5, int ay = 3){
	width = image.get_width();
	height = image.get_height();
    printf("%d, %d, %d, %d\n",tx,ty,ax,ay);
    PNG_Canvas_BW output_image(width, height); 
    RippleTransform ripple(tx, ty, ax, ay);
    int x,y;
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            x = ripple.tix(i,j);
            y = ripple.tiy(i,j);
            output_image[i][j] = (x<0 or y<0)? 128 : interpolate(image, x, y);
        }
    }
    image = output_image;    
}



int main(int argc, char** argv){
	if (argc < 3){
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
		return 0;
	}
	string input_filename = argv[1];
	string output_filename = argv[2];
	
	PNG_Canvas_BW canvas;
	if (!canvas.load_image(input_filename)){
		cerr << "Unable to load " << input_filename << ". Exiting..." << endl;
		return 0;
	}
    if(argc == 7){
         process_image(canvas, output_filename, strtol(argv[3],NULL,10), strtol(argv[4],NULL,10),strtol(argv[5],NULL,10),strtol(argv[6],NULL,10));
    }
    else process_image(canvas, output_filename);
	canvas.save_image(output_filename);
    return 0;
}
