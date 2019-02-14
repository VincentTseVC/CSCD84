/*
	CSC D84 - Unit 1 - Search

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in AI_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Starter by: F.J.E., Jul. 2015
	Updated by: F.J.E., Jan. 2018
*/

#ifndef __AI_search_header

#define __AI_search_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<limits.h>

#include "board_layout.h"

// Function prototypes for D84 - Unit 1 - Search assignment solution

void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2], int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode, int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]));
int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);
int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4]);

// If you need to add any function prototypes yourself, you can do so *below* this line.
typedef struct Node {
	int data;
	struct Node *next;
} Node;

typedef struct Container {
	struct Node *head, *tail;
	int (*pop)(Container*);
	void (*set)(Container*, int);
	int (*is_empty)(Container*);
} Container;
void push(Container *self, int item);
void enqueue(Container *self, int item);
int pop(Container *self);
int is_empty(Container *self);
int max(int a, int b);
int min(int a, int b);
int sum(double l[], int size);
Container *initializeContainer(int mode);
int loc_in_locs(int locations[][2], int location, int size);
int cood_to_idx(int loc[2]);
void dijstras(
  double graph[graph_size][4],
  int visit_order[size_X][size_Y],
  int path[graph_size][2],
  int mouse_loc[1][2],
  int cheese_loc[10][2],
  int cheeses,
  int cat_loc[10][2],
  int cats,
  int (*heuristic)(
    int x, 
    int y, 
    int cat_loc[10][2], 
    int cheese_loc[10][2], 
    int mouse_loc[1][2],
    int cats,
    int cheeses, 
    double gr[graph_size][4])
  );

#endif

