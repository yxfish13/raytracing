


#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

#include "scene.h"
#include "raytracing.h"
#include "object.h"
#include "ctime"
using namespace std;

const int IMAGE_WIDTH = 800;
const int IMAGE_HEIGHT = 600;
const char* windowTitle = "rantracing";
const char* fileName = "picture.bmp";
const double pi = acos(-1);
IplImage* image = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
//CvScalar pixel = cvGet2D(image, x1, y1);
//pixel.val[0]=ans*255/16*x/IMAGE_WIDTH;
//pixel.val[1]=ans*255/16*x/IMAGE_HEIGHT;
//pixel.val[2]=ans*255/16;
//cvSet2D(image,y2,x2,pixel);
//cvShowImage(windowTitle, image);
int main() {
    cvSet(image, CV_RGB(0, 0, 0));
    cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
    cvShowImage(windowTitle, image);
    Scene _scene;_scene.InitScene();
    Raytrace m_raytrace(_scene, IMAGE_WIDTH , IMAGE_HEIGHT);
    m_raytrace.SetView(vector3(320,240,-700));
    int t=clock();
    m_raytrace.render(image,windowTitle);
    cout<<(clock()-t)/CLOCKS_PER_SEC<<endl;
    cvShowImage(windowTitle, image);
    cvSaveImage(fileName, image);
    cvWaitKey(0);
    cvDestroyWindow(windowTitle);
    cvReleaseImage(&image);
    return 0;
 }


