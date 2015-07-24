//
//  scene.cpp
//  raytracing
//
//  Created by yxfish13 on 15/6/6.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//


#include "common.h"
#include "string.h"
#include "scene.h"


    // -----------------------------------------------------------
    // Scene class implementation
    // -----------------------------------------------------------


Scene::~Scene()
{
 //   delete [] m_Primitive;
}
intersect_event Scene::find_intersect(Ray &A){
    intersect_event event;
    double dist;dist = 300000;
    for (int i = 0 ; i < GetNrPrimitives() ;i++)
    if((A.GetOrigin().z!=-400||!GetPrimitive(i)->IsLight())){
        intersect_event event1 = GetPrimitive(i)->Intersect(A, dist);
        if (event1.retval)
            event=event1;
        if (i==7&&event.retval){
            int tmp=0;
        }
    }
    return event;
}
double Scene::LightArea(const vector3&S,Primitive *obj,vector3 &LL){
    Sphere * obj1=(Sphere *)(obj);
    double times=0;
    Square * obj3=(Square *)(obj);
    if (obj->GetType()==1)
        LL = (((Sphere *)obj)->GetCentre()-S).Normal();
    else
        LL=(vector3(0.5*obj3->GetR().x,0,0.5*obj3->GetR().z)+obj3->GetPos()-S).Normal();
    /*vector3 P=obj1->GetCentre();
    P=(P-S).Normal();
    Ray ray(S+P*EPSILON,P);
    intersect_event A=find_intersect(ray);
    if (A.obj==0||A.obj->IsLight())
        return 1;
    else return 0;*/
    int cnt=0;
    bool ts[SamplesLight*SamplesLight1 +1][SamplesLight*SamplesLight1+1];
    for (int i=0;i<=SamplesLight1;i++)
        for (int j=0;j<=SamplesLight1;j++){
            cnt++;
            if(obj->GetType()==1){
                double x=Rand01()*0.8+0.1;
                double y=Rand01()*0.8+0.1;
                double z=Rand01()*0.8+0.1;
                while (x*x+y*y+z*z>1){
                    x=Rand01();
                    y=Rand01();
                    z=Rand01();
                }
                //x=y=z=0;
                if (rand()&1) x=-x;if (rand()&1) y=-y;if (rand()&1) z=-z;
                vector3 P=obj1->GetCentre()+vector3(x,y,z)*obj1->GetRadius();
                P=(P-S).Normal();
                Ray ray(S+P*EPSILON,P);
                intersect_event A=find_intersect(ray);
                if (A.obj==0||A.obj->IsLight()){
                    times++;
                    ts[i*SamplesLight][j*SamplesLight]=1;
                }else
                    ts[i*SamplesLight][j*SamplesLight]=0;
                if (times) return times/cnt;
            }
            if(obj->GetType()==3){
                double x=0.98/SamplesLight1*i+0.01;
                double z=0.98/SamplesLight1*j+0.01;
                vector3 P=vector3(x*obj3->GetR().x,0,z*obj3->GetR().z)+obj3->GetPos();
                P=(P-S).Normal();
                Ray ray(S+P*EPSILON*100,P);
                intersect_event A=find_intersect(ray);
                if (A.obj==0||A.obj->IsLight()){
                    times++;
                    ts[i*SamplesLight][j*SamplesLight]=1;
                }else
                    ts[i*SamplesLight][j*SamplesLight]=0;
            }
        }
    if(obj->GetType()==1)
        return times/cnt;
    times=0;cnt=0;
    for (int i=0;i<SamplesLight1;i++)
        for (int j=0;j<SamplesLight1;j++){
            cnt++;
            times+=ts[i*SamplesLight][j*SamplesLight];
            if (ts[i*SamplesLight][j*SamplesLight]!=ts[i*SamplesLight+SamplesLight][j*SamplesLight]||
                ts[i*SamplesLight][j*SamplesLight]!=ts[i*SamplesLight][j*SamplesLight+SamplesLight]){
                for (int xx=0;xx<SamplesLight;xx++)
                    for (int yy=0;yy<SamplesLight;yy++)
                        if(xx+yy){
                            double x=0.99*(1.0/SamplesLight1*i+1.0/SamplesLight1/SamplesLight*xx)+0.005;
                            double z=0.99*(1.0/SamplesLight1*j+1.0/SamplesLight1/SamplesLight*yy)+0.005;
                            vector3 P=vector3(x*obj3->GetR().x,0,z*obj3->GetR().z)+obj3->GetPos();
                            P=(P-S).Normal();
                            Ray ray(S+P*EPSILON*100,P);
                            intersect_event A=find_intersect(ray);
                            if (A.obj==0||A.obj->IsLight()){
                                ts[i*SamplesLight+xx][j*SamplesLight+yy]=1;
                            }else
                                ts[i*SamplesLight+xx][j*SamplesLight+yy]=0;
                        }
            }else
                for (int xx=0;xx<SamplesLight;xx++)
                    for (int yy=0;yy<SamplesLight;yy++)
                        ts[i*SamplesLight+xx][j*SamplesLight+yy]=ts[i*SamplesLight][j*SamplesLight];
            
        }
    for (int i=0;i<=SamplesLight*SamplesLight1;i++)
        for (int j=0;j<=SamplesLight*SamplesLight1;j++){
            cnt++;
            if (ts[i][j])
                times++;
        }
    return times/cnt;
}

void Scene::FindLight(){
    Light_P = new Primitive*[100];
    for (int i=0;i<m_Primitives;i++)
        if (m_Primitive[i]->IsLight()){
            Light_P[m_L]=m_Primitive[i];m_L++;
        }
}

void Scene::InitScene()
{
    m_Primitive = new Primitive*[500];
    // ground plane
    m_Primitive[0] = new PlanePrim( vector3( 0, 1, 0 ), 200 );
    m_Primitive[0]->GetMaterial()->SetReflection( 0 );
    m_Primitive[0]->GetMaterial()->SetRefraction( 0.0f );
    m_Primitive[0]->GetMaterial()->SetDiffuse( 1 );
    m_Primitive[0]->GetMaterial()->SetColor( Color( 0.5, 0.5f, 0.8f ) );
    m_Primitive[0]->SetText("T_e.jpg");
    
    //m_Primitive[1] = new Square(  vector3( 400, 699, 500 ), vector3( 50, 0.1f, 25 )  );
    m_Primitive[1] =new Sphere(vector3(600,799,1700),150);
    m_Primitive[1]->Light( true );
    m_Primitive[1]->GetMaterial()->SetColor( Color( 7, 7, 7 ) );
    m_Primitives =2;
    
    m_Primitive[2] = new PlanePrim( vector3( 0, 1, 0 ), -900 );
    m_Primitive[2]->GetMaterial()->SetReflection( 0 );
    m_Primitive[2]->GetMaterial()->SetRefraction( 0.0f );
    m_Primitive[2]->GetMaterial()->SetDiffuse( 1 );
    m_Primitive[2]->GetMaterial()->SetColor( Color( 1, 1, 1 ) );
    //m_Primitive[2]->SetText("T_e.jpg");
    
    m_Primitives =3;
    
    Material mat;
    mat.SetDiffuse( 1 );
    mat.SetColor(Color(1,1,1));
    mat.SetReflection(0);
    mat.SetRefraction(0);
    
    m_Primitive[m_Primitives++] = new Compobj("cube3.obj",1200,vector3(-200,-300,-600),mat,"T_f.jpg");
    mat.SetDiffuse( 1 );
    mat.SetColor(Color(1,1,1));
    mat.SetReflection(0);
    mat.SetRefraction(0);
    mat.SetRefrIndex(1);
    //mat.SetDiffuse( 0 );
    mat.Setabsorb(Color(0,0.0,0.0));
    m_Primitive[m_Primitives++] = new Compobj("plane.obj",1400,vector3(50,-200,600),mat);
    
    
    mat.SetDiffuse( 0 );
    mat.SetColor(Color(1,1,1));
    mat.SetReflection(0);
    mat.SetRefraction(1);
    mat.SetRefrIndex(1.7);
    mat.Setabsorb(Color(0.01,0.01,0.01));
    m_Primitive[m_Primitives++] = new Compobj("dragon.obj",350,vector3(400,100,200),mat);
    
    // build the regular gird
    FindLight();
}

