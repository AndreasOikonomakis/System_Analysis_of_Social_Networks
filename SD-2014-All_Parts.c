/*
 ============================================================================
 Name        : SD-2014-Part-2.c
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "hashing.h"
#include "Statistics.h"
#include "Queries.h"



int main(int argc,char **argv) {

	int part_num;
	part_num = atoi(argv[1]);
	
	if(part_num == 1)
		Part1();
	else if(part_num ==2)
		Part2();
	else if(part_num == 3)
		Part3();
	
}


