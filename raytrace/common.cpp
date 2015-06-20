//
//  common.cpp
//  raytrace
//
//  Created by yxfish13 on 15/6/9.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//

#include "common.h"
double pow(double x,int y)
{
    if (y==0) return 1;
    //if(y==1)return x;
    double tmp=pow(x,y/2);
    tmp=tmp*tmp;
    if (y&1) tmp=tmp*x;
        return tmp;
}
long long temp=1;
int maxp1 = 113;
int maxp2 = 1000000009;
double Rand01(){
    //return 0.5;
   // temp=temp*maxp1%maxp2;
    return 0.99999*(rand()%RAND_MAX)/RAND_MAX;
}