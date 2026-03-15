#include "pch.h"
#include "IAT_Point3.h"
#include "IAT_Math.h"
#include <cmath>
#include <cstring>

CIATPoint::CIATPoint() { memset(m_Point, 0, 3 * sizeof(double)); }

CIATPoint::CIATPoint(const CIATPoint &p) { *this = p; }

CIATPoint::CIATPoint(double p[3]) { memcpy(m_Point, p, 3 * sizeof(double)); }

CIATPoint::CIATPoint(const double x, const double y, const double z) {
    m_Point[0] = x;
    m_Point[1] = y;
    m_Point[2] = z;
}

CIATPoint CIATPoint::operator+(CIATPoint offset) {
    backup();
    double tmp_offset[3];
    offset.Value(tmp_offset);
    plus(tmp_offset);
    CIATPoint tmp(m_Point);
    recover();
    return tmp;
}

CIATPoint CIATPoint::operator-(CIATPoint offset) {
    backup();
    double tmp_offset[3];
    (-offset).Value(tmp_offset);
    plus(tmp_offset);
    CIATPoint tmp(m_Point);
    recover();
    return tmp;
}

CIATPoint CIATPoint::operator-() {
    backup();
    multiply(-1.0);
    CIATPoint tmp(m_Point);
    recover();
    return tmp;
}

double CIATPoint::operator*(CIATPoint &point) {
    double tmp[3];
    point.Value(tmp);
    return product(tmp);
}

CIATPoint CIATPoint::operator*(const double c) {
    backup();
    multiply(c);
    CIATPoint tmp(m_Point);
    recover();
    return tmp;
}

CIATPoint CIATPoint::operator/(const double c) {
    backup();
    multiply(1 / c);
    CIATPoint tmp(m_Point);
    recover();
    return tmp;
}

CIATPoint CIATPoint::operator*=(const double c) {
    multiply(c);
    return *this;
}

double &CIATPoint::operator[](uint32_t i) {
    if (i < 3) {
        return m_Point[i];
    } else {
        return m_Point[0];
    }
}

void CIATPoint::Translation(double offset[3]) { plus(offset); }

void CIATPoint::Scale(double factor) { multiply(factor); }

void CIATPoint::Rotation(double angle, int axis) {
    switch (axis) {
        case 0: {
            double rot_mat[3][3] = {{1, 0, 0}, {0, cos(angle), sin(angle)}, {0, -sin(angle), cos(angle)}};
            multiply(rot_mat);
        } break;
        case 1: {
            double rot_mat[3][3] = {{cos(angle), 0, -sin(angle)}, {0, 1, 0}, {sin(angle), 0, cos(angle)}};
            multiply(rot_mat);
        } break;
        case 2: {
            double rot_mat[3][3] = {{cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1}};
            multiply(rot_mat);
        } break;
        default:
            break;
    }
}

double CIATPoint::YawRad() {
    double PointXZ[3] = {m_Point[0], 0, m_Point[2]};
    double UnitVecX[3] = {1, 0, 0};
    double YawVec = LkAngle3D(PointXZ, UnitVecX);
    if (PointXZ[2] > 0) { YawVec = 2 * M_PI - YawVec; }
    return YawVec;
}

double CIATPoint::PitchRad() {
    double UnitVecXZ[3] = {m_Point[0], 0, m_Point[2]};
    double PitchVec = angle(UnitVecXZ);
    if (m_Point[1] < 0) { PitchVec = 2 * M_PI - PitchVec; }
    return PitchVec;
}

double CIATPoint::Norm(uint16_t order) {
    if (order == 2) {
        return norm();
    } else {
        double tmp = 0;
        for (double ele : m_Point) { tmp += pow(ele, order); }
        return pow(tmp, 1.0 / order);
    }
}

CIATPoint CIATPoint::CrossProduct(CIATPoint p) {
    double tmp_p[3];
    p.Value(tmp_p);
    double tmp_res[3];
    cross_product(tmp_p, tmp_res);
    return CIATPoint(tmp_res);
}

double CIATPoint::CosAngle(double p[3]) {
    double tmp_aPb = product(p);
    double tmp_cosAngle = tmp_aPb / (Norm() * LkNorm(p, 3));
    return tmp_cosAngle;
}

double CIATPoint::CosAngle(CIATPoint p) {
    double tmp_p[3];
    p.Value(tmp_p);
    double tmp_aPb = product(tmp_p);
    double tmp_cosAngle = tmp_aPb / (Norm() * p.Norm());
    return tmp_cosAngle;
}

void CIATPoint::plus(const double offset[3]) {
    for (int i = 0; i < 3; ++i) m_Point[i] += offset[i];
}

void CIATPoint::multiply(double f) {
    for (double &ele : m_Point) { ele *= f; }
}

void CIATPoint::cross_product(const double b[3], double c[]) {
    double l = m_Point[0], m = m_Point[1], n = m_Point[2];
    double o = b[0], p = b[1], q = b[2];
    double tmp[3] = {m * q - n * p, n * o - l * q, l * p - m * o};
    memcpy(c, tmp, 3 * sizeof(double));
}

double CIATPoint::product(const double p[3]) {
    double res = 0;
    for (int i = 0; i < 3; ++i) res += p[i] * m_Point[i];
    return res;
}

double CIATPoint::norm() {
    double res = 0;
    for (double ele : m_Point) res += pow(ele, 2);
    res = sqrt(res);
    return res;
}

double CIATPoint::norm(double p[3]) {
    double res = 0;
    for (int i = 0; i < 3; ++i) res += pow(p[i], 2);
    res = sqrt(res);
    return res;
}

double CIATPoint::angle(double p[3]) {
    double cos_angle = product(p) / (norm(p) * norm());
    if (cos_angle > 1.0) {
        cos_angle = 1.0;
    } else if (cos_angle < -1.0) {
        cos_angle = -1.0;
    }
    return acos(cos_angle);
}

void CIATPoint::Value(double *val) { memcpy(val, m_Point, 3 * sizeof(double)); }
void CIATPoint::backup() { memcpy(m_PointBk, m_Point, 3 * sizeof(double)); }
void CIATPoint::recover() { memcpy(m_Point, m_PointBk, 3 * sizeof(double)); }

void CIATPoint::multiply(double p[3][3]) {
    double res[3] = {0.0};
    for (int i = 0; i < 3; ++i) {
        double element = 0;
        for (int j = 0; j < 3; ++j) { element += p[i][j] * m_Point[j]; }
        res[i] = element;
    }
    memcpy(m_Point, res, 3 * sizeof(double));
}
