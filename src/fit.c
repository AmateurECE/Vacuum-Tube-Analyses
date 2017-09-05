/*******************************************************************************
 * NAME:	    fit.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    This file contains the code that performs the multiple
 *		    polynomial regression.
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
#include <gsl/gsl_multifit_nlinear.h>

#include "fit.h"

/*******************************************************************************
 * API FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    surface_f
 *
 * DESCRIPTION:	    The surface equation. This function, given a vector of
 *		    coefficients, returns a coordinate on the z-axis that
 *		    corresponds to a point on the surface. The function is
 *		    defined as:
 *
 *			f(x,y)_i = B_0x + B_1x^2 + B_2y + B_3y^2 + B_4
 *
 * ARGUMENTS:	    x: (const gsl_vector *) -- vector of coefficients to test
 *		    data: (void *) -- pointer to a struct containing empirical
 *			data (void for type consistency with gsl).
 *		    f: (gsl_vector *) -- pointer to a vector of size n, which
 *			will contain the computed values for the vector x.
 *
 * RETURN:	    GSL_SUCCESS -- There's virtually no way that the function
 *			will fail, so returning a different value is pointless.
 *
 * NOTES:	    none.
 ***/
int surface_f(const gsl_vector * x, void * data, gsl_vector * f)
{
  gsl_matrix * values = ((fit_data_t *)data)->empirical_data;
  size_t n = values->size1;
  gsl_vector_view Ig = gsl_matrix_column(values, 0);
  gsl_vector_view Eg = gsl_matrix_column(values, 1);
  gsl_vector_view Ep = gsl_matrix_column(values, 2);

  double b0 = gsl_vector_get(x, 0);
  double b1 = gsl_vector_get(x, 1);
  double b2 = gsl_vector_get(x, 2);
  double b3 = gsl_vector_get(x, 3);
  double b4 = gsl_vector_get(x, 4);

  for (size_t i = 0; i < n; i++) {
    double yi = (b0 * gsl_vector_get(&Eg.vector, i))
      + (b1 * gsl_vector_get(&Ep.vector, i))
      + (b2 * pow(gsl_vector_get(&Eg.vector, i), 2.0))
      + (b3 * pow(gsl_vector_get(&Ep.vector, i), 2.0))
      + b4;
    gsl_vector_set(f, i, gsl_vector_get(&Ig.vector, i) - yi);
  }
  
  return GSL_SUCCESS;
}

/*******************************************************************************
 * FUNCTION:	    surface_df
 *
 * DESCRIPTION:	    Compute the value of the Jacobian matrix for coefficient
 *		    vector 'x' and parameters 'data'.
 *
 * ARGUMENTS:	    x: (const gsl_vector *) -- coefficient vector given by gsl.
 *		    data: (void *) -- empirical data struct (void for type 
 *			consistency with gsl).
 *		    J: (gsl_matrix *) -- pointer to the allocated matrix with
 *			which to fill the result of the Jacobian computation.
 *
 * RETURN:	    GSL_SUCCESS -- unnecessary to implement an error catching
 *		    feature since the jacobian entries are continuous at all
 *		    points, no memory allocation, etc.
 *
 * NOTES:	    
 ***/
int surface_df(const gsl_vector * x, void * data, gsl_matrix * J)
{
  gsl_matrix * values = ((fit_data_t *)data)->empirical_data;
  size_t n = values->size1;
  gsl_vector_view Eg = gsl_matrix_column(values, 1);
  gsl_vector_view Ep = gsl_matrix_column(values, 2);

  double b0 = gsl_vector_get(x, 0);
  double b1 = gsl_vector_get(x, 1);
  double b2 = gsl_vector_get(x, 2);
  double b3 = gsl_vector_get(x, 3);

  for (size_t i = 0; i < n; i++) {
    double y1 = b0 + (2 * b2 * gsl_vector_get(&Eg.vector, i));
    double y2 = b1 + (2 * b3 * gsl_vector_get(&Ep.vector, i));
    gsl_matrix_set(J, i, 0, y1);
    gsl_matrix_set(J, i, 1, y2);
  }

  return GSL_SUCCESS;
}

/*******************************************************************************
 * FUNCTION:	    callback
 *
 * DESCRIPTION:	    
 *
 * ARGUMENTS:	    
 *
 * RETURN:	    
 *
 * NOTES:	    
 ***/
void callback(const size_t iter,
	      void * params,
	      const gsl_multifit_nlinear_workspace * w)
{
  gsl_vector * x = gsl_multifit_nlinear_position(w);
  printf("iter %2zu: Y = %fEg + %fEp + %fEg^2 + %fEp^2 + %f\n",
	 iter,
	 gsl_vector_get(x, 0),
	 gsl_vector_get(x, 1),
	 gsl_vector_get(x, 2),
	 gsl_vector_get(x, 3),
	 gsl_vector_get(x, 4));
}

/*******************************************************************************
 * FUNCTION:	    fit_surface
 *
 * DESCRIPTION:	    Uses the GSL to perform a multiple polynomial regression
 *		    with the TRS method using 'data'.
 *
 * ARGUMENTS:	    data: (fit_data_t *) -- struct containing the data to fit.
 *
 * RETURN:	    (fit_data_t *) -- pointer to the same struct that is passed,
 *		    sets the relevant fields.
 *
 * NOTES:	    none.
 ***/
fit_data_t * fit_surface(fit_data_t * data, bool call)
{
  /* Define constants for fitting */
  const double xtol = 1e-8; /* Step tolerance */
  const double gtol = 1e-8; /* Gradient tolerance */
  const double ftol = 0.0;   /* ??? */
  gsl_vector_view view = gsl_vector_view_array(data->initial_values,
					       data->empirical_data->size2);

  /* Use the default parameters */
  gsl_multifit_nlinear_parameters params =
    gsl_multifit_nlinear_default_parameters();

  /* Initialize fdf structure */
  gsl_multifit_nlinear_fdf fdf = (gsl_multifit_nlinear_fdf){
    .f = surface_f,
    .df = surface_df,
    .fvv = NULL,
    .n = data->empirical_data->size1,
    .p = data->empirical_data->size2,
    .params = data
  };

  /* Allocate the solver */
  gsl_multifit_nlinear_workspace * w =
    gsl_multifit_nlinear_alloc(gsl_multifit_nlinear_trust,
			       &params,
			       data->empirical_data->size1,
			       data->empirical_data->size2);

  /* Initialize the solver */
  gsl_multifit_nlinear_winit(&view.vector, NULL, &fdf, w);

  int info, status;
  status = gsl_multifit_nlinear_driver(20, xtol, gtol, ftol,
				       callback, NULL, &info, w);

  printf("status = %s\ncallback = %d\n", gsl_strerror(status), (int)call);
  
  return NULL;
}

/******************************************************************************/
