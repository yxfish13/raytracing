//
//  common.h
//  raytracing
//
//  Created by yxfish13 on 15/6/6.
//  Copyright (c) 2015年 x-yu13. All rights reserved.
//

#ifndef raytracing_common_h
#define raytracing_common_h

#include "math.h"
#include "stdlib.h"

typedef unsigned int Pixel;

inline double Rand( double a_Range ) { return ((double)rand() / RAND_MAX) * a_Range; }


#define DOT(A,B)		(A.x*B.x+A.y*B.y+A.z*B.z)
#define NORMALIZE(A)	{double l=1/sqrtf(A.x*A.x+A.y*A.y+A.z*A.z);A.x*=l;A.y*=l;A.z*=l;}
#define LENGTH(A)		(sqrtf(A.x*A.x+A.y*A.y+A.z*A.z))
#define SQRLENGTH(A)	(A.x*A.x+A.y*A.y+A.z*A.z)
#define SQRDISTANCE(A,B) ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)+(A.z-B.z)*(A.z-B.z))

#define EPSILON			0.001
#define TRACEDEPTH		6

#define PI				3.141592653589793238462f

class vector3
{
public:
	vector3( double a_X=0, double a_Y=0, double a_Z=0 ) : x( a_X ), y( a_Y ), z( a_Z ) {};
	void Set( double a_X, double a_Y, double a_Z ) { x = a_X; y = a_Y; z = a_Z; }
    void Normalize() { double l = 1.0f / Length(); x *= l; y *= l; z *= l; }
    vector3 Normal() { vector3 A=*this;A.Normalize(); return A;}
    double Length() { return (double)sqrt( x * x + y * y + z * z ); }
	double SqrLength() { return x * x + y * y + z * z; }
	double Dot( vector3 a_V ) { return x * a_V.x + y * a_V.y + z * a_V.z; }
    double Maxis(){
        double ans=x+0.00001;
        if (y>ans) ans=y;
        if (z>ans) ans=z;
        return ans;
    }
	vector3 Cross( vector3 b ) { return vector3( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x ); }
	void operator += ( const vector3& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator -= ( vector3& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator *= ( double f ) { x *= f; y *= f; z *= f; }
	void operator *= ( vector3& a_V ) { x *= a_V.x; y *= a_V.y; z *= a_V.z; }
	vector3 operator- () const { return vector3( -x, -y, -z ); }
	friend vector3 operator + ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
	friend vector3 operator - ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	friend vector3 operator * ( const vector3& v, double f ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	friend vector3 operator * ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
	friend vector3 operator * ( double f, const vector3& v ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	union
	{
		struct { double x, y, z; };
		struct { double r, g, b; };
		struct { double cell[3]; };
	};
};

class plane
{
public:
	plane() : N( 0, 0, 0 ), D( 0 ) {};
	plane( vector3 a_Normal, double a_D ) : N( a_Normal ), D( a_D ) {};
	union
	{
		struct
		{
			vector3 N;
			double D;
		};
		double cell[4];
	};
};

class Ray
{
public:
    Ray(const vector3 &A=vector3(0,0,0),const vector3 &B=vector3(0,0,0)):start(A),dir(B){}
    vector3 GetDirection()     {return dir;}
    vector3 GetOrigin()        {return start;}
    void    set_direction(const vector3 &a){dir=a;}
    void    set_origin(const vector3 &a){start=a;}
private:
    vector3 dir;
    vector3 start;
};
double pow(double x,int y);
double Rand01();
typedef vector3 Color;

#endif
