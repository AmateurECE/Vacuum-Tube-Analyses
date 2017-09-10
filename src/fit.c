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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_blas.h>
#include <unistd.h>

#include "gnuplot_i/gnuplot_i.h"
#include "fit.h"

/*******************************************************************************
 * MACRO DEFINITIONS
 ***/

#define COMMAND_PNG_OUTPUT			\
  "set terminal png; "				\
  "set output 'surface.png'; "

#define COMMAND_SCRIPT							\
  "set title 'Multiple Polynomial Regression of Triode Characteristics'; " \
  "set xlabel 'Plate Voltage, Ep (V)'; "				\
  "set ylabel 'Grid Voltage, Eg (V)'; "					\
  "set zlabel 'Plate Current, Ip (V)' rotate parallel; "		\
  "set zrange [-1:8]; "							\
  "%s"									\
  "%s"

#define COMMAND_FN				\
  "f(x,y) = %2.4g*y + %2.4g*x + %2.4g*y**2 + %2.4g*x**2 + %2.4g; "

#define COMMAND_PLOT				\
  "splot '%s' using 1:2:3, f(x,y); "

/*******************************************************************************
 * GLOBAL VARIABLES
 ***/

static FILE * surface_log;

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ***/

static int print_to_log(gsl_multifit_nlinear_workspace * w,
			gsl_multifit_nlinear_fdf * fdf,
			int info,
			int status,
			gsl_matrix * covar,
			double chisq0,
			double chisq1,
			size_t n,
			size_t p);
static int tmp_write_data(fit_data_t * fit_data, FILE * tmpfd);

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
  gsl_vector_view Ig = gsl_matrix_column(values, 2);
  gsl_vector_view Eg = gsl_matrix_column(values, 1);
  gsl_vector_view Ep = gsl_matrix_column(values, 0);

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
 * NOTES:	    This function is currently under development. It doesn't
 *		    work.
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
 * DESCRIPTION:	    Callback function executed on every iteration of the solver.
 *
 * ARGUMENTS:	    iter: (const size_t) -- number of iterations thus far.
 *		    params: (void *) -- parameters passed from the caller.
 *		    w: (const gsl_multifit_nlinear_workspace *) -- workspace.
 *
 * RETURN:	    void.
 *
 * NOTES:	    none.
 ***/
void callback(const size_t iter,
	      void * params,
	      const gsl_multifit_nlinear_workspace * w)
{
  gsl_vector * x = gsl_multifit_nlinear_position(w);
  fprintf(surface_log,
	  "iter %2zu: Y = %2.4eEg + %2.4eEp + %2.4eEg^2 + %2.4eEp^2 + %2.4e\n",
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
 * RETURN:	    int --  0 on success, -1 otherwise.
 *
 * NOTES:	    none.
 ***/
int fit_surface(fit_data_t * data, bool call, FILE * outfh)
{
  surface_log = outfh ? outfh : stdout; /* Setup global file descriptor. */

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
    .df = NULL, /* Use the finite difference Jacobian. */
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

  /* Compute the initial cost. */
  gsl_vector * res = gsl_multifit_nlinear_residual(w);
  double chisq0;
  gsl_blas_ddot(res, res, &chisq0);

  /* Solve the system. */
  int info, status;
  status = gsl_multifit_nlinear_driver(20, xtol, gtol, ftol,
				       callback, NULL, &info, w);

  /* Compute covariance of best fit parameters. */
  gsl_matrix * Jacobian = gsl_multifit_nlinear_jac(w);
  gsl_matrix * covar = gsl_matrix_alloc(fdf.p, fdf.p);
  gsl_multifit_nlinear_covar(Jacobian, 0.0, covar);

  /* Compute final cost. */
  double chisq1;
  gsl_blas_ddot(res, res, &chisq1);

  /* Print the output. */
  print_to_log(w, &fdf, info, status, covar, chisq0, chisq1,
	       data->empirical_data->size1, data->empirical_data->size2);

  /* Fill the struct with the data */
  data->coefficients = calloc(5, sizeof(fit_param_t));
  if (data->coefficients == NULL)
    return 1;
  fit_param_t * parr = data->coefficients;
  double c = GSL_MAX_DBL(1, sqrt(chisq1 /
				 (data->empirical_data->size1 -
				  data->empirical_data->size2)));
  for (int i = 0; i < 5; i++) {
    parr[i].value = gsl_vector_get(w->x, i);
    parr[i].error = c * sqrt(gsl_matrix_get(covar,i,i));
  }

  gsl_multifit_nlinear_free(w);
  gsl_matrix_free(covar);
  
  return 0;
}

/*******************************************************************************
 * FUNCTION:	    plot
 *
 * DESCRIPTION:	    Pipes data and commands to GNUPlot to produce a .png image
 *		    file containing a surface plot of the regression data.
 *
 * ARGUMENTS:	    data: (fit_data_t *) -- Structure containing data to plot.
 *
 * RETURN:	    int -- 0 on success, -1 otherwise.
 *
 * NOTES:	    This function uses the gnuplot_i GNUPlot interface, written
 *		    by N. Devillard, 1998. This program does not re-license the
 *		    software written by Devillard, but it does distribute it for
 *		    ease of use.
 ***/
int plot(fit_data_t * data, bool png_output)
{
  /* Create the temp file. */
  char tmpfn[] = "/tmp/mpr-tubeXXXXXX";
  int tmpfdnum = mkstemp((char *)tmpfn);
  FILE * tmpfd = fdopen(tmpfdnum, "w");
  if (tmpfd == NULL) return -1;
  tmp_write_data(data, tmpfd);
  fclose(tmpfd);

  /* Create the GNUPlot commands. */
  char *script = NULL, *fn = NULL, *plot = NULL, *total = NULL;
  fit_param_t * parr = data->coefficients;
  asprintf(&fn, COMMAND_FN,
	   parr[0].value,
	   parr[1].value,
	   parr[2].value,
	   parr[3].value,
	   parr[4].value);
  if (fn == NULL)
    goto error_exit;

  asprintf(&plot, COMMAND_PLOT, tmpfn);
  if (plot == NULL)
    goto error_exit;

  asprintf(&script, COMMAND_SCRIPT, fn, plot);
  if (script == NULL)
    goto error_exit;

  asprintf(&total, "%s%s",
	   png_output ? COMMAND_PNG_OUTPUT : "",
	   script);
  if (total == NULL)
    goto error_exit;

  gnuplot_ctrl * proc = gnuplot_init();
  gnuplot_cmd(proc, total); /* Returns void, so we can only hope. */
  gnuplot_close(proc);

  /* Remove the temp file. */
  unlink(tmpfn);

  free(total);
  free(script);
  free(plot);
  free(fn);
  return 0;

 error_exit: {
    if (total != NULL) free(total);
    if (script != NULL) free(script);
    if (plot != NULL) free(plot);
    if (fn != NULL) free(fn);
    return -1;
  }
}

/*******************************************************************************
 * STATIC FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    print_to_log
 *
 * DESCRIPTION:	    Print the results to the output file.
 *
 * ARGUMENTS:	    w: (gsl_multifit_nlinear_workspace *) -- resultant data.
 *		    fdf: (gsl_multifit_nlinear_fdf *) -- resultant data.
 *		    info: (int) -- resultant data.
 *		    status: (int) -- resultant data.
 *		    covar: (gsl_matrix *) -- resultant data.
 *		    chisq0: (double) -- resultant data.
 *		    chisq1: (double) -- resultant data.
 *
 * RETURN:	    0
 *
 * NOTES:	    none.
 ***/
static int print_to_log(gsl_multifit_nlinear_workspace * w,
			gsl_multifit_nlinear_fdf * fdf,
			int info,
			int status,
			gsl_matrix * covar,
			double chisq0,
			double chisq1,
			size_t n,
			size_t p)
{
  fprintf(surface_log, "Summary from method: '%s'\n",
	  gsl_multifit_nlinear_trs_name(w));
  fprintf(surface_log, "Number of iterations: %zu\n",
	  gsl_multifit_nlinear_niter(w));
  fprintf(surface_log, "Function evaluations: %zu\n", fdf->nevalf);
  fprintf(surface_log, "Jacobian evaluations: %zu\n", fdf->nevaldf);
  fprintf(surface_log, "Reason for stopping: %s\n",
	  (info == 1) ? "small step size" : "small gradient");
  fprintf(surface_log, "Initial |f(x)| = %f\n", sqrt(chisq0));
  fprintf(surface_log, "Final   |f(x)| = %f\n", sqrt(chisq1));

  double dof = n - p;
  double c = GSL_MAX_DBL(1, sqrt(chisq1 / dof));
  fprintf(surface_log, "(Chi^2)/dof = %g\n", chisq1 / dof);

  fprintf (surface_log, "B_0 = %.5f +/- %.5f\n", gsl_vector_get(w->x, 0),
	   c * sqrt(gsl_matrix_get(covar,0,0)));
  fprintf (surface_log, "B_1 = %.5f +/- %.5f\n", gsl_vector_get(w->x, 1),
	   c * sqrt(gsl_matrix_get(covar,1,1)));
  fprintf (surface_log, "B_2 = %.5f +/- %.5f\n", gsl_vector_get(w->x, 2),
	   c * sqrt(gsl_matrix_get(covar,2,2)));
  fprintf (surface_log, "B_3 = %.5f +/- %.5f\n", gsl_vector_get(w->x, 3),
	   c * sqrt(gsl_matrix_get(covar,3,3)));
  fprintf (surface_log, "B_4 = %.5f +/- %.5f\n", gsl_vector_get(w->x, 4),
	   c * sqrt(gsl_matrix_get(covar,4,4)));
  
  fprintf (surface_log, "status = %s\n", gsl_strerror (status));
  return 0;
}

/*******************************************************************************
 * FUNCTION:	    tmp_write_data
 *
 * DESCRIPTION:	    Write the empirical data in fit_data out to the open file.
 *
 * ARGUMENTS:	    fit_data: (fit_data_t *) -- the struct containing the data.
 *		    tmpfd: (FILE *) -- open file descriptor to write to.
 *
 * RETURN:	    int -- 0 on success, -1 otherwise.
 *
 * NOTES:	    none.
 ***/
static int tmp_write_data(fit_data_t * fit_data, FILE * tmpfd)
{
  char * format = "%2.4g %2.4g %2.4g\n";
  gsl_matrix * data = fit_data->empirical_data;

  for (int i = 0; i < data->size1; i++) {
    fprintf(tmpfd, format,
	    gsl_matrix_get(data, i, 0),
	    gsl_matrix_get(data, i, 1),
	    gsl_matrix_get(data, i, 2));
  }

  return 0;
}

/******************************************************************************/
