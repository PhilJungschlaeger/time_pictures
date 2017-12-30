
#include <stdio.h>
#include <opencv2/opencv.hpp> //image operations
#include <time.h>             //time measuring & seed
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <math.h>
using namespace cv;
struct Color{
  Color(float red, float green, float blue) : r(red), g(green), b(blue) {}
  Color(): r(0.0f), g(0.0f), b(0.0f) {}

 //VARIABLEN-------------------------------------------------------------------
 float r;
 float g;
 float b;

 //FUNKTIONEN:OPERATIONEN-------------------------------------------------------
 Color& operator+=(Color const& other)
 {
   r += other.r;
   g += other.g;
   b += other.b;
   return *this;
 }
 friend std::ostream& operator<<(std::ostream& os, Color const& c)
    {
      os << "(" << c.r << "," << c.g << "," << c.b << ")";
      return os;
    }
    friend bool operator==(Color const& lhs, Color const& rhs)
    {
      return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}
};

int SIZEX=848;
int SIZEY=480;
float faktor=1;
int main(int argc, char** argv )
{

    std::string line;
    std::ifstream myfile(argv[1]);
    std::vector<Color> clr;
    if (myfile.is_open())
    {
        std::cout <<"Deine mutter rotzt in der gegnd umher Vol.i" <<"\n";
        while (getline(myfile,line))
        {
            std::stringstream ss;
            ss<<line;                   //First Line in
            std::string firstWord;
            Color in;
            ss>> in.r;
            ss>> in.g;
            ss>> in.b;
            clr.push_back(in);
        }
      }
      for (int h=0; h<10;h++)
      {
        float max=0;
        std::cout<<"faktor?\n";
        std::cin>>faktor;
        for ( std::vector<Color>::iterator p = clr.begin(); p != clr.end(); p++ ) {

          max=std::max(max,p->r);
          max=std::max(max,p->g);
          max=std::max(max,p->b);
        }
        std::cout<<"max: "<<max<<"\n";
        float scale=255/(log(1+max*faktor));

        cv::Mat dst=cv::Mat(SIZEY, SIZEX, CV_8UC3, cv::Scalar(0,0,0));  //a black picture
        int k=0;
        for(int x=0; x<SIZEX; x++)
        {
          for(int y=0; y<SIZEY; y++)
          {
            dst.at<Vec3b>(Point(x,y))[0]=scale*log(1+clr[k].r*faktor);
            dst.at<Vec3b>(Point(x,y))[1]=scale*log(1+clr[k].g*faktor);
            dst.at<Vec3b>(Point(x,y))[2]=scale*log(1+clr[k].b*faktor);
            k++;
          }
        }
        imwrite( "./out.jpg", dst );
        imshow("Display window", dst);

      }

    }
