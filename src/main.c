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
 * MAIN
 ***/

int main(int argc, char * argv[]) {
  gsl_matrix * matrix;

  matrix = read_tuples_csv(argv[1], strtol(argv[2], NULL, 10));

  double init[5] = {5.0, 5.0, 5.0, 5.0, 5.0};

  fit_data_t * dat = malloc(sizeof(fit_data_t));
  dat->empirical_data = matrix;
  dat->initial_values = &init;

  fit_surface(dat, true);
}

/******************************************************************************/
