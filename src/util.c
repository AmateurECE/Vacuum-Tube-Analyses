/*******************************************************************************
 * NAME:	    util.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    This file contains some basic utilities which are useful.
 *
 * CREATED:	    08/21/2017
 *
 * LAST EDITED:	    08/21/2017
 ***/

/*******************************************************************************
 * INCLUDES
 ***/

#include <stdlib.h>

#include "util.h"

/*******************************************************************************
 * API FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    read_tuples
 *
 * DESCRIPTION:	    Reads tuples in the form (\d+,\d+,...) from the file with
 *		    the name <filename> of size <n>.
 *
 * ARGUMENTS:	    filename: (const char *) -- the name of the file to read.
 *		    n: (size_t) -- the size of the tuple to read.
 *
 * RETURN:	    double * -- pointer to an array of doubles created from
 *		    reading the tuples in, or NULL if there was an error.
 *
 * NOTES:	    none.
 ***/
double * read_tuples(const char * filename, size_t n)
{
  FILE * file;
  if ((file = fopen(filename, "r")) == NULL || n <= 0)
    return NULL;

  char * mould = "(%f";
  for (int i = 1; i < n; i++) {
    strcat(mould, ",%f");
  }
  strcat(mould, ")");

  double * tuples = calloc(10, sizeof(double));
  while (!feof(file)) {
    
  }

  fclose(file);
}

/******************************************************************************/
