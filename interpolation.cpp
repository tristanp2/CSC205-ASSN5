/* image_processor_bw.cpp
   CSC 205 - Spring 2016
   
   Template for a grayscale PNG image processor.
   
   B. Bird - 03/02/2016
*/

#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "lodepng.h"
#include "filters.h"
#include "png_canvas.h"

using namespace std;
int width;
int height;
bool oob_check(int n, int max){
    if(n<0 or n>=max) return 128;
    else return n;
}
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
void process_image(PNG_Canvas_BW& image, string filename, int new_width = -1, int new_height = -1){
	width = image.get_width();
	height = image.get_height();
    cout<<width<<endl<<height<<endl;
    if(new_width < 0) new_width = 3*width;
    if(new_height < 0) new_height = 3*height;
    double ratio_x = (double)width / new_width;
    double ratio_y = (double)height / new_height;
    PNG_Canvas_BW output_image(new_width, new_height); 
    
    for(int i=0; i<new_width; i++){
        for(int j=0; j<new_width; j++){
            
            output_image[i][j] = interpolate(image, ratio_x*i, ratio_y*j);
        }
    }

    output_image.save_image(filename);
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
    if(argc == 5){
         process_image(canvas, output_filename, strtol(argv[3],NULL,10), strtol(argv[4],NULL,10));
    }
    else process_image(canvas, output_filename);
//  something odd is happening here for certain output sizes
//	canvas.save_image(output_filename);
    return 0;
}
