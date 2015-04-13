#ifndef COLOR_NAME_H
#define COLOR_NAME_H

#include <string>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class ColorName
{
    /*  giving a image, return the type of the color, 11 types in total */
    /*
     0  -> black
     1  -> blue
     2  -> brown
     3  -> grey
     4  -> green
     5  -> orange
     6  -> pink
     7  -> purple
     8  -> red
     9  -> white
     10 -> yellow
     */

    public:

        /* 
         * ===  FUNCTION  ======================================================================
         *         Name:  ColorName
         *  Description:  constructor, path of model file must be giving
         * =====================================================================================
         */
        ColorName( const string &path_to_model_file );   /* in: path of the color model*/



        /* 
         * ===  FUNCTION  ======================================================================
         *         Name:  get_color_name
         *  Description:  get the color type
         * =====================================================================================
         */
        bool get_color_name( const Mat &input_image,    /*  in: input image */
                             Mat &output_image,         /* out: output of the algorithm*/
                             const int type = 0) const; /*  in: type == 0 -> only return the color's type in output_image( value ranges from 0 to 10)
                                                                type == else -> further map the color's type to the real color value( good to visualize the result)*/

    private:
        /*  no copy */
        ColorName( const ColorName & rhs);
        ColorName& operator=( const ColorName &rhs);



        /* 
         * ===  FUNCTION  ======================================================================
         *         Name:  get_the_maxindex
         *  Description:  fill the matrix m_w2c_max
         * =====================================================================================
         */
        bool get_the_maxindex();

        /* model file, word_2_color map */
        Mat m_w2c; 

        /* stores the index of max value of the m_w2c */
        Mat m_w2c_max;

        /* each color's corresponding RGB value */
        vector<Vec3b> m_colors;

        /* ready to go ? */
        bool m_model_valid;
};

#endif
