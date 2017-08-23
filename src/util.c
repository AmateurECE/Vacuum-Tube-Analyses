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

#include "linkedlist.h"
#include "util.h"

/*******************************************************************************
 * API FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    read_tuples_csv
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
gsl_matrix * read_tuples_csv(const char * filename, size_t n)
{
  FILE * file;
  if ((file = fopen(filename, "r")) == NULL || n <= 0)
    return NULL;

  List * list = malloc(sizeof(List));
  list_init(list, free);

  while (!feof(file)) {
    double * arr = calloc(n, sizeof(double));
    for (int i = 0; i < n; i++) {
      fscanf(file, "%lf,", &arr[i]);
    }
    if (list_insnxt(list, list_tail(list), arr) != 0) {
      free(arr);
      goto error_exit;
    }
  }
  fclose(file);

  gsl_matrix * matrix = gsl_matrix_alloc(list_size(list), n);
  int i = 0;
  while (list_size(list) > 0) {
    double * vector;
    list_remnxt(list, NULL, (void **)&vector);
    for (int j = 0; j < n; j++) {
      gsl_matrix_set(matrix, i, j, vector[j]);
    }
    i++;
  }

  list_dest(list);
  free(list);

  return matrix;

 error_exit: {
    fclose(file);
    list_dest(list);
    free(list);
    return NULL;
  }
}

/*******************************************************************************
 * FUNCTION:	    read_tuples_xml
 *
 * DESCRIPTION:	    Read tuples from an XML file in the format:
 *
 *			<tuple>
 *			    <dim val=[value]/>
 *			    ...
 *			</tuple>
 *			...
 *
 *		    This function is mostly for my ease of use. XML is a lot
 *		    easier to read than CSV.
 *
 * ARGUMENTS:	    filename: (const char *) -- the path of the XML file.
 *		    n: (size_t) -- the size of the tuple to read.
 *
 * RETURN:	    gsl_matrix or NULL if unsuccessful.
 *
 * NOTES:	    none.
 ***/
gsl_matrix * read_tuples_xml(const char * filename, size_t n) {
  /* TODO: Implement this. */
  return NULL;
}

/******************************************************************************/
