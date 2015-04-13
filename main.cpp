#include <string>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"

#include "color_name.h"

using namespace std;
using namespace cv;

int main( int argc, char** argv)
{
    ColorName cn("w2c_model.xml");

    Mat input_image = imread( argv[1]);
    Mat output_image;

    /*  1 get the type of the color */
    //cn.get_color_name( input_image, output_image, 0);
    //cout<<"output image is "<<output_image<<endl;

    /*  2 show */
    cn.get_color_name( input_image, output_image, 1);

    imshow("output", output_image);
    waitKey(0);
    return 0;
}


