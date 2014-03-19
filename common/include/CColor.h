#ifndef CCOLOR_H
#define CCOLOR_H

class CConfigObject;
class CColor {
public:
	double r,g,b;
	CColor();
	static CColor RGB(double r, double g, double b);
	static CColor HSL(double h, double s, double l);
	static CColor FromSetting(CConfigObject *pNode);
	
	CColor operator+(const CColor &rhs);
	CColor operator-(const CColor &rhs);
	CColor operator*(const CColor &rhs);
	CColor operator*(const double rhs);
	CColor operator/(const CColor &rhs);
	CColor operator/(const double rhs);
};
#endif//CCOLOR_H
