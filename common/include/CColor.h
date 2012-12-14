#ifndef CPIXEL_H
#define CPIXEL_H
namespace libconfig {
	class Setting;
}

class CColor {
public:
	double r,g,b;
	CColor();
	static CColor RGB(double r, double g, double b);
	static CColor HSL(double h, double s, double l);
	static CColor FromSetting(libconfig::Setting &s);
	
	CColor operator+(const CColor &rhs);
	CColor operator-(const CColor &rhs);
	CColor operator*(const CColor &rhs);
	CColor operator*(const double rhs);
	CColor operator/(const CColor &rhs);
	CColor operator/(const double rhs);
};
#endif//CPIXEL_H
