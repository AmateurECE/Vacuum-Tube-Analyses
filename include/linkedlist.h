/*******************************************************************************
 * NAME:	    linkedlist.h
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    The header file containing the type definitions and function
 *		    prototypes for the implementation of a singly-linked list.
 *
 * CREATED:	    03/29/2017
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

#ifndef __ET_LINKEDLIST_H__
#define __ET_LINKEDLIST_H__

/*******************************************************************************
 * TYPE DEFINITIONS
 ***/

typedef struct _ListElm_ {
  
  void * data;
  struct _ListElm_ * next;

} ListElm;

typedef struct _List_ {

  int size;
  void (*destroy)(void * data);
  ListElm * head;
  ListElm * tail;

} List;

/*******************************************************************************
 * MACRO DEFINITIONS
 ***/

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#define list_isempty(list) ((list)->size == 0 ? 1 : 0)
#define list_ishead(list, element) ((list)->head == element ? 1 : 0)
#define list_istail(list, element) ((list)->tail == NULL ? 1 : 0)

/*******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

extern void list_init(List *, void (*)(void *));
extern int list_insnxt(List *, ListElm *, const void *);
extern int list_remnxt(List *, ListElm *, void **);
extern void list_traverse(List *, void (*)(void *));
extern int list_dest(List *);

#endif /* __ET_LINKEDLIST_H__ */

/******************************************************************************/
