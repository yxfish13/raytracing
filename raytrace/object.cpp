

//
//  object.cpp
//  raytracing
//
//  Created by yxfish13 on 15/6/6.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//

#include "object.h"
#include "string.h"
//#define Debug

// -----------------------------------------------------------
// Primitive class implementation
// -----------------------------------------------------------

void Primitive::SetName( char* a_Name )
{
    delete m_Name;
    m_Name = new char[strlen( a_Name ) + 1];
    strcpy( m_Name, a_Name );
}

// -----------------------------------------------------------
// Material class implementation
// -----------------------------------------------------------

Material::Material() :
m_Color( Color( 0.2f, 0.2f, 0.2f ) ),
m_Refl( 0 ), m_Diff( 0.2f ),m_RIndex(1),m_Spec(0.3),m_Refr(0)
{
}

// -----------------------------------------------------------
// Sphere primitive methods
// -----------------------------------------------------------

intersect_event Sphere::Intersect( Ray& a_Ray, double& a_Dist )
{
    vector3 v = a_Ray.GetOrigin() - m_Centre;
    double b = -DOT( v, a_Ray.GetDirection() );
    double det = (b * b) - DOT( v, v ) + m_SqRadius;
    int retval = MISS;
    if (det > 0)
    {
        det = sqrtf( det );
        double i1 = b - det;
        double i2 = b + det;
        if (i2 > 0)
        {
            if (i1 < 0)
            {
                if (i2 < a_Dist&&i2>EPSILON)
                {
                    a_Dist = i2;
                    retval = INPRIM;
                }
            }
            else
            {
                if (i1 < a_Dist&&i1>EPSILON)
                {
                    a_Dist = i1;
                    retval = HIT;
                }
            }
        }
    }
    intersect_event inter;
    inter.retval = retval;
    if (retval!=MISS){
        inter.dist=a_Dist;
        inter.obj = this;
        inter.P = a_Ray.GetDirection();
        NORMALIZE(inter.P);
        inter.P*=a_Dist;
        inter.P +=a_Ray.GetOrigin();
        inter.N = inter.P-m_Centre;
        NORMALIZE( inter.N );
        if (inter.N.Dot(a_Ray.GetDirection())>0)
            inter.N=-inter.N;
    }
    return inter;
}

Color Sphere::GetColor(double x,double y  ,double z) {
    
    if (x<-3000||!istexture||y<0)return m_Material.GetColor();
    vector3 temp=vector3(x,y,z)-m_Centre;
    double ty=acos(temp.Dot(vector3(0,1,0))/m_Radius)/3.1415926,tx=x;
    temp.y=0;double l=temp.Length();
    tx=acos(temp.x/l)/2/3.1415926;
    if (z<0) tx=1-tx;
    y=ty*image->height;
    x=tx*image->width;
    CvScalar pixel = cvGet2D(image, y, x);
    return vector3(1.0*pixel.val[0]/255,1.0*pixel.val[1]/255,1.0*pixel.val[2]/255);
}

// -----------------------------------------------------------
// Plane primitive class implementation
// -----------------------------------------------------------

intersect_event PlanePrim::Intersect( Ray& a_Ray, double& a_Dist )
{
    intersect_event inter;
    inter.retval=MISS;
    inter.obj=this;
    double d = DOT( m_Plane.N, a_Ray.GetDirection() );
    if (d != 0)
    {
        double dist = -(DOT( m_Plane.N, a_Ray.GetOrigin() ) + m_Plane.D) / d;
        if (dist > 0)
        {
            if (dist < a_Dist+EPSILON&&dist>EPSILON)
            {
                a_Dist = dist;
                inter.retval= HIT;
                inter.dist=a_Dist;
                inter.P = a_Ray.GetDirection();
                NORMALIZE(inter.P);
                inter.P*=a_Dist;
                inter.P+=a_Ray.GetOrigin();
                inter.N = m_Plane.N;
                inter.N.Normalize();
            }
        }
    }
    if (inter.N.Dot(a_Ray.GetDirection())>0)
        inter.N=-inter.N;
    return inter;
}

vector3 PlanePrim::GetNormal( vector3& a_Pos )
{
    return m_Plane.N;
}
Color PlanePrim:: GetColor(double x,double y ,double z)
{
    if (istexture){
        int tmp=0;
    }
    if (m_Plane.N.z!=0)
    { x+=3303;y+=3330;}
    else
        if (m_Plane.N.x!=0){
            x=z+3330,y+=3330;
        }
        else
            x=x+3330,y=z+3330;
    x+=1E5+300;y+=1E5+300;
    if (x<00000||!istexture||y<0)return m_Material.GetColor();
    y=int(y)%(image->height-80)+40;
    x=int(x)%(image->width-80)+40;
    CvScalar pixel = cvGet2D(image, y, x);
    return vector3(1.0*pixel.val[0]/255,1.0*pixel.val[1]/255,1.0*pixel.val[2]/255);
    //return vector3(1.0/224*(((int)(x*y))%230),1.0/224*(((int)(x*y))%230),1.0/224*(((int)(x*y))%230));
    if ((int(x/30)+int(y/30))%2) return vector3(1,1,1);else return vector3(0,0,0);
}
//Color triangle:: GetColor(double x,double y  ,double z) {
//    
//    if (x<-3000||!istexture||y<0)return m_Material.GetColor();
//    int tx=vector3(x,y,z).Dot(e1),ty=vector3(x,y,z).Dot(e2);
//    y=(ty%image->height+3000)%image->height;
//    x=(tx%image->width+3000)%image->width;
//    CvScalar pixel = cvGet2D(image, y, x);
//    return vector3(1.0*pixel.val[0]/255,1.0*pixel.val[1]/255,1.0*pixel.val[2]/255);
//    if ((int(x/30)+int(y/30))%2) return vector3(1,1,1);else return vector3(0,0,0);
//}

intersect_event triangle::Intersect(Ray& a_ray, double &a_dist){
    intersect_event inter;
    inter.retval=MISS;
    inter.obj=this;
    vector3 P=a_ray.GetDirection().Cross(e2);
    double det=DOT(e1,P);
    if (det>-EPSILON&&det<EPSILON) return inter;
    double inv_det=1/det;
    vector3 T=a_ray.GetOrigin()-Pa;
    double u=DOT(T,P)*inv_det;
    if (u<-EPSILON||u>1+EPSILON) return inter;
    vector3 Q=T.Cross(e1);
    double v=DOT(a_ray.GetDirection(),Q)*inv_det;
    if (v<-EPSILON||u+v>1+EPSILON) return inter;
    double t=DOT(e2,Q)*inv_det;
    if (t>EPSILON&&t<a_dist-EPSILON){
        inter.N=N;
        inter.dist=t;
        inter.P=a_ray.GetOrigin()+(a_ray.GetDirection()*t);
        inter.retval=1;
        a_dist=t;
        if(DOT(a_ray.GetDirection(), N)>0) inter.N=-N;
    }
    return inter;
}

intersect_event Square::Intersect( Ray& a_Ray, double& a_Dist ){
    intersect_event A;
    A.retval=0;
    vector3 start = a_Ray.GetOrigin();
    vector3 direc = a_Ray.GetDirection();
    if ((pos-start).y/direc.y<-EPSILON) return A;
    double a_dist=(pos-start).y/direc.y*direc.Length();
    if (a_dist>a_Dist) return A;
    vector3 t=start+(pos-start).y/direc.y*direc-pos;
    if (t.x>-EPSILON&&t.z>-EPSILON&&t.x<R.x+EPSILON&&t.z<R.z+EPSILON){
        a_Dist=a_dist;
        A.retval=1;
        A.obj=this;
        A.N = N;
        A.P = t+pos;
        A.dist=a_dist;
        if (A.N.Dot(a_Ray.GetDirection())>0)
            A.N=-A.N;
    }
    return A;
}
KdNode * Compobj::BuildKdTree(const vector3 &mn,const vector3 &mx,triangle**tria,int cnt,int axis)
{
    KdNode *root=new KdNode;
    if (cnt<30) {
        root->isleaf=true;
        if (cnt>0){
            root->tr= new triangle*[cnt];
            for (int i=0;i<cnt;i++)
                root->tr[i]=tria[i];
        }
        root->cnt=cnt;
        return root;
    }
    int cnt1=0,cnt2=0;
    triangle **Rt=new triangle*[cnt];
    double mid = (mx.cell[axis]+mn.cell[axis])/2;
    for (int i=0;i<cnt;i++)
    {
        int ts=tria[i]->cross(axis, mid);
        if (ts!=1) tria[cnt1++]=tria[i];
        if (ts!=-1) Rt[cnt2++]=tria[i];
    }
    vector3 m=mx;//axis=(axis+1)%3;
    m.cell[axis]=mid;
    root->left=BuildKdTree(mn,m, tria, cnt1, (axis+1)%3);
    m=mn;m.cell[axis]=mid;
    root->right=BuildKdTree(m,mx, Rt, cnt2, (axis+1)%3);
    return root;
}


Compobj::Compobj(char *fileName,double scale,vector3 trans,Material &mat)
{
    std::cout<<1<<std::endl;
    FILE * fin =fopen(fileName,"r");
     std::cout<<2<<std::endl;
    fscanf(fin,"# %d%d\n",&np,&nf);
    P = new vector3[np+1];
    F = new int[3*nf+3];
    mn=vector3(1E6,1E6,1E6);
    mx=vector3(-1E6,-1E6,-1E6);
    for (int i=1;i<=np;i++){
        double xx,zz;
        double theta=0;
        fscanf(fin,"v %lf%lf%lf\n",&xx,&P[i].y,&zz);
        P[i].x=xx;//=cos(theta)*xx-sin(theta)*zz;
        P[i].z=zz;//=cos(theta)*zz+sin(theta)*xx;
        P[i]*=scale;
        P[i]+=trans;
        if (mn.cell[0]>P[i].cell[0]) mn.cell[0]=P[i].cell[0];
        if (mn.cell[1]>P[i].cell[1]) mn.cell[1]=P[i].cell[1];
        if (mn.cell[2]>P[i].cell[2]) mn.cell[2]=P[i].cell[2];
        if (mx.cell[0]<P[i].cell[0]) mx.cell[0]=P[i].cell[0];
        if (mx.cell[1]<P[i].cell[1]) mx.cell[1]=P[i].cell[1];
        if (mx.cell[2]<P[i].cell[2]) mx.cell[2]=P[i].cell[2];
    }
    trig = new triangle*[nf+2];
   // IplImage *image = cvLoadImage("T_f.jpg");
    for (int i=0;i<nf;i++){
        fscanf(fin,"f %d%d%d\n",&F[i*3+0],&F[i*3+1],&F[i*3+2]);
        trig[i]=new triangle(P[F[i*3+0]],P[F[i*3+1]],P[F[i*3+2]]);
        trig[i]->SetMaterial(mat);
      //  trig[i]->SetText(image);
    }
    #ifndef Debug
    root=BuildKdTree(mn,mx,trig,nf,0);
    #endif
}
Compobj::Compobj(char *fileName,double scale,vector3 trans,Material &mat,char *Text)
{
    std::cout<<1<<std::endl;
    FILE * fin =fopen(fileName,"r");
    std::cout<<2<<std::endl;
    fscanf(fin,"# %d%d\n",&np,&nf);
    P = new vector3[np+1];
    F = new int[3*nf+3];
    mn=vector3(1E6,1E6,1E6);
    mx=vector3(-1E6,-1E6,-1E6);
    for (int i=1;i<=np;i++){
        double xx,zz;
        double theta=0;
        fscanf(fin,"v %lf%lf%lf\n",&xx,&P[i].y,&zz);
        P[i].x=xx;//=cos(theta)*xx-sin(theta)*zz;
        P[i].z=zz;//=cos(theta)*zz+sin(theta)*xx;
        P[i]*=scale;
        P[i]+=trans;
        if (mn.cell[0]>P[i].cell[0]) mn.cell[0]=P[i].cell[0];
        if (mn.cell[1]>P[i].cell[1]) mn.cell[1]=P[i].cell[1];
        if (mn.cell[2]>P[i].cell[2]) mn.cell[2]=P[i].cell[2];
        if (mx.cell[0]<P[i].cell[0]) mx.cell[0]=P[i].cell[0];
        if (mx.cell[1]<P[i].cell[1]) mx.cell[1]=P[i].cell[1];
        if (mx.cell[2]<P[i].cell[2]) mx.cell[2]=P[i].cell[2];
    }
    trig = new triangle*[nf+2];
    IplImage *image = cvLoadImage(Text);
    for (int i=0;i<nf;i++){
        fscanf(fin,"f %d%d%d\n",&F[i*3+0],&F[i*3+1],&F[i*3+2]);
        trig[i]=new triangle(P[F[i*3+0]],P[F[i*3+1]],P[F[i*3+2]]);
        trig[i]->SetMaterial(mat);
        trig[i]->SetText(image);
    }
#ifndef Debug
    root=BuildKdTree(mn,mx,trig,nf,0);
#endif
}
#ifdef Debug
intersect_event Compobj::Intersect( Ray& a_Ray, double& a_Dist ){
    intersect_event event;
    for (int i = 0 ; i < nf ;i++){
        intersect_event event1 = trig[i]->Intersect(a_Ray, a_Dist);
        if (event1.retval)
            event=event1;
    }
    last = GetMaterial()->GetColor();
    return event;
}
#endif

intersect_event Compobj:: SearchKdTree(Ray&a_Ray,const vector3 &mn,const vector3 &mx,KdNode *root,int axis,double &a_Dist)
{
    intersect_event event;
    if (root->isleaf){
        for (int i = 0 ; i < root->cnt ;i++){
            intersect_event event1 = root->tr[i]->Intersect(a_Ray, a_Dist);
            if (event1.retval)
                event=event1;
        }
        return event;
    }
    vector3 dir=a_Ray.GetDirection();
    vector3 st;
    double mdis=1E6,temp;
    bool flag=true;
    for (int ax=0;ax<3&&flag;ax++){
        mdis=1E7;
        st=a_Ray.GetOrigin();
        if ((temp=(mn.cell[ax]-st.cell[ax])/dir.cell[ax])>=0){
            if (mdis>temp)
                mdis=temp;
        }
        if ((temp=(mx.cell[ax]-st.cell[ax])/dir.cell[ax])>=0){
            if (mdis>temp)
                mdis=temp;
        }
        if (mdis>a_Dist) continue;
        st=st+(dir*mdis);
        int u=(ax+1)%3,v=(ax+2)%3;
        if (st.cell[u]>=mn.cell[u]-EPSILON&&st.cell[u]<=mx.cell[u]+EPSILON)
            if (st.cell[v]>=mn.cell[v]&&st.cell[v]<=mx.cell[v]){
                flag=false;
            }
    }
    if (flag) return event;
    double mid = (mx.cell[axis]+mn.cell[axis])/2;
    
    vector3 m=mx;//axis=(axis+1)%3;
    m.cell[axis]=mid;
    event=SearchKdTree(a_Ray,mn,m, root->left, (axis+1)%3,a_Dist);
    m=mn;m.cell[axis]=mid;
    intersect_event event1=SearchKdTree(a_Ray,m,mx, root->right, (axis+1)%3,a_Dist);
    if (event1.retval)
        event=event1;
    return event;
}

#ifndef Debug

intersect_event Compobj::Intersect( Ray& a_Ray, double& a_Dist ){
    intersect_event event;
    event=SearchKdTree(a_Ray,mn,mx,root,0,a_Dist);
    last = GetMaterial()->GetColor();
    return event;
}
#endif
