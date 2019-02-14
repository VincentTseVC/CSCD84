/*
	CSC D84 - Unit 3 - Reinforcement Learning

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in QLearn.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Starter by: F.J.E., Jan. 2016
*/

#ifndef __QLearn_header

#define __QLearn_header

// Generally needed includes
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include<limits.h>

#define alpha .01			// Learning rate for standard Q-Learning
#define lambda .5			// Discount rate for future rewards
#define max_graph_size 32*32

#define numFeatures 13			// UPDATE THIS to be the number of features you have

// Function prototypes for D84 - Unit 3 - Reinforcement Learning
void QLearn_update(int s, int a, double r, int s_new, double *QTable);
int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size);
double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size);

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size);
int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size);
void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size);
double Qsa(double weights[25], double features[25]);
void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA);

void expensiveFeature1(double gr[max_graph_size][4], int path[max_graph_size][2],int start_x, int start_y, int (*goalCheck)(int x, int y, int pos[5][2]), int pos[5][2], int s_type, int *l, int size_X);
int checkForGoal(int x, int y, int pos[5][2]);

// If you need to add any function prototypes yourself, you can do so *below* this line.
typedef struct Node {
	int data;
	struct Node *next;
} Node;
typedef struct Queue {
	struct Node *head, *tail;
	int (*dequeue)(Queue*);
	void (*enqueue)(Queue*, int);
	int (*is_empty)(Queue*);
} Queue;
void enqueue(Queue *self, int item);
int dequeue(Queue *self);
int is_empty(Queue *self);
int max(int a, int b);
int min(int a, int b);
int loc_in_coord(int locations[][2], int location, int size, int size_X);
int loc_in_locs(int locations[][2], int goal[2], int size);
int cood_to_idx(int loc[2], int size_X);
int getIndex(int i, int x, int y, int size_X);
int bfs (int cat_loc[5][2],
  int cheese_loc[5][2],
  int mouse_loc[1][2],
  int cats,
  int cheeses,
  double gr[max_graph_size][4],
  int size_X);

int trapped(int x, int y, double gr[max_graph_size][4], int size_X);
int close(int x, int y, int pos[5][2], int bound);
double surrounding_terrain(int x, int y, double gr[max_graph_size][4], int size_X);
double surrounded(int x, int y, int pos[5][2]);
double amount_around(int x, int y, int pos[5][2], int bound);
#endif

