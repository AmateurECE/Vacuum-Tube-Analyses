################################################################################
# NAME:		    Makefile
#
# AUTHOR:	    Ethan D. Twardy
#
# DESCRIPTION:	    Makefile for this program.
#
# CREATED:	    08/22/2017
#
# LAST EDITED:	    08/25/2017
#
# Copyright 2017, Ethan D. Twardy
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
###

TOP:=$(shell pwd)
OBJS:= main.c \
	linkedlist.c \
	fit.c \
	util.c

OBJS:=$(addprefix src/,$(OBJS))
OBJS:=$(patsubst %.c,%.o,$(OBJS))

CC=gcc
CFLAGS= -g \
	-Wall \
	-O0 \
	-I $(TOP)/include/ \
	-D HAVE_INLINE

.DELETE_ON_ERROR:
.PHONY: clean

src/main: $(OBJS)

clean:
	@rm -rf `find $(TOP) -name *.o`

################################################################################
