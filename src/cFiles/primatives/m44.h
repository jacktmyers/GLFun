#pragma once

#include "v4.h"
#include "v3.h"

class M44{
	public:
		// Variables
		V4 rows[4];
		// Functions
		M44();
		M44(V4 x,V4 y,V4 z,V4 w);
		M44(float* array);
		void setColumn(int,V4);
		void setRow(int,V4);
		void setAsTranslation(V3 vec);
	private:
		// Variables
		// Functions
};
