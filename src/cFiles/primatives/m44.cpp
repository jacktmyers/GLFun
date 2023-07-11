#include "m44.h"

M44::M44(){
	rows[0] = V4(0,0,0,0);
	rows[1] = V4(0,0,0,0);
	rows[2] = V4(0,0,0,0);
	rows[3] = V4(0,0,0,0);
}
void M44::setColumn(int col, V4 data){
	if((col > 3) || (col < 0)){
		printf("M44.setColumn incorrect column number!\n");
		return;
	}
	rows[0][col] = data[0];
	rows[1][col] = data[1];
	rows[2][col] = data[2];
	rows[3][col] = data[3];
}
void M44::setRow(int row, V4 data){
	if((row > 3) || (row < 0)){
		printf("M44.setRow incorrect row number!\n");
		return;
	}
	rows[row] = data;
}
void M44::setAsTranslation(V3 vec){
	rows[0] = V4(1.0f,.0f,.0f,vec[0]);
	rows[1] = V4(.0f,1.0f,.0f,vec[1]);
	rows[2] = V4(.0f,.0f,1.0f,vec[2]);
	rows[3] = V4(.0f,.0f,.0f,1.0f);
}
