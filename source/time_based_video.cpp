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
//#include "image_tool.cpp"
#include "video_tool.cpp"

using namespace cv;

int main(int argc, char** argv )
{

  //VIDEO: -loop img tool-
  /*
  VideoCapture capture;   //to store the Video
  std::cout<<"video\n";
  capture=VideoCapture(argv[1]);    //first param: video
  int len=50;
  Frame_factors fac(1,len);
  //fac.Average();
  //fac.Linear(0.1, 0);
  fac.Linear_plus(0.1,0,0.5);
  Time_based_video vid_maker(argv[1], fac, 0, 4000, len);
  vid_maker.render();
  */
  //Video: -dynamic long time-
  VideoCapture capture;   //to store the Video
  capture=VideoCapture(argv[1]);    //first param: video
  int len=50;                     //unnec
  Frame_factors fac(1,len);       //unnnc
  Time_based_video vid_maker(argv[1], fac, 0, 4000, len);
  vid_maker.render_dynamic_longtime();
}
