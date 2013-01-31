// HVA_2013_ImageProcessing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <opencv2/imgproc/imgproc.hpp> // Gaussian Blur
#include <opencv2/core/core.hpp>       // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp> // OpenCV window I/O

using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
   Mat image;
   string fileName = "C:\\Images\\retroreflective.png";

   // read the image
   image = imread(fileName, 1);

   // ensure the image was read
   if (!image.data)
      return(0);

   printf("Width: %d  Height: %d\n", image.cols, image.rows);

   // resize the image
   //resize(image, image, Size(320, 240));

   //Mat blurryImage;

   // blur the image to reduce noise
   //blur(image, blurryImage, Size(3, 3));

   imshow("Original Image", image);
  // imshow("Blurry Image", blurryImage);
  
   // convert RGB to HSV
   Mat HSVimage;

   // convert the image to HSV
   cvtColor(image, HSVimage, CV_BGR2HSV);

   // zero the mask image
   Mat mask = Mat::zeros(image.size(), CV_8UC1);

   // threshold HSV image for targets
   
   inRange(HSVimage, Scalar((int) (103 / 240.0f * 180 ),
                            (int) (100 / 240.0f * 255),
                            (int) (150 / 240.0f * 255)),
                     Scalar((int) (118 / 240.0f * 180),
                            (int) (240 / 240.0f * 255),
                            (int) (240 / 240.0f * 255)), mask);

   // create an image for the mask
   

	//int erosion_size = 1;

	//Mat element = getStructuringElement( MORPH_RECT,
	//	Size( 2 * erosion_size + 1, 2 * erosion_size + 1 ),
	//	Point(erosion_size, erosion_size));
	//
 //  // remove the small objects using erode
 //  erode(mask, mask, element);

	//imshow("Erosion Demo", mask );


   int                   foundRectangles = 0;
   vector<vector<Point>> contours;
   vector<Vec4i>         hierarchy;
   Mat                   contourImg;
   mask.copyTo(contourImg);

   // find contours in the mask
   findContours(contourImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

   // approximate contours to polygons + get bounding rects and circles
   vector<vector<Point>> contours_poly(contours.size());
   vector<Rect>          boundRect(contours.size());
   vector<Point2f>       center(contours.size());
   vector<float>         radius(contours.size());

   // get the rectangles from the polygon vectors
   for (int ploy = 0; ploy < contours.size(); ploy++)
   {
      approxPolyDP(Mat(contours[ploy]), contours_poly[ploy], arcLength(contours[ploy],true)*0.05, true);
	 //approxPolyDP(Mat(contours[ploy]), contours_poly[ploy], 10, true);
      boundRect[ploy] = boundingRect(Mat(contours_poly[ploy]));
      minEnclosingCircle(contours_poly[ploy], center[ploy], radius[ploy]);
   }

   // draw polygonal contour + bonding rects + circles
   Mat drawing;
   imshow("Mask Image", mask);
   cvtColor(mask, drawing, CV_GRAY2BGR);

   for (int contour = 0; contour < contours.size(); contour++)
   {
      // only accept rectangles greater than a minimum size
      if ((boundRect[contour].width  > 25) &&
          (boundRect[contour].height > 25))
      {

         // get the color of the pixel in the center of the rectangle
         Vec3b plane = image.at<Vec3b>(center[contour].y, center[contour].x);

		 printf("Contour: %d (%d, %d) %d %f\n", contour, (int)  center[contour].x, (int) center[contour].y, contours[contour].size(), arcLength(contours[contour],true)); 

		 int size = 4;
		 //cvPolyLine(drawing, &contours[contour],	
		 drawContours(drawing, contours, contour, Scalar(180,105,255),2);
         // indicate that a rectangle was found
         foundRectangles++;
      }
   }

   // Show in a window
   imshow("Contours", drawing);

   // return the contour array
  // return(foundRectangles);

//      vector<Vec4i>         hierarchy;
   // find the target locations
  // findContours(mask, mask, hierarchy, 


   
   
   cvWaitKey();

   return 0;
}

//void junk()
//{
   //// find contours in the mask
   //findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

   //// approximate contours to polygons + get bounding rects and circles
   //vector<vector<Point>> contours_poly(contours.size());
   //vector<Rect>          boundRect(contours.size());
   //vector<Point2f>       center(contours.size());
   //vector<float>         radius(contours.size());

   //// get the rectangles from the polygon vectors
   //for (int ploy = 0; ploy < contours.size(); ploy++)
   //{
   //   approxPolyDP(Mat(contours[ploy]), contours_poly[ploy], 3, true);
   //   boundRect[ploy] = boundingRect(Mat(contours_poly[ploy]));
   //   minEnclosingCircle(contours_poly[ploy], center[ploy], radius[ploy]);
   //}

