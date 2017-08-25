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

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit_nlin.h>

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
 *			Y_i = B_0x + B_1x^2 + B_2y + B_3y^2 + B_4
 *
 * ARGUMENTS:	    
 *
 * RETURN:	    
 *
 * NOTES:	    
 ***/
int surface_f(const gsl_vector * x, void * data, gsl_vector * f)
{
  
  return GSL_SUCCESS;
}

/*******************************************************************************
 * FUNCTION:	    surface_df
 *
 * DESCRIPTION:	    
 *
 * ARGUMENTS:	    
 *
 * RETURN:	    
 *
 * NOTES:	    
 ***/
int surface_df(const gsl_vector * x, void * data, gsl_matrix * J)
{
  return GSL_SUCCESS;
}

/*******************************************************************************
 * FUNCTION:	    fit_surface
 *
 * DESCRIPTION:	    
 *
 * ARGUMENTS:	    
 *
 * RETURN:	    
 *
 * NOTES:	    
 ***/
fit_data_t * fit_surface(gsl_matrix * data)
{
  return NULL;
}

/******************************************************************************/
