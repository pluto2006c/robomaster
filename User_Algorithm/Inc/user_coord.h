#ifndef __USER_COORD_H__
#define __USER_COORD_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 常量定义 ------------------------------------------------------------------*/
#define USER_PI        (3.14159265358979323846f)
#define USER_TWO_PI    (6.28318530717958647692f)
#define USER_HALF_PI   (1.57079632679489661923f)

#define PRECISION       1e-6f


/* 类型定义 ------------------------------------------------------------------*/

// 笛卡尔坐标系
typedef struct {
    float x;
    float y;
    float z;
} CartesianCoord;

// 极坐标系
typedef struct {
    float radius;
    float yaw;
    float pitch;
} PolarCoord;

/* 函数声明 ------------------------------------------------------------------*/
// 基础函数
float Math_Deg2Rad(float deg);
float Math_Rad2Deg(float rad);
float Math_WrapAngleRad(float rad);
float Math_WrapAngleDeg(float deg);

// 笛卡尔坐标系函数
float Cartesian_Distance(const CartesianCoord* p1, const CartesianCoord* p2);
float Cartesian_Magnitude(const CartesianCoord* coord);
float Cartesian_AngleDifference(const CartesianCoord* c1, const CartesianCoord* c2);
void Cartesian_Add(const CartesianCoord* c1, const CartesianCoord* c2, CartesianCoord* result);
void Cartesian_Subtract(const CartesianCoord* c1, const CartesianCoord* c2, CartesianCoord* result);
void Cartesian_Scale(const CartesianCoord* coord, float scale, CartesianCoord* result);
void Cartesian_Normalize(const CartesianCoord* coord, CartesianCoord* result);
float Cartesian_DotProduct(const CartesianCoord* c1, const CartesianCoord* c2);
void Cartesian_CrossProduct(const CartesianCoord* c1, const CartesianCoord* c2, CartesianCoord* result);

// 极坐标系函数
float Polar_Distance(const PolarCoord* p1, const PolarCoord* p2);
float Polar_AbsAngleDifference(const PolarCoord* p1, const PolarCoord* p2);
float Polar_YawDifference(const PolarCoord* p1, const PolarCoord* p2);
float Polar_PitchDifference(const PolarCoord* p1, const PolarCoord* p2);
void Polar_Add(const PolarCoord* p1, const PolarCoord* p2, PolarCoord* result);
void Polar_Subtract(const PolarCoord* p1, const PolarCoord* p2, PolarCoord* result);
void Polar_Scale(const PolarCoord* coord, float scale, PolarCoord* result);
void Polar_Normalize(const PolarCoord* coord, PolarCoord* result);

// 坐标转换函数
void Cartesian_ToPolar(const CartesianCoord* cartesian, PolarCoord* result);
void Polar_ToCartesian(const PolarCoord* polar, CartesianCoord* result);

#endif //__USER_COORD_H__

