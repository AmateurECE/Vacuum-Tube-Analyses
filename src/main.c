/*******************************************************************************
 * NAME:	    main.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Main test file for the program.
 *
 * CREATED:	    08/22/2017
 *
 * LAST EDITED:	    08/22/2017
 ***/

/*******************************************************************************
 * INCLUDES
 ***/

#include <stdio.h>
#include <gsl/gsl_matrix.h>

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
