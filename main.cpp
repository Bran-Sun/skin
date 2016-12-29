#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


void detectHScolor(const cv::Mat & image, double minHue , double maxHue,double minSat,double maxSat,cv::Mat &mask )
{
    cv::Mat hsv;
    cv::cvtColor (image , hsv , CV_BGR2HSV);
    
    std::vector<cv::Mat> channels;
    cv::split (hsv , channels);
    
    cv::Mat mask1;
    cv::threshold (channels[0] , mask1 , maxHue , 255 , cv::THRESH_BINARY_INV);
    cv::Mat mask2;
    cv::threshold (channels[0] , mask2 , minHue , 255 , cv::THRESH_BINARY);
    
    cv::Mat huemask;
    if ( minHue < maxHue )
        huemask = mask1 & mask2;
    else
        huemask = mask1 | mask2;
    
    cv::threshold (channels[1] , mask1 , maxSat , 255 , cv::THRESH_BINARY_INV);
    cv::threshold (channels[1] , mask2 , minSat , 255 , cv::THRESH_BINARY);
    
    cv::Mat satMask;
    satMask = mask1 & mask2;
    
    mask = huemask & satMask;
}

inline void procImage (cv::Mat &img , cv::Mat &output , int threshold , int erode_size , int dilate_size)
{
    cv::Mat erode_elem = cv::getStructuringElement (cv::MORPH_RECT , cv::Size (erode_size + 1 , erode_size + 1));
    cv::Mat dilate_elem_1 = cv::getStructuringElement (cv::MORPH_RECT , cv::Size (dilate_size + 1 , dilate_size + 1));
    cv::Mat dilate_elem_2 = cv::getStructuringElement (cv::MORPH_ELLIPSE ,
                                                       cv::Size (dilate_size + 1 , dilate_size + 1));
    
    cv::threshold (img , output , threshold , 255 , cv::THRESH_BINARY | cv::THRESH_OTSU);
    
    cv::imshow ("befire" , img);
    
    cv::erode (output , output , erode_elem);
    //   cv::imshow("erode",output);
    cv::dilate (output , output , dilate_elem_1);
    cv::dilate (output , output , dilate_elem_2);
    cv::imshow ("dilate" , output);
}


inline cv::Rect findMaxContour (cv::Mat &img)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours (img , contours , cv::RETR_LIST , cv::CHAIN_APPROX_NONE);
    
    double max_area = 0;
    std::vector<cv::Point> max_contour;
    
    for ( int i = 0; i < contours.size (); i++ )
    {
        double area = cv::contourArea (contours[i]);
        
        if ( area > max_area )
        {
            max_area = area;
            max_contour = contours[i];
        }
    }
    if ( contours.size () == 0 )
        max_contour.push_back (cv::Point (0 , 0));
    
    return cv::boundingRect (max_contour);
}



int main ()
{
    cv::Mat image;
    image=cv::imread("person.jpg");
    cv::imshow ("origin",image);
    
    cv::Mat mask;
    detectHScolor (image ,160,10,15,166,mask);
    cv::Mat detected(image.size(),CV_8UC3,cv::Scalar(0,0,0));
    image.copyTo (detected,mask);
    
    cv::Mat gray , output;
    cv::cvtColor (detected , gray , CV_BGR2GRAY);
    procImage (gray , output , 0 , 5 , 15);
    cv::Rect MaxRect = findMaxContour (output);
    cv::rectangle (image , MaxRect , cv::Scalar (0 , 0 , 255));
    
    cv::imshow ("output" , image);
    cv::imshow("result",detected);
    cv::waitKey (0);
    return 0;
}