#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stdlib.h>

#define GRAPH_WIDTH 20
#define GRAPH_HEIGHT 10

typedef struct {
	size_t count;
	float data[128];
} floats;

floats floats_ln (floats xs);
float floats_sum_values (floats x);
floats floats_sum (floats x, floats y);
floats floats_sum_float (floats x, float y);
floats floats_mul_float (floats x, float y);
floats floats_mul (floats x, floats y);
float floats_dot (floats x, floats y);

typedef struct {
	float m,
	      b,
	      r,
	      SSe,
	      SSm,
	      SSb;
} lin_reg_t;	

lin_reg_t lin_reg (floats x, floats y);
float map (float xi, float xf, float yi, float yf, float x);
void term_graph (floats x, floats y, float width, float height);

#endif
