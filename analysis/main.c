#include <stdio.h>
#include "formater.h"
#include "analysis.h"
#include <math.h>

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
	table_append(&tbl, "ycol", "temperatura");
	//table_append_float(&tbl, "pendiente", rl.m, -log10(rl.SSm)/2);
	//table_append_float(&tbl, "des_pendiente", sqrt(rl.SSm), -log10(rl.SSm));
	
	formater_file("analysis/grafica.fmt", "src/grafica_1.tex", &tbl);

	table_define(&tbl, "xcol", "tiempo");
	table_define(&tbl, "ycol", "ln_temperatura");
	formater_file("analysis/grafica.fmt", "src/grafica_2.tex", &tbl);

	return 0;
}
