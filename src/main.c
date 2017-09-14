/*******************************************************************************
 * NAME:	    main.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Main test file for the program.
 *
 * CREATED:	    08/22/2017
 *
 * LAST EDITED:	    08/25/2017
 *
 * Copyright 2017, Ethan D. Twardy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

/*******************************************************************************
 * INCLUDES
 ***/

#include <stdio.h>
#include <stdbool.h>
#include <gsl/gsl_matrix.h>

#include "util.h"
#include "fit.h"

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ***/

static void print_matrix(gsl_matrix * matrix, FILE * log);

/*******************************************************************************
 * MAIN
 ***/

int main(int argc, char * argv[]) {
  gsl_matrix * matrix = read_tuples_csv("data/12AX7-Data.csv", 3);
  FILE * fitlog = fopen("test.log", "w");
  print_matrix(matrix, fitlog);

  double init[5] = {1.0, 1.0, 1.0, 1.0, 1.0};
  fit_data_t * dat = malloc(sizeof(fit_data_t));
  dat->empirical_data = matrix;
  dat->initial_values = init;
  fit_surface(dat, true, fitlog);
  plot(dat, true);
  fclose(fitlog);

  gsl_matrix_free(matrix);
}

/*******************************************************************************
 * STATIC FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    print_matrix
 *
 * DESCRIPTION:	    Pretty simple. Prints a gsl_matrix structure.
 *
 * ARGUMENTS:	    matrix: (gsl_matrix *) -- the matrix to print.
 *
 * RETURN:	    void.
 *
 * NOTES:	    none
 ***/
static void print_matrix(gsl_matrix * matrix, FILE * log)
{
  for (int i = 0; i < matrix->size1; i++) {
    fprintf(log, "[ ");
    for (int j = 0; j < matrix->size2; j++) {
      fprintf(log, "%g\t", gsl_matrix_get(matrix, i, j));
    }
    fprintf(log, "]\n");
  }
}

/******************************************************************************/
