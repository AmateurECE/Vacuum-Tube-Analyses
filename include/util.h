/*******************************************************************************
 * NAME:	    util.h
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Header file containing the public interface for the
 *		    functions in util.c
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

#ifndef __ET_UTIL_H__
#define __ET_UTIL_H__

/*******************************************************************************
 * INCLUDES
 ***/

#include <gsl/gsl_matrix.h>

#include <stdlib.h>

/*******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

/**
 * \brief Read tuples of size \c n from a CSV file
 * \param filename The path of the file to open
 * \param n The size of the tuples to read
 * \return \c gsl_matrix with tuples or \c NULL on failure.
 */
extern gsl_matrix * read_tuples_csv(const char * filename, size_t n);

/**
 * \brief Read tuples of size \c n from an XML file
 * \param filename The path of the file to open
 * \param n The size of the tuples to read
 * \return \c gsl_matrix or \c NULL on failure.
 */
extern gsl_matrix * read_tuples_xml(const char * filename, size_t n);

#endif /* __ET_UTIL_H__ */

/******************************************************************************/
