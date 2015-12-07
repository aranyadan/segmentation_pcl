#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray,tracks;

int thresh = 120;
int max_thresh = 255;

char* source_window = "Source image";
char* corners_window = "Corners detected";
char* tracked = "tracks";

/// Function header
void cornerHarris_demo( int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  VideoCapture v(argv[1]);
  namedWindow( source_window, CV_WINDOW_NORMAL);
  namedWindow( corners_window, CV_WINDOW_NORMAL );
  namedWindow( tracked, CV_WINDOW_NORMAL );
  createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
  int c=0;
  while(1)
  {
    v>>src;
    if(c==0)
    {
      tracks=Mat(src.rows, src.cols,CV_8UC1, Scalar(255));
      c++;
    }
    cvtColor( src, src_gray, CV_BGR2GRAY );


    imshow( source_window, src );

    cornerHarris_demo( 0, 0 );

    char a=waitKey(100);
    if(a==27)
      break;
  }
  return(0);
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

  /// Detecting corners
  cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners
  int count=0;
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
               circle( tracks, Point( i, j ), 1,  Scalar(0), 1, 8, 0 );
               count ++;
              }
          }
     }
  /// Showing the result
  if(count==1)
  {
    cout<<"0\n";
    tracks=Mat(src.rows,src.cols, CV_8UC1, Scalar(255));
  }

  imshow( corners_window, dst_norm_scaled );
  imshow( tracked, tracks );
}
