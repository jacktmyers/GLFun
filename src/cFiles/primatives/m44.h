#pragma once

#include "v4.h"
#include "v3.h"

class M44{
	public:
		// Variables
		// Functions
		M44();
		void setColumn(int,V4);
		void setRow(int,V4);
		void setAsTranslation(V3 vec);
	private:
		// Variables
		V4 rows[4];
		// Functions
};
