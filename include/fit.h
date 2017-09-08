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

#include <stdbool.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/*******************************************************************************
 * TYPE DEFINITIONS
 ***/

typedef struct fit_param {
  double value;
  double error;
} fit_param_t;

typedef struct fit_data {
  fit_param_t * coefficients;
  double * initial_values;
  gsl_matrix * empirical_data;
} fit_data_t;

/*******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

extern int surface_f(const gsl_vector * x, void * data, gsl_vector * f);
/* This function is currently still in development. */
/* extern int surface_df(const gsl_vector * x, void * data, gsl_matrix * J); */
extern int fit_surface(fit_data_t * data, bool callback, FILE * outfh);
extern int plot(fit_data_t * data, bool png_output);

/******************************************************************************/
