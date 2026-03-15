#include "pch.h"
#include "IAT_Math.h"
#include <cmath>
#include <ctime>

////速度换算
double ZkMs2Kmh(   ////将m/s 转换为 km/h
        double lVel////待转换速度，单位：m/s
) {
    return lVel * 3.6;
}

double ZkKmh2Ms(   ////将km/h 转换为 m/s
        double lVel////待转换速度，单位：km/h
) {
    return lVel / 3.6;///或 *0.278
}

double ZkMh2Kmh(   ////将 马赫数 转换为 km/h
        double lVel////待转换速度，单位：马赫
) {
    return lVel * 1225;
}

double ZkKmh2Mh(   ////将km/h 转换为 马赫数
        double lVel////待转换速度，单位：km/h
) {
    return lVel / 1225;///或 *0.278
}

double ZkMh2Ms(    ////将 马赫数 转换为 m/s
        double lVel////待转换速度，单位：m/s
) {
    return lVel * 340.3;
}

double ZkMs2Mh(    ////将m/s 转换为 马赫数
        double lVel////待转换速度，单位：m/s
) {
    return lVel / 340.3;///或 *0.278
}

double ZkKmh2Kn(   ////将km/h 转换为 kn(1节=1海里/h)
        double lVel////待转换速度，单位：km/h
) {
    return lVel / 1.852;
}

double ZkKn2Kmh(   ////将 kn(1节=1海里/h) 转换为 km/h
        double lVel////待转换速度，单位：kn(节)
) {
    return lVel * 1.852;
}

double ZkMs2Kn(    ////将m/s 转换为 kn(1节=1海里/h)
        double lVel////待转换速度，单位：m/s
) {
    return lVel / 0.51444444444444;
}
double ZkKn2Ms(    ////将 kn(1节=1海里/h) 转换为 m/s
        double lVel////待转换速度，单位：kn(节)
) {
    return lVel * 0.51444444444444;
}

////距离换算

double ZkKm2Nm(    ////将公里 转换为 海里 nmail
        double lVel////待转换速度，单位：km
) {
    return lVel / 1.852;
}

double ZkNm2Km(    ////将 海里 nmail 转换为 公里
        double lVel////待转换速度，单位：nm
) {
    return lVel * 1.852;
}

double ZkNm2Cab(   ////将海里nmail 转换为 海链 Cable
        double lVel////待转换速度，单位：nm
) {
    return lVel * 10;
}

double LkFt2m(     //// 将 英尺 Foot 转换为 米 m
        double lDis//// 待转换距离，单位：m
) {
    return lDis * 0.3048;
}

double LkFt22m2(double lArea) { return lArea * 0.092903; }

double LkLbs2N(double lForce) { return lForce * 4.4492; }

double LkM2Ft(double lDis) { return lDis * 3.2808399; }

double LkMS2FtS(double lVel) { return LkM2Ft(lVel); }

double ZkCab2Nm(   ////将 海链 Cable 转换为 海里nmail
        double lVel////待转换速度，单位：Cal
) {
    return lVel / 10;
}

////sign函数实现
double ZkSign(////计算符号函数 输入>0 输出为1； 输入<0 输出-1； 输入=0 输出0
        double lX) {
    double Value = 0.0;
    if (lX > 0) { Value = 1; }
    if (lX == 0) { Value = 0; }
    if (lX < 0) { Value = -1; }
    return Value;
}

double ZkAt(      // 根据坐标计算方位角（单位：弧度）,北向为零度,逆时针为正方向(-PI~PI)
        double lX,// X轴坐标，北向
        double lY // Y轴坐标，东向
) {
    double sita = 0.0;
    if (lX >= 0 && lY <= 0) {
        sita = -atan(lY / lX);
    } else if (lX < 0 && lY <= 0) {
        sita = M_PI - atan(lY / lX);
    } else if (lX < 0 && lY > 0) {
        sita = M_PI - atan(lY / lX);
    } else if (lX >= 0 && lY > 0) {
        sita = 2 * M_PI - atan(lY / lX);
    }

    if (sita > M_PI) sita -= 2 * M_PI;

    return sita;
}

double ZkAt2(     // 根据坐标计算方位角（单位：弧度）,北向为零度,逆时针为正方向(0~2PI)
        double lX,// X轴坐标，北向
        double lY // Y轴坐标，东向
) {
    double sita = ZkAt(lX, lY);

    if (sita < .0) sita += 2 * M_PI;

    return sita;
}

double ZkMc(        // 计算角度使其转化为-PI～PI（单位：弧度）
        double lSita// 角度值，单位：弧度
) {
    lSita = ZkMc2(// 计算角度使其转化为0～2PI（单位：弧度）
            lSita // 角度值，单位：弧度
    );

    // 再转换到-PI~PI之间
    if (lSita >= M_PI) { lSita = lSita - 2.0 * M_PI; }
    return lSita;
}

double ZkMc2(       // 计算角度使其转化为0～2PI（单位：弧度）
        double lSita// 角度值，单位：弧度
) {
    double n = abs(int(lSita / (2.0 * M_PI)));

    if (lSita >= .0) {
        lSita = lSita - 2.0 * M_PI * (n);
    } else if (lSita <= .0) {
        lSita = lSita + 2.0 * M_PI * (n + 1);
    }
    return lSita;
}

double ZkNoZero( // 防止除零的时候使用，当X<ERR时，返回值为ERR
        double lX// 待检测参数
) {
    double tTemp = ERR;
    if (lX > ERR) { tTemp = lX; }

    return tTemp;
}

double ZkD2R(    // 将度转换为弧度
        double lD// 待转换角度，单位：度
) {
    return lD * M_PI / 180.0;
}

double ZkR2D(    // 将弧度转换为度
        double lH// 待转换角度，单位：弧度
) {
    return lH * 180.0 / M_PI;
}

int ZkSgn(       // 定义符号函数，正数返回1，负数返回-1，0返回0
        double lX// 待检测参数
) {
    int tReturn;

    if (lX > 0) {
        tReturn = 1;
    } else if (lX < 0) {
        tReturn = -1;
    } else {
        tReturn = 0;
    }

    return tReturn;
}

double ZkSgnX(   // 定义修正的符号函数，使其连续化
        double lX// 待检测参数
) {
    return lX / (fabs(lX) + 0.001);
}

double LkNorm(double X[], int D, double order) {
    double sum = 0;
    for (int i = 0; i < D; ++i) { sum += pow(X[i], order); }
    sum = pow(sum, 1 / order);
    return sum;
}

void LkClip(double X_clip[], double X[], double X_min[], double X_max[], int D) {
    for (int i = 0; i < D; ++i) X_clip[i] = LkClip(X[i], X_min[i], X_max[i]);
}

double LkClip(double X, double X_min, double X_max) {
    if (X <= X_min) return X_min;
    else if (X >= X_max)
        return X_max;
    else
        return X;
}

double LkRandn() {
    srand((unsigned) time(nullptr));
    int tmp_num = 1000;
    double tmp_x = .0;
    for (int i = 0; i < tmp_num; ++i) { tmp_x += (double) rand() / RAND_MAX; }
    tmp_x -= tmp_num / 2.0;
    tmp_x /= sqrt(tmp_num / 12.0);
    return tmp_x;
}

double LkRand(double a, double b) {
    srand((unsigned) time(nullptr));
    double tmp_x = .0;
    tmp_x = (double) rand() / RAND_MAX;
    tmp_x = (b - a) * tmp_x + a;
    return tmp_x;
}

double LkRound(double x, int n) {
	double tmp = x * pow(10, n);
	tmp = lround(tmp);
	tmp /= pow(10, n);
    return tmp;
}

int LkRound(double x) {
    int y = 0;
    y = (lround(x + 0.5) > int(x)) ? (int(x) + 1) : int(x);
    return y;
}

double LkAngle2D(double a[2], double b[2]) {
    double cos_angle = LkVecDot(a, b, 2) / (LkNorm(a, 2) * LkNorm(b, 2));
    return acos(cos_angle);
}

double LkAngle3D(double a[3], double b[3]) {
    double cos_angle = LkVecDot(a, b, 3) / (LkNorm(a, 3) * LkNorm(b, 3));
    return acos(cos_angle);
}

double LkVecDot(const double a[], const double b[], int dim) {
    double res = 0;
    for (int i = 0; i < dim; ++i) { res += a[i] * b[i]; }
    return res;
}

double T0 = 288.150;   // 海平面绝对温度, 开尔文
double L = -6.5e-3;    // 垂直温度梯度，开尔文/m
double g0 = 9.80665;   // 标准重力加速度, m/s2
double R = 287.05287;  // 干空气比气体常数，焦耳/(开·千克)
double P0 = 101325.0;  //海平面大气压强, 帕
double r0 = 6356.766e3;// 地球半径，m

double LkGeoHeight(double h_asl) {
    double h_geo = r0 * h_asl / (r0 + h_asl);
    return h_geo;
}

double LkAirTemperature(double h_asl) {
    double h_geo = LkGeoHeight(h_asl);
    double T = 0;
    if (h_geo < 11e3) {
        T = T0 + L * h_geo;
    } else {
        T = 216.65;
    }
    return T;
}
double LkAirPressure(double h_asl) {
    double Ph = 0.0;
    double h_geo = LkGeoHeight(h_asl);
    double T = LkAirTemperature(h_asl);
    if (h_geo < 11e3) {
        Ph = P0 * pow(T / T0, -g0 / (L * R));
    } else {
        Ph = exp(-g0 * (h_geo - 11e3) / (R * T));
    }
    return Ph;
}
double LkAirDensity(double h_asl) {
    double Ph = LkAirPressure(h_asl);
    double T = LkAirTemperature(h_asl);
    double roh = Ph / (R * T);
    return roh;
}

double LkGravity(double h_asl) {
    double gh = g0 * pow(r0 / (r0 + h_asl), 2);
    return gh;
}

double LkSoundSpeed(double h_asl)
{
    double speed = 20.05 * sqrt(288 - h_asl * 0.65 / 100);
    return speed;
}

double LkRound(double src, uint8_t precision)
{
    double tmp_res = round(src * pow(10, precision));
    tmp_res /= pow(10, precision);
    return tmp_res;
}
