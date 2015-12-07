#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray,prev;

int thresh = 180;
int max_thresh = 255;
//std::vector<Point> Points;
std::vector<std::vector<int> > Points;
char* source_window = "Source image";
char* corners_window = "Corners detected";

/// Function header
void cornerHarris_demo( int, void* );

void Matcher();
/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  VideoCapture v(argv[1]);
  namedWindow( source_window, CV_WINDOW_NORMAL);
  namedWindow( corners_window, CV_WINDOW_NORMAL );
  createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
  int c=0;
  while(1)
  {
    prev=src;
    v>>src;
    cvtColor( src, src_gray, CV_BGR2GRAY );


    imshow( source_window, src );
    Matcher();
    cornerHarris_demo( 0, 0 );

    char a=waitKey(1000);
    if(a==27)
      break;
    /*for(int i=0;i<Points.size();i++)
    {
      cout<<"no of points:"<<Points.size()<<" r:"<<Points[i][0]<<"  c:"<<Points[i][1]<<"\n";
    }
    cout<<"new:\n";
    Points.resize(0);*/

  }
  return(0);
}

void Matcher()
{
  cv::Point current;
  cv::Mat temp_template;
  cv::Mat temp_base_image,result;
  for(int i=0;i<Points.size();i++)
  {
    current.x=Points[i][1];
    current.y=Points[i][0];
    //creating the template to be checked in subsequent frames
    if(current.x>prev.cols -9 || current.x < 10 || current.y > prev.rows -9 || current.y < 10)
      continue;
    temp_template=prev(cv::Rect(current.x - 10, current.y - 10, 20, 20));
    //creating the base image the template will be correlated with
    if(current.x<= int(0.075 * src.cols))
      current.x = int(0.075 * src.cols) +1;
    else if(current.x >=int(0.925 * src.cols))
          current.x=int(0.925 * src.cols) -1;
    if(current.y<= int(0.075 * src.rows))
      current.y = int(0.075 * src.rows) +1;
    else if(current.y >=int(0.925 * src.rows))
          current.y=int(0.925 * src.rows) -1;
    temp_base_image=src(cv::Rect(int(current.x - float( 0.075 * src.cols)), int(current.y -float( 0.075 * src.rows)), int(0.15*src.cols), int(0.15*src.rows)));
    //template matching with correlation coefficient
       //creating place to store result
    int result_cols =  temp_base_image.cols - temp_template.cols + 1;
    int result_rows = temp_base_image.rows - temp_template.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1 );
    matchTemplate( temp_base_image, temp_template, result, CV_TM_CCOEFF);
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    if(maxVal>0.97)
    {

    }

    namedWindow( "template", CV_WINDOW_NORMAL);
    namedWindow( "templatebaseimage", CV_WINDOW_NORMAL);
    imshow("template", temp_template);
    imshow("templatebaseimage", temp_base_image);
    int a = waitKey(0);
    if(a==8)
      continue;
  }
}
/** @function cornerHarris_demo */
void cornerHarris_demo( int, void* )
{

  Mat dst, dst_norm, dst_norm_scaled;
  dst = Mat::zeros( src.size(), CV_32FC1 );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;
  std::vector<int> temp;
  /// Detecting corners
  cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners
  int count=0;
  for( int i = 0; i < dst_norm.rows ; i++ )
     { for( int j = 0; j < dst_norm.cols; j++ )
          {
            if( (int) dst_norm.at<float>(i,j) > thresh )
              {
               circle( dst_norm_scaled, Point( j, i ), 5,  Scalar(0), 2, 8, 0 );
               temp.push_back(i);
               temp.push_back(j);
               temp.push_back(0);
               Points.push_back(temp);
               temp.resize(0);
              }
          }
     }
  /// Showing the result
  imshow( corners_window, dst_norm_scaled);
}
