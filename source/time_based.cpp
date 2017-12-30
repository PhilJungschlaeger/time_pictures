/*
info:

todo:

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

int main(int argc, char** argv )
{
    std::string dir = argv[1];  //inputfolder
    std::vector<std::string> files = std::vector<std::string>();
    std::vector<cv::Mat> pictures = std::vector<cv::Mat>();
    getdir(dir,files);
    clock_t begin_time = clock();
    cv::Mat image;
    for (unsigned int i = 0;i < 5/*files.size()*/;i++) {
        image = cv::imread(dir+"/"+files[i], 1 );
        if ( image.data )
        {
            std::cout << dir+files[i] << std::endl;
            pictures.push_back(image);
        }
    }
    int SIZEX=pictures[0].cols;
    int SIZEY=pictures[0].rows;
    std::cout <<"loading done\ntook:"<<(float( clock () - begin_time )/  CLOCKS_PER_SEC)<<"seconds\n";

    begin_time = clock();
    cv::Mat dst=cv::Mat(SIZEY, SIZEX, CV_8UC3, cv::Scalar(0,0,0));  //a black picture

    for (unsigned int i = 0;i < pictures.size(); i++) {
    	cv::addWeighted(dst,1.0,pictures[i],0.8,0,dst);
    }

    /*
    for (int x=0; x<SIZEX; x++) {
      for (int y=0; y<SIZEY; y++){
        for (unsigned int i = 1;i < pictures.size();i++) {
        	dst.at<cv::Vec3b>(cv::Point(x,y))+=pictures[i].at<cv::Vec3b>(cv::Point(x,y))*0.8;
        }
      }
    }
    */

    std::cout <<"calc done\ntook:"<<(float( clock () - begin_time )/  CLOCKS_PER_SEC)<<"seconds l\n";

    namedWindow("Display window",0);
    setMouseCallback("Display window", onMouse, &dst);

    imshow("Display window", dst);

    waitKey(0);
    //cv::imwrite("./fout.JPG", dst);

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
