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
 * LAST EDITED:	    08/21/2017
 ***/

#ifndef __ET_UTIL_H__
#define __ET_UTIL_H__

/*******************************************************************************
 * INCLUDES
 ***/

#include <stdlib.h>

/*******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

extern double * read_tuples(const char * filename, size_t n);

#endif /* __ET_UTIL_H__ */

/******************************************************************************/
