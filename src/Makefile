################################################################################
# NAME:		    Makefile
#
# AUTHOR:	    Ethan D. Twardy
#
# DESCRIPTION:	    Makefile for this program.
#
# CREATED:	    08/22/2017
#
# LAST EDITED:	    08/22/2017
###

CC = gcc
CFLAGS = -g -Wall -O0 `pkg-config --cflags gsl`
LDLIBS = `pkg-config --libs gsl`
OBJS = main.c util.c linkedlist.c fit.c

main: $(OBJS)

