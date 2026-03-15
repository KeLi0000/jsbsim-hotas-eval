#pragma once

#include <cstdint>

class CIATPoint {
public:
    CIATPoint();
    CIATPoint(const CIATPoint &);
    explicit CIATPoint(double p[3]);
    CIATPoint(double x, double y, double z);

    CIATPoint operator+(CIATPoint);
    CIATPoint operator-(CIATPoint);
    CIATPoint operator-();
    double operator*(CIATPoint &);
    CIATPoint operator*(double);
    CIATPoint operator/(double);
    CIATPoint operator*=(double);
    double &operator[](uint32_t i);

    void Value(double *val);

    // 平移操作
    void Translation(double offset[3]);
    // 放缩操作
    void Scale(double factor);
    // 旋转操作，绕axis（0：x，1：y，2：z）旋转angle（rad）
    void Rotation(double angle, int axis);
    // 求矢量方位角，rad
    double YawRad();
    // 求矢量俯仰角，rad
    double PitchRad();
    // 求p范数
    double Norm(uint16_t order = 2);
    // 叉乘向量
    CIATPoint CrossProduct(CIATPoint);
    // 求矢量和另外矢量夹角的cos
    double CosAngle(double[3]);
    double CosAngle(CIATPoint);

private:
    double m_Point[3]{0.0};  // 列向量
    double m_PointBk[3]{0.0};// 备份
    void backup();
    void recover();

    void plus(const double[3]);
    void multiply(double);
    // 点积 行向量 乘 列向量
    double product(const double p[3]);
    // 向量积 ×乘 向量
    void cross_product(const double b[3], double c[]);
    // 矩阵 乘 列向量
    void multiply(double[3][3]);

    double norm();
    static double norm(double[3]);
    double angle(double[3]);
};
