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
#include <gsl/gsl_matrix.h>

#include "util.h"

/*******************************************************************************
 * MACRO DEFINITIONS
 ***/

#define DEFINE_FORMAT_STRUCT(n, form)			\
  typedef struct {					\
    short n; /* Number of values in the tuple */	\
    char format[] = form; /* Format string */		\
    double tuple[n]; /* Size for one tuple. */		\
  } format##n##_t;

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
 * RETURN:	    gsl_matrix * -- pointer to a matrix of doubles created from
 *		    reading the tuples in, or NULL if there was an error.
 *
 * NOTES:	    none.
 ***/
gsl_matrix * read_tuples(const char * filename, size_t n)
{
  FILE * file;
  if ((file = fopen(filename, "r")) == NULL || n <= 0)
    return NULL;

  char * mould = "(%f";
  for (int i = 1; i < n; i++) {
    strcat(mould, ",%f");
  }
  strcat(mould, ")");

  DEFINE_FORMAT_STRUCT(n,)

  gsl_matrix * matrix = gsl_matrix_alloc()
  while (!feof(file)) {
    
  }

  fclose(file);
}

/******************************************************************************/
