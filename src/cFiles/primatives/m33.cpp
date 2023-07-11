#include "m33.h"
#include <math.h>
#include <istream>
#include <iostream>

M33::M33(){
	rows[0] = V3(0,0,0);
	rows[1] = V3(0,0,0);
	rows[2] = V3(0,0,0);
}

M33::M33(V3 r0, V3 r1, V3 r2){
	rows[0] = r0;
	rows[1] = r1;
	rows[2] = r2;
}

void M33::setColumn(int col, V3 data){
	if((col > 2) || (col < 0)){
		printf("M33.setColumn incorrect column number!\n");
		return;
	}
	rows[0][col] = data[0];
	rows[1][col] = data[1];
	rows[2][col] = data[2];
}

V3 M33::getColumn(int col){
	if((col > 2) || (col < 0)){
		printf("M33.setColumn incorrect column number!\n");
		return V3(0,0,0);
	}
	return V3(rows[0][col], rows[1][col], rows[2][col]);
}

void M33::setRow(int row, V3 data){
	if((row > 2) || (row < 0)){
		printf("M33.setRow incorrect row number!\n");
		return;
	}
	rows[row] = data;
}
void M33::setAsRotationAboutAxis(char axis, float angle){
	float angleRad = angle/180.0f * M_PI;
	if(axis == 'z'){
		rows[0] = V3(cos(angleRad),-sin(angleRad),0);
		rows[1] = V3(sin(angleRad),cos(angleRad),0);
		rows[2] = V3(0,0,1);
	}else if(axis == 'x'){
		rows[0] = V3(1,0,0);
		rows[1] = V3(0,cos(angleRad),-sin(angleRad));
		rows[2] = V3(0,sin(angleRad),cos(angleRad));
	}else if(axis == 'y'){
		rows[0] = V3(cos(angleRad), 0, sin(angleRad));
		rows[1] = V3(0, 1, 0);
		rows[2] = V3(-sin(angleRad), 0, cos(angleRad));
	}else{
		printf("M33::setAsRotationAboutAxis: Invalid axis\n");
	}
}

V3 M33::operator*(V3 v) {
	M33& m = *this;
	V3 ret;
	ret[0] = m[0] * v;
	ret[1] = m[1] * v;
	ret[2] = m[2] * v;
	return ret;
}

M33 M33::operator*(float f) {
	M33& m = *this;
	m[0] = m[0] * f;
	m[1] = m[1] * f;
	m[2] = m[2] * f;
	return m;
}

V3& M33::operator[](int i) {
	return rows[i];
}

M33 M33::operator*(M33 m1) {
	M33 ret;
	M33 &m0 = *this;
	ret.setColumn(0, m0*m1.getColumn(0));
	ret.setColumn(1, m0*m1.getColumn(1));
	ret.setColumn(2, m0*m1.getColumn(2));
	return ret;
}

std::istream& operator>>(std::istream& istr, M33& m) {
	std::cerr << "Column 1: " << std::endl;
	std::cin >> m[0];
	std::cerr << "Column 2: " << std::endl;
	std::cin >> m[1];
	std::cerr << "Column 3: " << std::endl;
	std::cin >> m[2];
	return istr;
}
