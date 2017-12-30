/*
wo solls hin:
editing tool pixel/bereich: belichtungszeit kurve/integral
->bilder dürfen keine verbrennungen vorher haben
->zu dunkle bereiche eines bildes kann man nicht korrekt zurück gewinnen/rechenfehler
//so gesehen, sind gute aufnahmen zwingend nötig, welcher nicht rgb ähnlich informationen speichert
//mit üblichen bildern, machen einfache videos sinn, solange diese nicht zu einem stückeligem
//resultat kommen
langzeitbelichutngsvideo/lifestream
input: Video
hoch und runterbelichten, pixelkurve, belichtungs breite(kurve),
output: video

info:

todo:
-wieso verkackt die gopro bei frame 240.. image empty
-anstelle von addweighted.. informationen in array speichern, weil eventuell informationen
verloren gehen.. rgb..
-> einfach rauf summieren und dann per mapp wieder in rgb raum quetschen.. eventuell  mit kurve log(0.0001*x)
  wenn faktor vor x zu groß, dann wired bild recht weißlich blass
*/
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

float faktor =0.000001;
using namespace cv;
void getdir (std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        //return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
}


static void onMouse(int event, int x, int y, int, void* imgptr){
  if ( event != 1 ) return;     // only draw on lmouse down
  Mat & img = (*((Mat*)imgptr)); // first cast, then deref
  Point pt1 = Point(x, y);
  circle(img, pt1, 1, Scalar(0, 255, 0), 100, 8, 0);
  imshow("Display window", img);
  waitKey(1);
    std::cout<<" now\n";
}

void usage(){
  std::cout<<"----------------------------------------------------------------\n";
  std::cout<<"usage: ./rand_sample_store ";
  std::cout<<"\"./picture_filepath\" ";
  std::cout<<"samples_amount ";
  //std::cout<<"\"./outputfilepath\"\n\n";
  std::cout<<"\n\n";

  std::cout<<"(required)  String:   ./picture_filepath\n";
  std::cout<<"(required)  int:      samples_amount\n\n";
  //std::cout<<"(optional)  String:   ./outputfilepath\n";
  std::cout<<"outpufile:  output.txt";
  std::cout<<"----------------------------------------------------------------\n";
}
struct Color{
  Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

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


int main(int argc, char** argv )
{
    //input: -v[ideo]/ -i[magefolder]
    std::string input_type= argv[1];
    std::string input = argv[2];  //inputfolder
    std::vector<std::string> files = std::vector<std::string>();

    //VIDEO:
    int SIZEX;
    int SIZEY;
    if("-v"== input_type)
    {
      VideoCapture capture;
      int totalFrameNumber;
      std::cout<<"video\n";
      capture=VideoCapture(argv[2]);
      totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);

      cv::Mat currentFrame;
      capture>>currentFrame;
      std::cout<<"depth"<<currentFrame.depth()<<"\n";

      SIZEX=currentFrame.cols;
      SIZEY=currentFrame.rows;
      std::cout<<SIZEX<<"\n";
      std::cout<<SIZEY<<"\n";
      cv::Mat dst=cv::Mat(SIZEY, SIZEX, CV_8UC3, cv::Scalar(0,0,0));  //a black picture
      clock_t begin_time = clock();
      std::vector<Color> clr;
      for(int x=0; x<SIZEX; x++)
      {
        for(int y=0; y<SIZEY; y++)
        {
          Color newclr(dst.at<Vec3b>(Point(x,y))[0],dst.at<Vec3b>(Point(x,y))[1],dst.at<Vec3b>(Point(x,y))[2]);
          clr.push_back(newclr);
        }
      }

      for (unsigned int i = 0;i <totalFrameNumber; i++) {
        capture>>currentFrame;
        if(!currentFrame.empty()){  //gopro fails during the first 500 frames
          //cv::addWeighted(dst,1.0,currentFrame,faktor,0,dst);
          //std::cout<<dst.at<Vec3b>(Point(150,100))<<"\n";
          //imshow("Display", dst);
          //waitKey(10);

          //float vec:
          int k=0;
          for(int x=0; x<SIZEX; x++)
          {
            for(int y=0; y<SIZEY; y++)
            {
              Color newclr( faktor*currentFrame.at<Vec3b>(Point(x,y))[0],
                            faktor*currentFrame.at<Vec3b>(Point(x,y))[1],
                            faktor*currentFrame.at<Vec3b>(Point(x,y))[2]);
              clr[k]+=newclr;
              k++;
            }
          }
        }else{
          std::cout<<"image empty\n";
        }

        if(!(i%500))  //just to prit progress
        {
          std::cout<<i<<" of "<<totalFrameNumber<<"\n";
        }
      }
      std::cout <<"calc done\ntook:"<<(float( clock () - begin_time )/  CLOCKS_PER_SEC)<<"seconds\n";

      imwrite( "./out.jpg", dst );

      float max=0;

      for ( std::vector<Color>::iterator p = clr.begin(); p != clr.end(); p++ ) {
        max=std::max(max,p->r);
        max=std::max(max,p->g);
        max=std::max(max,p->b);
      }
      std::cout<<"max: "<<max<<"\n";
      float scale=255/(log(1+max));
      int k=0;
      std::fstream file("out.txt", std::ios::out);

      for(int x=0; x<SIZEX; x++)
      {
        for(int y=0; y<SIZEY; y++)
        {
          file << clr[k].r << " ";
          file << clr[k].g << " ";
          file << clr[k].b << "\n";
          dst.at<Vec3b>(Point(x,y))[0]=scale*log(1+clr[k].r);
          dst.at<Vec3b>(Point(x,y))[1]=scale*log(1+clr[k].g);
          dst.at<Vec3b>(Point(x,y))[2]=scale*log(1+clr[k].b);
          k++;
        }
      }
        file.close();
        imwrite( "./outf.jpg", dst );
      //imshow("Display window", dst);
      //waitKey(0);
    }
}

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
