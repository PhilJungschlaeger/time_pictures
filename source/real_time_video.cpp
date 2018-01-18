#include <stdio.h>            //?in/out?
#include <opencv2/opencv.hpp> //opencv library
#include <time.h>             //time measuring & seed
#include <string.h>           //? <3 string ?
#include <vector>             //storing image data
#include <sys/types.h>        //?folder?
#include <dirent.h>           //?folder?
#include <errno.h>            //?folder?
#include <iostream>           //?in/out?
#include <math.h>             //math
//test:
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;

int main(int argc, char** argv )
{

  //VIDEO:
  VideoCapture capture;   //to store the Video
  std::cout<<"video\n";
  capture=VideoCapture(argv[1]);    //first param: video
  ///capture.set(CV_CAP_PROP_FPS, 50);
  std::cout<<capture.get(CV_CAP_PROP_FPS)<<"\n";
  int len=50;

  Mat frame;
  capture>>frame;

  Mat frame_d;
  Mat frame_d_tmp=cv::Mat(frame.rows, frame.cols, CV_32FC3, cv::Scalar(0,0,0));  //a black picture;
  float factor=0.01;
  namedWindow("frame",1);
  for (int i=0; i<capture.get(CV_CAP_PROP_FRAME_COUNT); i++)
  {
    /*http://ninghang.blogspot.de/2012/11/list-of-mat-type-in-opencv.html
    integers:   VC8UC3 FAILS in color!
    float:      Is perfect in speed and color
    double:     brings speed limitation, maybe on a different machine
    */
    //std::cout<<i<<"\n";
    capture>>frame;
    //working float solution: wokrs fine
    std::cout<<i<<"\n";
    capture>>frame;
    frame.convertTo(frame_d, CV_32FC3, 1/255.0);
    if(waitKey(1) >= 0) break;
    imshow("frame",frame_d_tmp);
    addWeighted(frame_d_tmp,1-factor,frame_d,factor,0,frame_d_tmp);
    /*working double solution: but slow
    std::cout<<i<<"\n";
    capture>>frame;
    frame.convertTo(frame_d, CV_64FC3, 1/255.0);
    if(waitKey(1) >= 0) break;
    imshow("frame",frame_d_tmp);
    addWeighted(frame_d_tmp,1-factor,frame_d,factor,0,frame_d_tmp);
    */
  }
}
