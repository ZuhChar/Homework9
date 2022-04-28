# 
# Makefile
# Author: Zachary Crimmel
# Date: Apr 27, 2022
#
# COSC 3750, Homework 9
#
# This is a Makefile designed to compile the wyshell.c file
# Collaborated with Ian Moon on this Homework
#

CC=gcc
CFLAGS= -Wall -ggdb
RM= rm -f

.PHONY: all clean tidy

all: wyshell

wyshell: wyshell.c
	${CC} ${CFLAGS} wyshell.c wyscanner.c -o wyshell

clean:
	${RM} *.o wyshell

tidy:
	${RM} a.out core.* wyshell