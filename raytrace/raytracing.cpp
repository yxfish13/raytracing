//
//  raytracing.cpp
//  raytracing
//
//  Created by yxfish13 on 15/6/6.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//

#include "raytracing.h"
#include "pthread.h"
#include <cmath>
#include <unistd.h>
Scene Raytrace:: m_scene;
int Raytrace:: width=0,Raytrace:: height=0;
Color** Raytrace:: board=NULL;
vector3 Raytrace:: o_view=vector3(0,0,0);
Raytrace::Raytrace(Scene &_scene,int w,int h){
    m_scene = _scene; width   =  w; height  =  h;
    board = new Color*[width*2+2];
    for (int i=0; i < width*2+2 ; i++ )
        board[i] = new Color[height*2+2];
}
Color Raytrace:: GetPhong(intersect_event &event,Ray &_ray,int depth,double ind,double &dist,int ttt,bool refra){
    Color pix(0,0,0);
    if(event.obj->GetMaterial()->GetDiffuse())
        for (int i=0;i<m_scene.GetNLight();i++){
            double area;
            Primitive   *A=m_scene.GetLight(i);
            vector3 LL;
            area=m_scene.LightArea(event.P,A,LL);
            if (area>EPSILON)
            {
                vector3 RR = -LL- event.N * 2.0 * DOT( -LL, event.N );
                double dot1=DOT(event.N,LL)*event.obj->GetMaterial()->GetDiffuse();
                double dot2=pow(DOT(RR,-_ray.GetDirection()),15)*event.obj->GetMaterial()->GetSpecular();
                dot1=dot1*event.retval;
                dot2=dot2*event.retval;
                if (dot1>0)
                    pix+=event.obj->GetColor(event.P.x,event.P.y,event.P.z)*A->GetColor()*
                    dot1*area;
                 if (dot2>0)
                 pix=pix+ A->GetColor()*dot2*area;
            }
        }
    return pix;
}
Color Raytrace:: GetMT(intersect_event &event,Ray &_ray,int depth,double ind,double &dist,int ttt,bool refra){
    Color pix(0,0,0);
    return pix;
}

Color Raytrace::Raytracing(Ray &_ray,int depth,double ind,double &dist,int ttt,bool refra){
    if (depth>MaxDepth) return Color(0,0,0);
    Color pix(0,0,0);
    intersect_event event=m_scene.find_intersect(_ray);
    dist=0;
    double t_dist=0;
    if (event.retval){
       
        if (event.obj->IsLight())
            return event.obj->GetColor();
        if (refra)  event.retval=-1;
        //Phong
        double tempt;
        //
     //  Color pix_ttt=GetPhong(event,_ray,depth,ind,tempt,ttt,refra);
                Color pix_R=Color(0,0,0);
        
                int Samples=1;
                if (ttt==0) Samples=mt_times;
        
        
         
        
        if(event.obj->GetMaterial()->GetDiffuse()>0.01)
            for (int i=0;i<m_scene.GetNLight();i++){
                double area;
                Primitive   *A=m_scene.GetLight(i);
                vector3 LL;
                area=m_scene.LightArea(event.P,A,LL);
                if (area>EPSILON)
                {
                    vector3 RR = -LL- event.N * 2.0 * DOT( -LL, event.N );
                    double dot1=DOT(event.N,LL)*event.obj->GetMaterial()->GetDiffuse();
                   // double dot2=pow(DOT(RR,-_ray.GetDirection()),15)*event.obj->GetMaterial()->GetDiffuse();
                    dot1=dot1*event.retval;
                    //dot2=dot2*event.retval;
                    Sphere *AA=(Sphere*)A;
                    vector3 Len=event.P-AA->GetCentre();
                    double cos2a=((AA)->GetSqRadius()/Len.SqrLength());
                    cos2a=sqrt(1-cos2a);
                    if (dot1>0)
                        pix+=event.obj->GetColor(event.P.x,event.P.y,event.P.z)*A->GetColor()*
                        dot1*area*2*(1.0-cos2a);
                   // if (dot2>0)
                     //   pix=pix+ A->GetColor()*dot2*area;
                }
            }
        if (Samples){
            int ts=Samples;
            if(event.obj->GetMaterial()->GetDiffuse())
                for (int ts=0;ts<Samples;ts++){
                    vector3 LL=event.N;
                    vector3 X = vector3(1,1,1);
                    for (int i=0;i<3;i++)
                        if (LL.cell[i]!=0)
                            X.cell[i]=-(LL.cell[(i+1)%3]*X.cell[(i+1)%3]+LL.cell[(i+2)%3]*X.cell[(i+2)%3])/LL.cell[i];
                    X.Normalize();
                    vector3 Y=X.Cross(LL);Y.Normalize();
                    double theta=acos(sqrt(Rand01())),beta=Rand01()*2*3.1415926;
                    LL=LL*cos(theta)+X*sin(theta)*cos(beta)+Y*sin(theta)*sin(beta);
                    Ray Diffuse_ray(event.P+LL*EPSILON*10,LL);
                    //double t_dist;
                    Color pix_t;
                    vector3 RR = -LL- event.N * 2.0 * DOT( -LL, event.N );
                    double dot2 = 0;//pow(RR.Dot(-_ray.GetDirection()),30);
                    if (dot2<0)dot2=0;
                    pix_t=Raytracing(Diffuse_ray, depth+step
                                     ,ind,t_dist,1,refra)*event.obj->GetMaterial()->GetDiffuse()*(DOT(LL,event.N)+dot2);
                    pix_R+=pix_t;
                    dist+=t_dist;
                }
            if (ts==0) ts=1;
            pix_R=pix_R*event.obj->GetColor(event.P.x,event.P.y,event.P.z)*(1.0/ts);
            pix+=pix_R*2;
            dist/=ts;
            dist+=event.dist;
        }
        //vector3 pixtemp=GetPhong(event,_ray,depth,ind,tempt,ttt,refra);
        //if (pixtemp.Length()>pix.Length()) pix=pixtemp;
        pix=pix+ event.obj->GetColor(event.P.x,event.P.y,event.P.z)*s_light*event.obj->GetMaterial()->GetDiffuse();
        //if (ttt)pix=pix*(1.0/event.obj->GetColor(event.P.x,event.P.y,event.P.z).Maxis());
        //if (pix_ttt.Length()>pix.Length()) pix=pix_ttt;
        
        
        
        double reflection =event.obj->GetMaterial()->GetReflection();
        if (event.obj->GetMaterial()->GetRefraction()>EPSILON){
            vector3 T;double cosT,cosi;
            double outind=event.obj->GetMaterial()->GetRefrIndex();
            if (event.retval==-1) outind=1;
            double _ind=ind/outind;
            cosi=-DOT(event.N, _ray.GetDirection());
            cosT=(1-_ind*_ind*(1-cosi*cosi));
            if (cosT>=-EPSILON/1000){
                cosT=sqrt(cosT);
                T = _ind*_ray.GetDirection();
                T =T -(_ind*_ray.GetDirection().Dot(event.N)+cosT)*event.N;
                Ray RfRay(event.P+T*EPSILON*10,T);
                Color Rix_RF=Raytracing(RfRay, depth+1, outind, t_dist,ttt,not refra);
                if (refra) dist=event.dist;//else t_dist=event.dist;
                vector3 trans=event.obj->GetAbsorb();
                trans.x=exp(t_dist*-0.2*trans.x);
                trans.y=exp(t_dist*-0.2*trans.y);
                trans.z=exp(t_dist*-0.2*trans.z);
                if (event.retval==1)pix+=Rix_RF*event.obj->GetMaterial()->GetRefraction()*trans;
                else
                    pix+=Rix_RF;
                //dist=t_dist+event.dist;
            }else{
                //depth--;
                Color pix_R=Color(0,0,0);
                vector3 R = _ray.GetDirection()- event.N * 2.0 * DOT( _ray.GetDirection(), event.N );
                Ray ReflectRay(event.P + R*EPSILON*10,R);
                //Color pix_R;
                pix_R=Raytracing(ReflectRay, depth+1,ind,t_dist,ttt,refra);
                pix+=pix_R;
                //if (refra) dist=t_dist+event.dist; else dist=0;
            }
         }
        
        //reflection
        pix_R=Color(0,0,0);
        vector3 R = _ray.GetDirection()- event.N * 2.0 * DOT( _ray.GetDirection(), event.N );
        Ray ReflectRay(event.P + R*EPSILON*10,R);
        //Color pix_R;
        if (event.retval==1&&reflection){
            pix_R=Raytracing(ReflectRay, depth+1,ind,t_dist,ttt,refra)*event.obj->GetColor(event.P.x,event.P.y,event.P.z)*reflection;
            dist=t_dist+event.dist;
        }
        pix+=pix_R;
        //if (refra) dist=t_dist+event.dist; else dist=0;
    }
   // if (pix.x>1) pix.x=1;
   // if (pix.y>1) pix.y=1;
   // if (pix.z>1) pix.z=1;
    return pix;
}

void* Raytrace::RayT(void*arg){
    int f=f_dis,R=f_light,multi=f_times;
    int y=(*((int*)arg));
    for (int x1=0;x1<samplelen;x1++)
        for (int y1=0;y1<samplelen;y1++)
            for(int x=0;x<width;x++){
                vector3 a(1.0/samplelen*x1+x,1.0/samplelen*y1+y,-200);
                //a+=vector3(Rand01()/samplelen,Rand01()/samplelen,0);
                a=a-o_view;
                a.Normalize();
                vector3 af=o_view+((f-o_view.z)/a.z)*a;
                double dist;
                for (int sp=0;sp<multi;sp++){
                    vector3 o_v=o_view;
                    double theta=drand48()*2*3.14159;
                    double ls=drand48();
                    o_v.x+=R*cos(theta)*ls;
                    o_v.y+=R*sin(theta)*ls;
                    Ray _ray;
                    a=af-o_v;
                    a.Normalize();
                    _ray.set_origin(o_v);
                    _ray.set_direction(a);
                    Color temp;
                    temp=Raytracing(_ray,0,1,dist,0,0);
                    if (temp.x>1) temp.x=1;
                    if (temp.y>1) temp.y=1;
                    if (temp.z>1) temp.z=1;
                    board[x][y]+=temp*(1.0/multi/samplelen/samplelen);
                }
                if (board[x][y].x>1) board[x][y].x=1;
                if (board[x][y].y>1) board[x][y].y=1;
                if (board[x][y].z>1) board[x][y].z=1;
            }
            //cvShowImage(windowTitle, image);
    //cvSaveImage("a.bmp", image);
    return (void*) 8;
}
void Raytrace:: draw(int y,IplImage* image,const char *windowTitle){
    for (int x=0;x<width;x++){
        CvScalar pixel = cvGet2D(image, y, x);
        pixel.val[0]=board[x][y].x*255;
        pixel.val[1]=board[x][y].y*255;
        pixel.val[2]=board[x][y].z*255;
        cvSet2D(image,height-y-1,x,pixel);
    }
    cvSaveImage("a.bmp", image);
}
void Raytrace:: render(IplImage* image,const char *windowTitle){
    int temp=0;
    int multi=maxthread;
    pthread_t tidp[multi];
    int *b[multi];
    for (int i=0;i<multi;i++)b[i]=new int;
    for (int j=0;j<multi;j++){
        *b[j]=j+temp;
        pthread_create(&tidp[j],NULL,RayT,(void*)b[j]);
    }
    int now=temp;
    while (now<height){
        pthread_join(tidp[now%multi],NULL);
        if (now+multi<height){
            *b[now%multi]=now+multi;
            pthread_create(&tidp[now%multi],NULL,RayT,(void*)b[now%multi]);
        }
        draw(now,image,windowTitle);
       if (now%5==0) cvShowImage(windowTitle, image);
       // cvSaveImage(fileName, image);
        cvWaitKey(1);
       // sleep(0.5);
        now=now+1;
        std::cout<<now<<std::endl;
    }
    cvShowImage(windowTitle, image);
    cvSaveImage("a.bmp", image);
}
