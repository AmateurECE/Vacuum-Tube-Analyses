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
#include "gsl/gsl_matrix.h"

#include "config.h"
#include "util.h"

/*******************************************************************************
 * MAIN
 ***/

extern char etext, edata, end;

int main(int argc, char * argv[]) {
  gsl_matrix * matrix;

  matrix = read_tuples_csv(argv[1], strtol(argv[2], NULL, 10));
  
  printf("Program text segment: %p\n", &etext);
  printf("Program data segment: %p\n", &edata);
  printf("Program bss segment:  %p\n", &end);

  for (int i = 0; i < matrix->size1; i++) {
    printf("[");
    for (int j = 0; j < matrix->size2; j++) {
      printf(" %f ", gsl_matrix_get(matrix, i, j));
    }
    printf("]\n");
  }

}

/******************************************************************************/
