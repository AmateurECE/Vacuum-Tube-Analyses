/*******************************************************************************
 * NAME:	    fit.h
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    This file contains the public interface for the functions
 *		    in fit.c
 *
 * CREATED:	    08/22/2017
 *
 * LAST EDITED:	    08/22/2017
 ***/

/*******************************************************************************
 * INCLUDES
 ***/

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/*******************************************************************************
 * TYPE DEFINITIONS
 ***/

typedef struct fit_data {
  gsl_vector * coefficient_vector;
} fit_data_t;

/*******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

extern int surface_f(const gsl_vector * x, void * data, gsl_vector * f);
extern int surface_df(const gsl_vector * x, void * data, gsl_matrix * J);
extern fit_data_t * fit_surface(gsl_matrix * data);

/******************************************************************************/
