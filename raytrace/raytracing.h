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
const int MaxDepth = 7;
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
private:
    static Scene m_scene;
    static int width,height;
    static Color **board;
    static vector3 o_view;
};

#endif
