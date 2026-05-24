#include "analysis.h"

#include <math.h>

floats floats_ln (floats xs) {
	floats res = { .count = xs.count };
	for (size_t i = 0; i < xs.count; i++)
		res.data[i] = log(xs.data[i]);
	return res;
}

float floats_sum_values (floats x) {
	float sum = 0;
	for (size_t i = 0; i < x.count; i++)
		sum += x.data[i];
	return sum;
}

floats floats_sum (floats x, floats y) {
        floats sum = { .count = x.count > y.count ? y.count : x.count };
        for (size_t i = 0; i < sum.count; i++)
                sum.data[i] = x.data[i] + y.data[i];
        return sum;
}

floats floats_sum_float (floats x, float y) {
        floats sum = { .count = x.count };
        for (size_t i = 0; i < sum.count; i++)
                sum.data[i] = x.data[i] + y;
        return sum;
}

floats floats_mul_float (floats x, float y) {
        floats mul = { .count = x.count };
        for (size_t i = 0; i < mul.count; i++)
                mul.data[i] = x.data[i] * y;
        return mul;
}

floats floats_mul (floats x, floats y) {
        floats mul = { .count = x.count > y.count ? y.count : x.count };
        for (size_t i = 0; i < mul.count; i++)
                mul.data[i] = x.data[i] * y.data[i];
        return mul;
}

float floats_dot (floats x, floats y) {
	return floats_sum_values(floats_mul(x, y));
}

lin_reg_t lin_reg (floats x, floats y) {
	size_t count = x.count > y.count ? y.count : x.count;
	float X = floats_sum_values(x) / count,
	      Y = floats_sum_values(y) / count,
	      XX = floats_dot(x, x) / count,
	      XY = floats_dot(x, y) / count,
	      YY = floats_dot(y, y) / count,
	      SSX = XX - X * X,
 	     SSY = YY - Y * Y,
	      SXY = XY - X * Y;
	lin_reg_t lr = {
		.m = (XY - X * Y) / (XX - X * X),
        	.b = (XX * Y - X * XY) / (XX - X * X),
		.r = (SXY * SXY) / (SSX * SSY),
        	.SSe = SSY - lr.m * lr.m * SSX,
        	.SSm = lr.SSe / SSX,
        	.SSb = lr.SSm * XX
	};
	return lr;
}	

float map (float xi, float xf, float yi, float yf, float x) {
	return yi + (yf - yi) / (xf - xi) * (x - xi);
}

void term_graph (floats x, floats y, float width, float height) {
	char screen[GRAPH_WIDTH * GRAPH_HEIGHT];
	for (size_t i = 0; i < GRAPH_WIDTH * GRAPH_HEIGHT; i++)
		screen[i] = ' ';

	for (size_t i = 0; i < x.count; i++)
		screen[(int)(map(0, width, 0, GRAPH_WIDTH, x.data[i])) +
		       (int)(map(0, height, GRAPH_HEIGHT, 0, y.data[i])) * GRAPH_WIDTH] = '*';
	for (size_t i = 0; i < GRAPH_HEIGHT; i++) {
		printf("%.*s\n", GRAPH_WIDTH, &screen[GRAPH_WIDTH*i]);
	}
}
