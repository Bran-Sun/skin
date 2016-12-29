#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


void detectHScolor(const cv::Mat & image, double minHue , double maxHue,double minSat,double maxSat,cv::Mat &mask )
{
    cv::Mat hsv;
    cv::cvtColor (image,hsv ,CV_BGR2HSV);
    
    std::vector<cv::Mat> channels;
    cv::split(hsv,channels);
    
    cv::Mat mask1;
    cv::threshold (channels[0],mask1,maxHue,255,cv::THRESH_BINARY_INV);
    cv::Mat mask2;
    cv::threshold (channels[0],mask2,minHue,255,cv::THRESH_BINARY);
    
    cv::Mat huemask;
    if(minHue<maxHue)
        huemask=mask1 & mask2;
    else
        huemask=mask1 | mask2;
    
    cv::threshold (channels[1],mask1,maxSat,255,cv::THRESH_BINARY_INV);
    cv::threshold (channels[1],mask2,minSat,255,cv::THRESH_BINARY);
    
    cv::Mat satMask;
    satMask=mask1 &mask2;
    
    mask=huemask & satMask;
    
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
    
    cv::imshow("result",detected);
    cv::waitKey (0);
    return 0;
}