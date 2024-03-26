cmake_minimum_required(VERSION 3.10)

project(pint C)

add_library(
	pint
	SHARED
	print.c
)

add_library(
	print
	SHARED
	print.c
)
