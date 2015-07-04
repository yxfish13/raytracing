//
//  raytracing.h
//  raytracing
//
//  Created by yxfish13 on 15/6/6.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//

#ifndef raytracing_raytracing_h
#define raytracing_raytracing_h
#include "common.h"
#include "scene.h"
#include <opencv2/opencv.hpp>
#include <libiomp/omp.h>
/*#include <iostream>
#include <cmath>*/
#include "object.h"

const int    MaxDepth    = 9;//追踪层数
const bool   supersample = false;
const int    samplelen   = 5;
const double samplediff  = 0.2;
const double f_light     = 15;//光圈
const double f_dis       = 50;//焦距
const int    f_times     = 40;//光圈的采样次数
const int    maxthread   = 6;//多线程树
const int    mt_times    = 25;//mt的采样次数
const int    step        = 3;
class Raytrace{
public:
    Raytrace(){}
    Raytrace(Scene &_scene,int w,int h);
    static Color Raytracing(Ray &A,int depth,double ind,double &dist,int ttt,bool refra);
    Color** GetPhoto(){return board;}
    void SetView(const vector3 &o){o_view=o;}
    void render( IplImage* image,const char * windowTitle);
    static void *RayT(void*arg);
    void draw(int y,IplImage* image,const char *windowTitle);
    static Color GetPhong(intersect_event &event,Ray &_ray,int depth,double ind,double &dist,int ttt,bool refra);
    static Color GetMT(intersect_event &event,Ray &_ray,int depth,double ind,double &dist,int ttt,bool refra);
private:
    static Scene m_scene;
    static int width,height;
    static Color **board;
    static vector3 o_view;
};

#endif
