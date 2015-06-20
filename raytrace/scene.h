//
//  scene.h
//  raytracing
//
//  Created by yxfish13 on 15/6/6.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//

#ifndef raytracing_scene_h
#define raytracing_scene_h

#include "common.h"
#include "object.h"


// -----------------------------------------------------------
// Scene class definition
// -----------------------------------------------------------
const int SamplesLight=4;
const int SamplesLight1=1;
class Scene
{
public:
    Scene() : m_Primitives( 0 ), m_Primitive( 0 ),m_L( 0 ), Light_P( 0 ) {};
    ~Scene();
    void InitScene();
    void FindLight();
    Primitive* GetLight(int idx){return Light_P[idx];}
    int GetNLight(){return m_L;}
    int GetNrPrimitives() { return m_Primitives; }
    Primitive* GetPrimitive( int a_Idx ) { return m_Primitive[a_Idx]; }
    intersect_event find_intersect(Ray &A);
    void AddSphere(Primitive *A);
    void AddPlane(Primitive *A);
    double LightArea(const vector3&ray, Primitive *obj,vector3 &LL);
private:
    int m_Primitives,m_L;
    Primitive** m_Primitive;
    Primitive** Light_P;
};
#endif
