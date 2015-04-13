#include <iostream>
#include <vector>

#include "color_name.h"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

ColorName::ColorName(const string &path_to_model_file ):m_model_valid(false)
{
    FileStorage fs( path_to_model_file, FileStorage::READ);
    if(!fs.isOpened())
    {
        cout<<"Can not load the model file "<<path_to_model_file<<endl;
        return;
    }
    
    fs["matrix"] >> m_w2c;
    const int number_of_colors = 11;
    if( m_w2c.empty() || m_w2c.cols != number_of_colors)
    {
        cout<<"Model's file invalid , cols -> "<<number_of_colors<<endl;
        return;
    }

    /*  compute the index of the max value each row */
    get_the_maxindex();
    
    /*  prepare the RGB value for each color */
    m_colors.clear();
    m_colors.push_back( Vec3b(0,0,0));                    // black
    m_colors.push_back( Vec3b(255,0,0));                  // blue
    m_colors.push_back( Vec3b(0.25*255,0.4*255,0.5*255)); // brown
    m_colors.push_back( Vec3b(0.5*255,0.5*255,0.5*255));  // grey
    m_colors.push_back( Vec3b(0,255,0));                  // green
    m_colors.push_back( Vec3b(0,0.8*255,255));            // orange
    m_colors.push_back( Vec3b(255,0.5*255,255));          // pink
    m_colors.push_back( Vec3b(255,0,255));                // purple
    m_colors.push_back( Vec3b(0,0,255));                  // red
    m_colors.push_back( Vec3b(255,255,255));              // white
    m_colors.push_back( Vec3b(0,255,255));                // yellow


    /* set valid */
    m_model_valid = true;
}

bool ColorName::get_the_maxindex()
{
    if( m_w2c.empty())
        return false;

    m_w2c_max = Mat::zeros( m_w2c.rows, 1, CV_8U);
    
    for( unsigned int row_index = 0; m_w2c.rows > row_index; row_index++)
    {
        int max_index = 0;
        double max_value = -1;
        for( unsigned int col_index=0;m_w2c.cols>col_index;col_index++)
        {
            if( m_w2c.at<double>(row_index, col_index) > max_value)
            {
                max_value = m_w2c.at<double>(row_index, col_index);
                max_index = col_index;
            }
        }
        m_w2c_max.at<uchar>(row_index,0) = max_index;
    }

    return true;
}

bool ColorName::get_color_name( const Mat &input_image,    /*  in: input image */
                                Mat &output_image,         /* out: output of the algorithm*/
                                const int type) const      /*  in: type == 0 -> only return the color's type in output_image( value ranges from 0 to 10)
                                                                   type == 1 -> further map the color's type to the real color value( good to visualize the result)*/

{
    /* check if it is a color image */
    if( input_image.channels() != 3 || input_image.empty() )
    {
        cout<<"The input image should be BGR channels"<<endl;
        return false;
    }

    /*  since the output value's range is [0, 32767] , we can not use opencv's LUT() function*/
    Mat color_type_image = Mat::zeros( input_image.size(), CV_8U);
    for( unsigned int r_index=0;r_index<color_type_image.rows;r_index++)
    {
        const uchar *row_ptr = static_cast< const uchar*>( input_image.ptr(r_index));
        for( unsigned int c_index=0;c_index<color_type_image.cols;c_index++)
        {
            int index_value = 32*32*cvFloor(*row_ptr*1.0/8)+32*cvFloor(*(row_ptr+1)*1.0/8)+cvFloor(*(row_ptr+2)*1.0/8);
            color_type_image.at<uchar>( r_index, c_index) = m_w2c_max.at<uchar>( index_value,0);
            row_ptr += 3; /* cross BGR 3 pixels */
        }
    }
    /*  only need the color's type */
    if(type == 0)
    {
        output_image = color_type_image;
        return true;
    }
    
    /*  convert the type to actual color */
    Mat color_output_image = Mat::zeros( input_image.size(), CV_8UC3);
   
    for(unsigned int r_index=0;r_index<color_output_image.rows;r_index++)
    {
        for(unsigned int c_index=0;c_index<color_output_image.cols;c_index++)
        {
            color_output_image.at<Vec3b>(r_index,c_index) = m_colors[color_type_image.at<uchar>(r_index,c_index)];
        }
    }
    output_image = color_output_image;
    return true;
}
