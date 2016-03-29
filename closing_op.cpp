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

void process_image(PNG_Canvas_BW& image, int size = 5){
	int width = image.get_width();
	int height = image.get_height();
    bool** pixels = new bool*[width];
    for(int i=0; i<width; i++)
        pixels[i] = new bool[height];
    
    PNG_Canvas_BW output_image(width, height);

    MorphFilter dilation(size);
    MorphFilter erosion(size, true);
    cout<<"structure:\n";
    dilation.print();

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            if(image[i][j] > 128)
                pixels[i][j] = 0;
            else
                pixels[i][j] = 1;
        }
    }
    
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            output_image[i][j] = !dilation.apply(i, j, pixels, width, height)*255; 
        }
    }
    image = output_image;
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            output_image[i][j] = !erosion.apply(i, j, pixels, width, height)*255;
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
	
    if(argc > 3)	process_image(canvas, strtol(argv[3], NULL, 10));
    else            process_image(canvas);
	canvas.save_image(output_filename);
	
	
}
