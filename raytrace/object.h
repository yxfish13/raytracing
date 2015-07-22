//
//  object.h
//  raytracing
//
//  Created by yxfish13 on 15/6/6.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//

#ifndef raytracing_object_h
#define raytracing_object_h
#include "common.h"
#include <opencv2/opencv.hpp>
// Intersection method return values
#define HIT		 1		// Ray hit primitive
#define MISS	 0		// Ray missed primitive
#define INPRIM	-1		// Ray started inside primitive

// -----------------------------------------------------------
// Material class definition
// -----------------------------------------------------------
const double s_light=0.1;
class Material
{
public:
    Material();
    void SetColor( const Color& a_Color ) { m_Color = a_Color; }
    void Setabsorb( const Color& a_Color ) { absorb = a_Color; }
    Color GetColor() { return m_Color; }
    Color GetAbsorb(){ return absorb;}
    void SetDiffuse( double a_Diff ) { m_Diff = a_Diff; }
    void SetSpecular( double a_Spec ) { m_Spec = a_Spec; }
    void SetReflection( double a_Refl ) { m_Refl = a_Refl; }
    void SetRefraction( double a_Refr ) { m_Refr = a_Refr; }
    void SetRefrIndex( double a_Refr ) { m_RIndex = a_Refr; }
    double GetSpecular() { return m_Spec; }
    double GetDiffuse() { if (m_Diff>s_light)return m_Diff-s_light;else return 0; }
    double GetReflection() { return m_Refl; }
    double GetRefraction() { return m_Refr; }
    double GetRefrIndex() { return m_RIndex; }
private:
    Color m_Color;
    Color absorb;
    double m_Refl, m_Refr;
    double m_Diff, m_Spec;
    double m_RIndex;
};

// -----------------------------------------------------------
// Primitive class definition
// -----------------------------------------------------------
class Primitive;
class intersect_event{
public:
    intersect_event():dist(0),retval(0),obj(0){}
    vector3 N;
    vector3 P;
    double dist;
    int retval;
    Primitive *obj;
};
class Primitive
{
public:
    enum
    {
        SPHERE = 1,
        PLANE,
        SQUARE
    };
    Primitive() : m_Name( 0 ), m_Light( false ),istexture(false) {};
    Material* GetMaterial() { return &m_Material; }
    void SetMaterial( Material& a_Mat ) { m_Material = a_Mat; }
    virtual int GetType() = 0;
    virtual intersect_event Intersect( Ray& a_Ray, double& a_Dist ) = 0;
    virtual vector3 GetNormal( vector3& a_Pos ) = 0;
    virtual Color GetColor(double x=-1E6,double y=-1E6 ,double z= -1E6) {
        if(!istexture)return m_Material.GetColor();
       // if (m_Plane.N.z!=0)
        //{ x+=3303;y+=3330;}
        //else
          //  if (m_Plane.N.x!=0){
            //    x=z+3330,y+=3330;
            //}
            //else
              //  x=x+3330,y=z+3330;
        x+=1E5+300;y+=1E5+300;
        if (x<00000||!istexture||y<0)return m_Material.GetColor();
        y=int(y)%(image->height-80)+40;
        x=int(x)%(image->width-80)+40;
        CvScalar pixel = cvGet2D(image, y, x);
        return vector3(1.0*pixel.val[0]/255,1.0*pixel.val[1]/255,1.0*pixel.val[2]/255);
        //return vector3(1.0/224*(((int)(x*y))%230),1.0/224*(((int)(x*y))%230),1.0/224*(((int)(x*y))%230));
        if ((int(x/30)+int(y/30))%2) return vector3(1,1,1);else return vector3(0,0,0);
    }
    virtual Color GetAbsorb() { return m_Material.GetAbsorb(); }
    void SetText(char *Name){image = cvLoadImage(Name);istexture=true; }
    void SetText(IplImage* imag){image=imag;istexture=true;}
    //void SetMaterial(Material m_material){m_Material=m_material;}
    virtual void Light( bool a_Light ) { m_Light = a_Light; }
    bool IsLight() { return m_Light; }
    void SetName( char* a_Name );
    char* GetName() { return m_Name; }
protected:
    Material m_Material;
    char* m_Name;
    bool m_Light;
    bool istexture;
    IplImage* image;
};

// -----------------------------------------------------------
// Sphere primitive class definition
// -----------------------------------------------------------

class Sphere : public Primitive
{
public:
    int GetType() { return SPHERE; }
    Sphere( const vector3& a_Centre, double a_Radius ) :
    m_Centre( a_Centre ), m_SqRadius( a_Radius * a_Radius ),
    m_Radius( a_Radius ), m_RRadius( 1.0f / a_Radius ) {};
    vector3& GetCentre() { return m_Centre; }
    double GetSqRadius() { return m_SqRadius; }
    double GetRadius() { return m_Radius; }
    intersect_event Intersect( Ray& a_Ray, double& a_Dist );
    Color GetColor(double x=-1E6,double y=-1E6 ,double z= -1E6);
    vector3 GetNormal( vector3& a_Pos ) { return (a_Pos - m_Centre) * m_RRadius; }
private:
    vector3 m_Centre;
    double m_SqRadius, m_Radius, m_RRadius;
};

// -----------------------------------------------------------
// PlanePrim primitive class definition
// -----------------------------------------------------------

class PlanePrim : public Primitive
{
public:
    int GetType() { return PLANE; }
    PlanePrim( vector3  a_Normal, double a_D ) : m_Plane( plane( a_Normal.Normal(), a_D ) ) {};
    vector3& GetNormal() { return m_Plane.N; }
    double GetD() { return m_Plane.D; }
    intersect_event Intersect( Ray& a_Ray, double& a_Dist );
    Color GetColor(double x=-1E6,double y=-1E6  ,double z=-1E6);
    vector3 GetNormal( vector3& a_Pos );
private:
    plane m_Plane;
};

class triangle : public Primitive
{
public:
    int GetType() { return 3; }
    triangle( const vector3 & a,const vector3 & b,const vector3 & c  ) :Pa(a),Pb(b),Pc(c)
    {
        e1=Pb-Pa,e2=Pc-Pa;
        N=e1.Cross(e2).Normal();
    }
    vector3& GetNormal() { return N; }
    intersect_event Intersect( Ray& a_Ray, double& a_Dist );
    //Color GetColor(double x=-1E6,double y=-1E6  ,double z=-1E6) ;
    vector3 GetNormal( vector3& a_Pos ){return N;}
    int cross(int axis,double x){
        int tmp=(Pa.cell[axis]>x)+(Pb.cell[axis]>x)+(Pc.cell[axis]>x);
        return (tmp+1)/2-1;
    }
private:
    vector3 Pa,Pb,Pc,e1,e2;
    vector3 N;
    double buv;
};

class Square : public Primitive
{
public:
    Square( const vector3 & a_pos, const vector3 &a_R ) : pos(a_pos),R(a_R) {}
    int GetType() { return SQUARE; }
    intersect_event Intersect( Ray& a_Ray, double& a_Dist );
    vector3 GetPos( ){return pos;}
    vector3 GetR() {return R;}
    vector3 GetNormal( vector3& a_Pos ){vector3 A(0,1,0);return A;}
    //void Lig
private:
    vector3 pos;
    vector3 R;
    vector3 N;
};
struct KdNode{
    KdNode():left(0),right(0),isleaf(0),cnt(0),tr(0){}
    KdNode *left,*right;
    bool isleaf;
    int cnt;
    triangle ** tr;
};
class Compobj : public Primitive
{
public:
    int GetType() { return 4; }
    Compobj(char *filename,double scale,vector3 trans,Material &mat);
    Compobj(char *filename,double scale,vector3 trans,Material &mat,char *Text);
    intersect_event Intersect( Ray& a_Ray, double& a_Dist );
    vector3 GetNormal( vector3& a_Pos ){ std::cout<<"normal"; return P[0];}
    Color GetColor(double x=-1E6,double y=-1E6  ,double z=-1E6) {return last;}
    void setAdll(){for (int i=0;i<nf;i++) trig[i]->SetMaterial(*GetMaterial());}
    KdNode * BuildKdTree(const vector3 &mn,const vector3 &mx,triangle**tria,int cnt,int axis);
    intersect_event SearchKdTree(Ray&a_Ray,const vector3 &mn,const vector3 &mx,KdNode *root,int axis,double &a_Dist);
    
private:
    triangle **trig;
    KdNode *root;
    vector3 mn,mx;
    vector3 *P;
    int *F;
    Color last;
    int np,nf;
};


#endif
