#include <stdio.h>
#include <math.h>

typedef struct {
	size_t count;
	float data[128];
} floats;

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

floats floats_mul (floats x, floats y) {
        floats mul = { .count = x.count > y.count ? y.count : x.count };
        for (size_t i = 0; i < mul.count; i++)
                mul.data[i] = x.data[i] * y.data[i];
        return mul;
}

float floats_dot (floats x, floats y) {
	return floats_sum_values(floats_mul(x, y));
}

typedef struct {
	float m,
	      b,
	      r,
	      SSe,
	      SSm,
	      SSb;
} lin_reg_t;	

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

void term_graph (floats x, floats y) {
	char screen[100*40];
	for (size_t i = 0; i < 100*40; i++)
		screen[i] = ' ';

	for (size_t i = 0; i < x.count; i++)
		screen[(int)(map(0, 6000, 0, 100, x.data[i])) + 
		       (int)(map(0, 90, 40, 0, y.data[i])) * 100] = '*';
	for (size_t i = 0; i < 40; i++) {
		printf("%.100s\n", &screen[100*i]);
	}
}

int main (void) {
	floats temp = {0}, time = {0};
	FILE *temp_file = fopen("res/temperaturas.csv", "r"),
	     *time_file = fopen("res/tiempos.csv", "r");
	while ( !feof(temp_file) || !feof(time_file) ) {
		fscanf(temp_file, "%f\n", &temp.data[temp.count++]);
		fscanf(time_file, "%f\n", &time.data[time.count++]);
	}
	fclose(temp_file);
	fclose(time_file);
	floats ln_temp = floats_ln(temp);
	lin_reg_t rl = lin_reg(time, ln_temp);
	printf(
		"##############\n"
		"# RESULTADOS #\n"
		"##############\n"
		"Y = (%f+-%f) x + (%f+-%f), r = %f\n",
		rl.m, sqrt(rl.SSm),
		rl.b, sqrt(rl.SSb),
		rl.r
	);
	printf(
		"#############################\n"
		"# Temperagura contra tiempo #\n"
		"#############################\n"
	),
	term_graph(time, temp);
	return 0;
}
