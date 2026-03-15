#pragma once

#include <cstdint>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923132169163975144
#endif

#ifndef M_PI_4
#define M_PI_4 0.78539816339744830961566084581988
#endif

#define BOLTZMANN_K 1.380649e-23  // 玻尔兹曼常数，J*K-1
#define STAND_ROOM_TEMPERATURE 290// 标准室温，K
#define ERR 1e-6

////速度换算
////将m/s 转换为 km/h
double ZkMs2Kmh(double lVel////待转换速度，单位：m/s
);

////将km/h 转换为 m/s
double ZkKmh2Ms(double lVel////待转换速度，单位：km/h
);

////将 马赫数 转换为 km/h
double ZkMh2Kmh(double lVel////待转换速度，单位：马赫
);

////将km/h 转换为 马赫数
double ZkKmh2Mh(double lVel////待转换速度，单位：km/h
);

////将 马赫数 转换为 m/s
double ZkMh2Ms(double lVel////待转换速度，单位：m/s
);

////将m/s 转换为 马赫数
double ZkMs2Mh(double lVel////待转换速度，单位：m/s
);

////将km/h 转换为 kn(1节=1海里/h)
double ZkKmh2Kn(double lVel////待转换速度，单位：km/h
);

////将 kn(1节=1海里/h) 转换为 km/h
double ZkKn2Kmh(double lVel////待转换速度，单位：kn(节)
);

////将m/s 转换为 kn(1节=1海里/h)
double ZkMs2Kn(double lVel////待转换速度，单位：m/s
);

////将 kn(1节=1海里/h) 转换为 m/s
double ZkKn2Ms(double lVel////待转换速度，单位：kn(节)
);

////距离换算

////将公里 转换为 海里 nmail
double ZkKm2Nm(double lDis////待转换速度，单位：km
);

////将 海里 nmail 转换为 公里
double ZkNm2Km(double lDis////待转换速度，单位：nm
);

////将海里nmail 转换为 海链 Cable
double ZkNm2Cab(double lDis////待转换速度，单位：nm
);

////将 海链 Cable 转换为 海里nmail
double ZkCab2Nm(double lDis////待转换速度，单位：Cal
);

////英制单位换算
//// 将 英尺 Foot 转换为 米 m
double LkFt2m(double lDis//// 待转换距离，单位：m
);

// 将 平方英尺ft^2 转换为 平方米m^2
double LkFt22m2(double lArea);

// 将 磅力lbs 转换为牛顿N
double LkLbs2N(double lForce);

// 将 米m 转换为 英尺ft
double LkM2Ft(double lDis);

// 将 米每秒m/s 转换为 英尺每秒ft/s
double LkMS2FtS(double lVel);

////角度换算
//// 将度转换为弧度
double ZkD2R(double lD//// 待转换角度，单位：度
);

//// 将弧度转换为度
double ZkR2D(double lR//// 待转换角度，单位：弧度
);

//// 根据坐标计算方位角（单位：弧度）,北向为零度,反时针为正方向(-PI~PI)
double ZkAt(double lX,//// X轴坐标
            double lY //// Y轴坐标
);

//// 根据坐标计算方位角（单位：弧度）,北向为零度,反时针为正方向(0~2PI)
double ZkAt2(double lX,//// X轴坐标
             double lY //// Y轴坐标
);

//// 计算角度使其转化为-PI～PI（单位：弧度）
double ZkMc(double lSita//// 角度值，单位：弧度
);

//// 计算角度使其转化为0～2PI（单位：弧度）
double ZkMc2(double lSita//// 角度值，单位：弧度
);

//// 防止除零的时候使用，当X<ERR时，返回值为ERR
double ZkNoZero(double lX//// 待检测参数
);

//// 定义符号函数，正数返回1，负数返回-1，0返回0
int ZkSgn(double lX//// 待检测参数
);

//// 定义修正的符号函数，使其连续化
double ZkSgnX(double lX//// 待检测参数
);

////其他函数
////计算符号函数 输入>0 输出为1； 输入<0 输出-1； 输入=0 输出0
double ZkSign(double lX);

//// 范数计算
double LkNorm(double X[], int D, double order = 2);

//// 限幅
void LkClip(double X_clip[], double X[], double X_min[], double X_max[], int D);
double LkClip(double X, double X_min, double X_max);

//// 随机数产生

/// 正态随机数
double LkRandn();

/// 均匀随机数
double LkRand(double a, double b);

//// 四舍五入，保留n位小数
double LkRound(double x, int n);

//// 四舍五入
int LkRound(double x);

//// 计算2维向量夹角
double LkAngle2D(double a[2], double b[2]);

//// 计算3维向量夹角
double LkAngle3D(double a[3], double b[3]);

//// 计算向量点积
double LkVecDot(const double a[], const double b[], int dim);

/*!
 * 计算位势高度
 * @param h_asl 海平面高度，几何高度，m
 * @return 位势高度，m
 */
double LkGeoHeight(double h_asl);
/*!
 * 计算空气温度
 * @param h_asl 海平面高度，m
 * @return 空气温度，开尔文
 */
double LkAirTemperature(double h_asl);
/*!
 * 计算大气压强
 * @param h_asl 海平面高度，m
 * @return 大气压强，帕斯卡
 */
double LkAirPressure(double h_asl);
/*!
 * 计算空气密度
 * @param h_asl 海平面高度，几何高度，m
 * @return 空气密度 kg/m3
 */
double LkAirDensity(double h_asl);
/*!
 * 计算重力加速度
 * @param h_asl 海平面高度，几何高度，m
 * @return 重力加速度，m/s2
 */
double LkGravity(double h_asl);
/*!
 * 计算音速
 * 
 * 
 */
double LkSoundSpeed(double h_asl);

/*!
 * 四舍五入保留N位小数
 * @param src 原始数据
 * @param precision 保留位数
 * @return 保留结果
 */
double LkRound(double src, uint8_t precision);
