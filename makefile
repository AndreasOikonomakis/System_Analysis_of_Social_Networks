all: clear build

build:
#	gcc -o SD-2014-All_Parts SD-2014-All_Parts.c GraphLib.c graph_functions.c hfunctions.c Statistics.c Query1.c Query2.c Query3.c Query4.c Tread_Utilities.c Forum_graph.c Community_Graph_Utilities.c Clique_Utilities.c GN_Algorithm.c Parts.c CHECKS.c  -pthread 
	gcc -o SD-2014-All_Parts SD-2014-All_Parts.c GraphLib.c graph_functions.c hfunctions.c Statistics.c Query1.c Query2.c Query3.c Query4.c Parts.c CHECKS.c Thread_Utilities.c Forum_graph.c Clique_Utilities.c -pthread 
#	cc -g -Wall -o SD-2014-All_Parts SD-2014-All_Parts.c GraphLib.c graph_functions.c hfunctions.c Statistics.c Query1.c Query2.c Query3.c Query4.c Parts.c CHECKS.c Thread_Utilities.c -pthread 
clean:
	rm -f SD-2014-Part-2

clear:
	clear
