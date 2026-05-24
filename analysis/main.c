#include <stdio.h>
#include "formater.h"
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

#define GRAPH_WIDTH 20
#define GRAPH_HEIGHT 10

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

int main (void) {
	floats temp = {0}, time = {0};
	FILE *temp_file = fopen("res/temperaturas.csv", "r"),
	     *time_file = fopen("res/tiempos.csv", "r"),
	     *data_file = fopen("res/data.csv", "w");
	fprintf(data_file, "tiempo,temperatura,ln_temperatura\n");
	while ( !feof(temp_file) || !feof(time_file) ) {
		fscanf(temp_file, "%f\n", &temp.data[temp.count++]);
		fscanf(time_file, "%f\n", &time.data[time.count++]);
	}
	fclose(temp_file);
	fclose(time_file);
	floats temp_rel = floats_sum_float(temp, -25);
	floats ln_temp = floats_ln(temp_rel);
	for (size_t i = 0; i < time.count; i++)
		fprintf(data_file, "%f, %f, %f\n", time.data[i], temp.data[i], ln_temp.data[i]);
	fclose(data_file);
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
	term_graph(time, temp, time.data[time.count - 1] + 1, temp.data[0] + 1);
	printf(
		"######################################\n"
		"# Temperagura relativa contra tiempo #\n"
		"######################################\n"
	),
	term_graph(time, temp_rel, time.data[time.count - 1] + 1, temp_rel.data[0] + 1);
	printf(
		"#################################\n"
		"# Log Temperagura contra tiempo #\n"
		"#################################\n"
	);
	term_graph(time, ln_temp, time.data[time.count - 1] + 1, ln_temp.data[0] + 1);
	printf(
		"##############\n"
		"# RESULTADOS #\n"
		"##############\n"
		"Y = (%f+-%f) x + (%f+-%f), r = %f\n",
		rl.m, sqrt(rl.SSm),
		rl.b, sqrt(rl.SSb),
		rl.r
	);
	table_t tbl;

	table_append(&tbl, "datos", "res/data.csv");
	table_append(&tbl, "xcol", "tiempo");
	table_append(&tbl, "ycol", "ln_temperatura");
	//table_append_float(&tbl, "pendiente", rl.m, -log10(rl.SSm)/2);
	//table_append_float(&tbl, "des_pendiente", sqrt(rl.SSm), -log10(rl.SSm));
	
	formater_file("analysis/grafica.fmt", "src/grafica.tex", &tbl);

	return 0;
}
