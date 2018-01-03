/*  time_based_picture
Idee:
      über eine Reihe an Bildern(Serie, Video) iteriert dieses tool.
      Es ermöglicht eine Bild, welches ein beliebiges Integral über
      die Serie darstellt. Für dieses finale Bild stehen so deutlich
      mehr Informationen zur verfügung und eine über die Zeit gewertete
      interpretation ist möglich.

Anwendung:
      bei einer Langzeitbelichtung ist man nicht mehr auf einen filter
      angewiesen und beschränkt. nachträglich kann man die theoretische
      belichtungszeit wählen
      ...
      es fehlt noch die möglichkeit in pixeln faktoren zu verstecken

Benutzung:
      input:      video oder Ordner mit bildern
      parameter:  (float) gewünschte intensität(0-1000)
                  ()  belichtungs-relations-funktion
                  ()  hdr kurve

?folge?:
      -real-time-tool
        ->editing curve of image areas..
      -wie viele bildr kann mein pc zwischenspeichern?

!einschränkungen!:
      -normal-gut-belichtete Bilder sind sinvoll:
        -verbrennung(255) kann man nur im grenzbereich leicht korrigieren
        -totale schwärze(0) kann man nur im grenzbereich leicht korrigieren
*/

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


using namespace cv;

void usage(){}

/*
Wenn ein Bild aus mehreren Bildern bestehen soll muss jedes Bild einen Faktor bekommen
In frame faktors findet man eine Collection aus möglichen Vorfaktoren:
-Average: gewöhnlicher Graufilter (die Anzahl an Bildern bestimmt die Stärke des
          Graufilters), eine konstante funktion

-Linear:  Ein Linearer Verlauf, mit einem Fator und einer Konstanten ermöglicht eine
          Graufilter ändrung in der zeit..

-....
*/
struct Frame_factors{
  Frame_factors(double i,int c):
  Intensity(i),
  Count(c)
  {}

  std::vector<double>& Average(){
    double value=Intensity/((double)Count);
    Factors.clear();
    for (int i=0; i<(Count); i++)
    {
        Factors.push_back(value);
    }
    return Factors;
  }

  //rise: +1 or -1 please
  std::vector<double>& Linear(double rise, double constant){
    Factors.clear();
    double sum=0;
    double factor=0;
    if(rise>=0.0f)
    {
      std::cout<<rise<<"big\n";
      for(int i=0; i<Count; i++)
      {
          double value=i;//+from;
          Factors.push_back(value);
          sum+=value;
      }

    }else{
      std::cout<<"sdds\n";
      for(int i=0; i<Count; i++)
      {
          double value=Count-i;//+from;
          Factors.push_back(value);
          sum+=value;
      }

    }


    factor=(Intensity-constant)/sum;
    constant=constant/Count;
    for(int i=0; i<Count; i++)
    {
        Factors[i]=Factors[i]*factor+constant;
        std::cout<<Factors[i]<<"\n";
    }
    return Factors;
  }

  //member
  double                Intensity;
  int                   Count;  //calculated images
  std::vector<double>   Factors;
};

class Time_based_picture{
public:
  Time_based_picture(VideoCapture& pictures, Frame_factors& factors, int start, int end):
      _Pictures(pictures),
      _Factors(factors),
      _Start(start),
      _End(end)
      {}

  void render(){
    cv::Mat currentFrame;
    _Pictures>>currentFrame;
    _X=currentFrame.cols;
    _Y=currentFrame.rows;
    int totalFrameNumber = _Pictures.get(CV_CAP_PROP_FRAME_COUNT);
    std::cout<<"expects: "<<totalFrameNumber<<" frames\n";


    cv::Mat dst=cv::Mat(_Y, _X, CV_64FC3, cv::Scalar(0.0f,0.0f,0.0f));
    int err_count=0;
    int faktor_count=0;

    for (unsigned int i = 0;i <totalFrameNumber-1; i++) //through video
    {
      if(i>=_Start && i<=_End+err_count)                //in our range?
      {
        if(!currentFrame.empty()){
          if(!(i%50))  //just to print progress
          {
            std::cout<<i<<" of "<<totalFrameNumber<<"\n";
          }
          for(int x=0; x<_X; x++)
          {
            for(int y=0; y<_Y; y++)
            {
              for (int c=0; c<3; c++)
              {
                dst.at<Vec3d>(Point(x,y))[c]+=(double)_Factors.Factors[faktor_count]*currentFrame.at<Vec3b>(Point(x,y))[c];
              }
            }
          }
          faktor_count++;
            }else{
          //sometime frames are empty..!?
          err_count++;
        }
      }
      if(i>=_End)
      {
        break;
      }
      _Pictures>>currentFrame;
    }
    std::cout<<"empty images: "<<err_count<<"\n";

    //calc:


    std::cout<<"done\n";
    imwrite( "./outf.jpg", dst );
  }
private:
  int                 _X;
  int                 _Y;
  VideoCapture        _Pictures;
  Frame_factors       _Factors;
  int                 _Start;
  int                 _End;

};

int main(int argc, char** argv )
{
  //VIDEO:
  VideoCapture capture;   //to store the Video
  std::cout<<"video\n";
  capture=VideoCapture(argv[1]);    //first param: video
  Frame_factors fac(1,100);
  fac.Average();
  //fac.Linear(0.1, 0);
  Time_based_picture pic_maker(capture, fac, 0, 100);
  pic_maker.render();
  //reade input:
}
////////////////////////////////////////////////
/*
ermöglicht den schnellen zugriff auf viele bilder...
  -> real-time application
  //nur mein laptop packt maximal 870 kleine bilder...
  //wie genau ist da der zusammenhang mit dem speicher?
  use:
  (fromvid)
  if(!currentFrame.empty()){
    storage.append(currentFrame);
  }
  ...
  cv::Mat dst=cv::Mat(SIZEY, SIZEX, CV_64FC3, cv::Scalar(0.0f,0.0f,0.0f));
  int index=0;
  std::cout<<"storage._count "<<storage._count<<"\n";
  for(int i =0; i<storage._count; i++){
    for(int x=0; x<SIZEX; x++){
      for(int y=0; y<SIZEY; y++){
        for(int c=0; c<3; c++)
        {
            dst.at<Vec3d>(Point(x,y))[c]+=(double)(2/((double)storage._count))*storage.get_data(i,x,y,c);///(float)100;
        }
      }
    }
  }
  std::cout<<"done\n";
  imwrite( "./outf.jpg", dst );
*/
struct Super_image{
  Super_image(int X, int Y):
      _X(X),
      _Y(Y),
      _count(0)
      {}

  /*appends a image to the super image*/
  void append(Mat const& img){
    _count++;
    for(int x=0; x<_X; x++)
    {
      for(int y=0; y<_Y; y++)
      {
        _data.push_back(img.at<Vec3b>(Point(x,y))[0]);
        _data.push_back(img.at<Vec3b>(Point(x,y))[1]);
        _data.push_back(img.at<Vec3b>(Point(x,y))[2]);
      }
    }
  }
  double get_data(int frame, int x, int y, int c){
    int index=frame*(_X*_Y*3)+x*_Y*3+y*3+c;
    return _data[index];
  }
    //MEMBER:
    int _X;
    int _Y;
    int _count;
    std::vector<unsigned char> _data;
};

//////////////////////////////////////////////////
/*

//MAIN:
		srand(time(NULL));  //SEED
		int SIZEX=image.cols;
		int SIZEY=image.rows;

    Pixel pix;
		Vec3b color;
    std::string output_name="output.txt";
    RsfWriter writer(output_name);    //will be wrting our pixel

    //take samples:
    std::cout<<"now sampling..";
    const clock_t begin_time = clock();
		for (int i=0; i<samples_amount; i++)
		{
      pix.x= rand() % SIZEX;
		  pix.y= rand() % SIZEY;
			pix.color = image.at<Vec3b>(Point(pix.x,pix.y));
      //std::cout<<image.at<Vec3b>(Point(pix.x,pix.y))<<"\n";
      writer.add(pix);
		}
    std::cout<<"done\nnow storing..";
    writer.save();
    std::cout <<"done\ntook:"<<(float( clock () - begin_time )/  CLOCKS_PER_SEC)<<"seconds in total\n";

//this part shows how to read the fileformat!:
    RsfReader reader("output.txt");
    std::vector<Pixel> data_=reader.vec_with_pixels();
    cv::Mat pic_out(SIZEY, SIZEX, CV_8UC3, cv::Scalar(0,0,0));  //a black picture
    for ( std::vector<Pixel>::iterator p = data_.begin(); p != data_.end(); p++ ) {
      pic_out.at<Vec3b>(Point(p->x,p->y))=p->color;
    }
    imwrite( "./out.jpg", pic_out );
    return 0;
}
*/
//found: https://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/
