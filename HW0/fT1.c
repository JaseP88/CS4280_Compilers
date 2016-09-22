//fT1 source

#include "T.h"
#include "fT1.h"

static int intT1Helper;

void f1t1(T1 t1) {
	T2 t2;

	f1t1Helper();
	intT1Helper = t1;
	g1 = t1;
	t2 = g1;
	//other stuff
}

void f2t1(T1 t1) {
	intT1Helper = 0;
	g2 = t1;
	//stuff
}

//should put this first as a standard  <all static function to top>
static void f1t1Helper() {
	//helper stuff
}