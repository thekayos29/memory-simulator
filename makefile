# Simple Makefile for Memory Simulator

all:
	g++ src/main.cpp -o memsim.exe

clean:
	del memsim.exe