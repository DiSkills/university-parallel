#ifndef FIELD_H
#define FIELD_H

bool field_is_solved(int **field, int size);
int **field_input(int size);
void field_del(int **field, int size);
void field_print(int **field, int size);

void field_solve(int **field, int size, int start, int stop);

#endif
