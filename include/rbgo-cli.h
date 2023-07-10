#pragma once
#include <rbgo-types.h>

namespace rbgo {

class CliMode{
	static void *vp;
public:
	static void initialize();
	static void inputBlock(bool b);
	static void inputEcho(bool b);
	static void inputBlockEcho();
	static void inputNoBlockNoEcho();
	static void outputNoBuf();
	static void restore();
};

}//ns
