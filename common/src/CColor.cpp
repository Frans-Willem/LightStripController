#include "CColor.h"
#include "CConfigObject.h"

CColor::CColor() {
	r=g=b=0.0;
}

CColor CColor::RGB(double r, double g, double b) {
	CColor ret;
	ret.r = r;
	ret.g = g;
	ret.b = b;
	return ret;
}

double HSLInternal(double temp1,double temp2,double temp3) {
	while (temp3<0.0) {
		temp3+=1.0;
	}
	while (temp3>1.0) {
		temp3-=1.0;
	}
	if (6.0*temp3<1.0) return temp1+((temp2-temp1)*6.0*temp3);
	if (2.0*temp3<1.0) return temp2;
	if (3.0*temp3<2.0) return temp1+((temp2-temp1)*((2.0/3.0)-temp3)*6.0);
	return temp1;
}

CColor CColor::HSL(double h, double s, double l) {
	if (s == 0.0) return RGB(l,l,l);
	double temp1, temp2;
	if (l < 0.5) temp2 = l * (1.0 + s);
	else temp2 = (l + s) - (l * s);
	temp1 = (2.0 * l) - temp2;
	return RGB(HSLInternal(temp1, temp2, h + (1.0/3.0)), HSLInternal(temp1, temp2, h), HSLInternal(temp1, temp2, h - (1.0/3.0)));
}

CColor CColor::FromSetting(CConfigObject *pNode) {
	if (pNode->isArray()) {
		return RGB(pNode->getDouble(0),pNode->getDouble(1),pNode->getDouble(2));
	}
	if (pNode->get("r") || pNode->get("g") || pNode->get("b"))
		return RGB(pNode->getDouble("r"),pNode->getDouble("g"),pNode->getDouble("b"));
	if (pNode->get("h") || pNode->get("s") || pNode->get("l"))
		return HSL(pNode->getDouble("h",0.0), pNode->getDouble("s",1.0), pNode->getDouble("l",0.5));
	return RGB(0.0,0.0,0.0);
}

CColor CColor::operator+(const CColor &rhs) {
	CColor ret;
	ret.r = r + rhs.r;
	ret.g = g + rhs.g;
	ret.b = b + rhs.b;
	return ret;
}
CColor CColor::operator-(const CColor &rhs) {
	CColor ret;
	ret.r = r - rhs.r;
	ret.g = g - rhs.g;
	ret.b = b - rhs.b;
	return ret;
}
CColor CColor::operator*(const CColor &rhs) {
	CColor ret;
	ret.r = r * rhs.r;
	ret.g = g * rhs.g;
	ret.b = b * rhs.b;
	return ret;
}
CColor CColor::operator*(const double rhs) {
	CColor ret;
	ret.r = r * rhs;
	ret.g = g * rhs;
	ret.b = b * rhs;
	return ret;
}
CColor CColor::operator/(const CColor &rhs) {
	CColor ret;
	ret.r = r / rhs.r;
	ret.g = g / rhs.g;
	ret.b = b / rhs.b;
	return ret;
}
CColor CColor::operator/(const double rhs) {
	CColor ret;
	ret.r = r / rhs;
	ret.g = g / rhs;
	ret.b = b / rhs;
	return ret;
}

