/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_coord.h"
#include <math.h>

/* 函数体 --------------------------------------------------------------------*/

/**
* @brief 角度转弧度
* @param deg 角度(°)
* @return 弧度(rad)
*/
float Math_Deg2Rad(float deg) {
    return deg * (USER_PI / 180.0f);
}

/**
* @brief 弧度转角度
* @param rad 弧度(rad)
* @return 角度(°)
*/
float Math_Rad2Deg(float rad) {
    return rad * (180.0f / USER_PI);
}

/**
* @brief 将弧度角限制到 [-pi, pi]
* @param rad 输入弧度(rad)
* @return 归一化后的弧度(rad)
*/
float Math_WrapAngleRad(float rad) {
    float x = fmodf(rad, USER_TWO_PI);
    if (x > USER_PI) {
        x -= USER_TWO_PI;
    } else if (x < -USER_PI) {
        x += USER_TWO_PI;
    }
    return x;
}

/**
* @brief 将角度限制到 [-180, 180]
* @param deg 输入角度(deg)
* @return 归一化后的角度(deg)
*/
float Math_WrapAngleDeg(float deg) {
    float x = fmodf(deg, 360.0f);
    if (x > 180.0f) {
        x -= 360.0f;
    } else if (x < -180.0f) {
        x += 360.0f;
    }
    return x;
}

/**
* @brief 计算两点间距离
* @param p1 点1
* @param p2 点2
* @return 欧氏距离
*/
float Cartesian_Distance(const CartesianCoord* p1, const CartesianCoord* p2) {
    const float dx = p2->x - p1->x;
    const float dy = p2->y - p1->y;
    const float dz = p2->z - p1->z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

/**
* @brief 计算向量模长
* @param coord 笛卡尔坐标
* @return 向量模长
*/
float Cartesian_Magnitude(const CartesianCoord* coord) {
    return sqrtf(coord->x * coord->x + coord->y * coord->y + coord->z * coord->z);
}

/**
* @brief 向量加法
* @param c1 向量1
* @param c2 向量2
* @param result 结果向量
* @return 相加结果
*/
void Cartesian_Add(const CartesianCoord* c1, const CartesianCoord* c2, CartesianCoord* result) {
    result->x = c1->x + c2->x;
    result->y = c1->y + c2->y;
    result->z = c1->z + c2->z;
}

/**
* @brief 向量减法
* @param c1 向量1
* @param c2 向量2
* @param result 结果向量
*/
void Cartesian_Subtract(const CartesianCoord* c1, const CartesianCoord* c2, CartesianCoord* result) {
    result->x = c1->x - c2->x;
    result->y = c1->y - c2->y;
    result->z = c1->z - c2->z;
}

/**
* @brief 向量缩放
* @param coord 输入向量
* @param scale 缩放系数
* @param result 结果向量
*/
void Cartesian_Scale(const CartesianCoord* coord, float scale, CartesianCoord* result) {
    result->x = coord->x * scale;
    result->y = coord->y * scale;
    result->z = coord->z * scale;
}

/**
* @brief 向量归一化
* @param coord 输入向量
* @param result 结果向量(单位向量)
*/
void Cartesian_Normalize(const CartesianCoord* coord, CartesianCoord* result) {
    const float mag = Cartesian_Magnitude(coord);
    if (mag < PRECISION) {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    } else {
        Cartesian_Scale(coord, 1.0f / mag, result);
    }
}

/**
* @brief 向量点积
* @param c1 向量1
* @param c2 向量2
* @return 点积结果
*/
float Cartesian_DotProduct(const CartesianCoord* c1, const CartesianCoord* c2) {
    return c1->x * c2->x + c1->y * c2->y + c1->z * c2->z;
}

/**
* @brief 计算两个向量之间的夹角
* @param c1 向量1
* @param c2 向量2
* @return 夹角(度)，范围[0, 180]
* @note 返回值始终为正值，表示两向量之间的最小夹角
*/
float Cartesian_AngleDifference(const CartesianCoord* c1, const CartesianCoord* c2) {
    const float mag1 = Cartesian_Magnitude(c1);
    const float mag2 = Cartesian_Magnitude(c2);
    
    // 防止除零
    if (mag1 < PRECISION || mag2 < PRECISION) {
        return 0.0f;
    }
    
    // 计算夹角余弦值
    float cos_angle = Cartesian_DotProduct(c1, c2) / (mag1 * mag2);
    
    // 限制在[-1, 1]范围内，防止浮点误差
    if (cos_angle > 1.0f) cos_angle = 1.0f;
    if (cos_angle < -1.0f) cos_angle = -1.0f;
    
    // 返回角度(度)
    return Math_Rad2Deg(acosf(cos_angle));
}

/**
* @brief 向量叉积
* @param c1 向量1
* @param c2 向量2
* @param result 结果向量 (c1 × c2)
*/
void Cartesian_CrossProduct(const CartesianCoord* c1, const CartesianCoord* c2, CartesianCoord* result) {
    result->x = c1->y * c2->z - c1->z * c2->y;
    result->y = c1->z * c2->x - c1->x * c2->z;
    result->z = c1->x * c2->y - c1->y * c2->x;
}

/**
* @brief 极坐标归一化
* @param coord 输入极坐标
* @param result 结果极坐标(角度限制在[-180, 180])
*/
void Polar_Normalize(const PolarCoord* coord, PolarCoord* result) {
    result->radius = fabsf(coord->radius);
    result->yaw = Math_WrapAngleDeg(coord->yaw);
    result->pitch = Math_WrapAngleDeg(coord->pitch);
}

/**
* @brief 极坐标加法(通过笛卡尔坐标系实现)
* @param p1 极坐标1
* @param p2 极坐标2
* @param result 结果极坐标
*/
void Polar_Add(const PolarCoord* p1, const PolarCoord* p2, PolarCoord* result) {
    CartesianCoord c1, c2, c_result;
    
    // 转换为笛卡尔坐标
    Polar_ToCartesian(p1, &c1);
    Polar_ToCartesian(p2, &c2);
    
    // 笛卡尔坐标加法
    Cartesian_Add(&c1, &c2, &c_result);
    
    // 转换回极坐标
    Cartesian_ToPolar(&c_result, result);
}

/**
* @brief 极坐标减法(通过笛卡尔坐标系实现)
* @param p1 极坐标1
* @param p2 极坐标2
* @param result 结果极坐标 (p1 - p2)
*/
void Polar_Subtract(const PolarCoord* p1, const PolarCoord* p2, PolarCoord* result) {
    CartesianCoord c1, c2, c_result;
    
    // 转换为笛卡尔坐标
    Polar_ToCartesian(p1, &c1);
    Polar_ToCartesian(p2, &c2);
    
    // 笛卡尔坐标减法
    Cartesian_Subtract(&c1, &c2, &c_result);
    
    // 转换回极坐标
    Cartesian_ToPolar(&c_result, result);
}

/**
* @brief 极坐标缩放(半径缩放)
* @param coord 输入极坐标
* @param scale 缩放系数
* @param result 结果极坐标
*/
void Polar_Scale(const PolarCoord* coord, float scale, PolarCoord* result) {
    result->radius = coord->radius * scale;
    result->yaw = coord->yaw;
    result->pitch = coord->pitch;
}

/**
* @brief 计算两个极坐标点之间的欧氏距离
* @param p1 极坐标1
* @param p2 极坐标2
* @return 欧氏距离
*/
float Polar_Distance(const PolarCoord* p1, const PolarCoord* p2) {
    CartesianCoord c1, c2;
    
    // 转换为笛卡尔坐标
    Polar_ToCartesian(p1, &c1);
    Polar_ToCartesian(p2, &c2);
    
    // 计算笛卡尔距离
    return Cartesian_Distance(&c1, &c2);
}

/**
* @brief 计算两个极坐标点之间的角度差(综合yaw和pitch)
* @param p1 极坐标1
* @param p2 极坐标2
* @return 角度差(度)
*/
float Polar_AbsAngleDifference(const PolarCoord* p1, const PolarCoord* p2) {
    const float yaw_diff = Math_WrapAngleDeg(p2->yaw - p1->yaw);
    const float pitch_diff = Math_WrapAngleDeg(p2->pitch - p1->pitch);
    
    // 计算综合角度差
    return sqrtf(yaw_diff * yaw_diff + pitch_diff * pitch_diff);
}

/**
* @brief 计算两个极坐标点之间的yaw角度差(带正负)
* @param p1 极坐标1
* @param p2 极坐标2
* @return yaw角度差(度)，范围[-180, 180]
* @note 正值表示p2在p1的逆时针方向，负值表示顺时针方向
*/
float Polar_YawDifference(const PolarCoord* p1, const PolarCoord* p2) {
    return Math_WrapAngleDeg(p2->yaw - p1->yaw);
}

/**
* @brief 计算两个极坐标点之间的pitch角度差(带正负)
* @param p1 极坐标1
* @param p2 极坐标2
* @return pitch角度差(度)，范围[-180, 180]
* @note 正值表示p2在p1的上方，负值表示下方
*/
float Polar_PitchDifference(const PolarCoord* p1, const PolarCoord* p2) {
    return Math_WrapAngleDeg(p2->pitch - p1->pitch);
}

/**
* @brief 笛卡尔坐标转极坐标
* @param cartesian 笛卡尔坐标
* @param result 结果极坐标(角度单位:度)
*/
void Cartesian_ToPolar(const CartesianCoord* cartesian, PolarCoord* result) {
    // 计算半径
    result->radius = Cartesian_Magnitude(cartesian);
    
    // 计算偏航角 (yaw) - 水平面投影角度
    result->yaw = Math_Rad2Deg(atan2f(cartesian->y, cartesian->x));
    
    // 计算俯仰角 (pitch) - 与水平面夹角
    const float horizontal_dist = sqrtf(cartesian->x * cartesian->x + cartesian->y * cartesian->y);
    result->pitch = Math_Rad2Deg(atan2f(cartesian->z, horizontal_dist));
}

/**
* @brief 极坐标转笛卡尔坐标
* @param polar 极坐标(角度单位:度)
* @param result 结果笛卡尔坐标
*/
void Polar_ToCartesian(const PolarCoord* polar, CartesianCoord* result) {
    const float yaw_rad = Math_Deg2Rad(polar->yaw);
    const float pitch_rad = Math_Deg2Rad(polar->pitch);

    const float cos_pitch = cosf(pitch_rad);
    
    result->x = polar->radius * cos_pitch * cosf(yaw_rad);
    result->y = polar->radius * cos_pitch * sinf(yaw_rad);
    result->z = polar->radius * sinf(pitch_rad);
}
