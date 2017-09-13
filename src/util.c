/*******************************************************************************
 * NAME:	    util.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    This file contains some basic utilities which are useful.
 *
 * CREATED:	    08/21/2017
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

#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <string.h>

#include "linkedlist.h"
#include "util.h"

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ***/

static size_t remove_comments(char ** string, size_t size);

/*******************************************************************************
 * API FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    read_tuples_csv
 *
 * DESCRIPTION:	    Reads tuples in the form \d+,\d+,... from the file with
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
gsl_matrix * read_tuples_csv(const char * filename, size_t size)
{
  FILE * file;
  if ((file = fopen(filename, "r")) == NULL || size <= 0)
    return NULL;

  List * list = malloc(sizeof(List));
  list_init(list, free);

  double * arr;
  char * line = NULL;
  size_t n = 0;
  while (getline(&line, &n, file) != -1) {

    if (remove_comments((char **)&line, n) <= 0) continue;
    if ((arr = calloc(size, sizeof(double))) == NULL) goto error_exit;
    char * scratch;
    line = strtok_r(line, ",", &scratch);

    int i = 0;
    do {
      int ret = sscanf(line, "%lf", &arr[i]);
      if (ret <= 0) fprintf(stderr, "Something happened:\nline:\t%s", line);
      i++;
    } while ((line = strtok_r(NULL, ",", &scratch)) != NULL && i < size);

    line = NULL;
    if (list_insnxt(list, list_tail(list), arr) != 0)
      goto error_exit;
  }
  fclose(file);
  file = NULL;

  gsl_matrix * matrix = gsl_matrix_alloc(list_size(list), size);
  int i = 0;
  while (list_size(list) > 0) {
    double * vector;
    list_remnxt(list, NULL, (void **)&vector);
    for (int j = 0; j < size; j++) {
      gsl_matrix_set(matrix, i, j, vector[j]);
    }
    i++;
  }

  list_dest(list);
  free(list);

  return matrix;

 error_exit: {
    if (file != NULL) fclose(file);
    list_dest(list);
    free(arr);
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

/*******************************************************************************
 * STATIC FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    remove_comments
 *
 * DESCRIPTION:	    Find and remove any '#' delimited comments in the .csv file.
 *		    Removes them by truncating the string with a '\0' char.
 *
 * ARGUMENTS:	    string: (char **) -- the string to gnaw.
 *		    size: (size_t) -- the size of the string.
 *
 * RETURN:	    size_t -- the size of the new string. Returns -1 if there is
 *		    an error, 0 if '#' is the first character.
 *
 * NOTES:	    Does not manage storage.
 ***/
static size_t remove_comments(char ** string, size_t size)
{
  if (string == NULL || size == 0)
    return -1;
  if (*string == NULL)
    return -1;

  char * new = memchr(*string, '#', size);
  if (new == NULL) return size;

  size_t newsize = (size_t)(new - *string);
  return newsize;
}

/******************************************************************************/
