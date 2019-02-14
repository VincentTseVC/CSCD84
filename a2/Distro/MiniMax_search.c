/*
  CSC D84 - Unit 2 - MiniMax search and adversarial games

  This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
  handout carefully - it describes the game, the data you will
  have to handle, and the search functions you must provide.

  Once you have read the handout carefully, implement your search
  code in the sections below marked with

  **************
  *** TO DO:
  **************

  Make sure to add it to your report.txt file - it will be marked!

  Have fun!

  DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
  management being done properly, and for memory leaks.

  Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"

void enqueue(Queue *self, int item) {
  /* This function insert the item to the front of the linked list container */
  Node *new_node = (struct Node*)malloc(sizeof(struct Node));
  new_node->next == NULL;
  new_node->data = item;
  if (self->head == NULL && self->tail == NULL) {
    self->head = self->tail = new_node;
  } else {
    self->tail->next = new_node;
    self->tail = new_node;
  }
  
}

int dequeue(Queue *self) {
  /* This function return the removed the item frmo the front of the linked list container */
  int res = self->head->data;
  Node *dummy = self->head;
  if (self->head == self->tail) {
    self->head = self->tail = NULL;
  } else {
    self->head = self->head->next;
  }
  free(dummy);
  return res;
}

int is_empty(Queue *self) {
  /* This function return true iff the container is empty, otherwise false */
  if (self->head == NULL && self->head == NULL) {
    return 1;
  } else {
    return 0;
  }
}

Queue *initializeQueue() {
  Queue *stack = (struct Queue*)malloc(sizeof(struct Queue));

  stack->dequeue = dequeue;
  stack->head = NULL;
  stack->tail = NULL;
  stack->enqueue = enqueue;
  stack->is_empty = is_empty;
  return stack;
}

int getIndex(int i, int x, int y){
    if (i == 0) return (x + ((y-1)*size_X));
    if (i == 1) return ((x+1) + (y*size_X));
    if (i == 2) return (x + ((y+1)*size_X));
    if (i == 3) return ((x-1) + (y*size_X));
    return 0;
}

int max(int a, int b) {
  /* This function returns the maximum of two ints*/
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int min(int a, int b) {
  /* This function returns the minimum of two ints*/
  if (a > b) {
    return b;
  } else {
    return a;
  }
}

int cood_to_idx(int loc[2]) {
  return loc[0] + (loc[1] * size_X);
}

/* check if its a cheese or cat */
int loc_in_locs(int locations[][2], int location, int size) {
  int i;
  for (i=0;i<size;i++) {
    if (cood_to_idx(locations[i]) == location) {
      return 1;
    }
  }
  return 0;
}


double MiniMax(double gr[graph_size][4], 
              int path[1][2],
        double minmax_cost[size_X][size_Y], 
        int cat_loc[10][2], 
        int cats, 
        int cheese_loc[10][2], 
        int cheeses, 
        int mouse_loc[1][2], 
        int mode, 
        double (*utility)(int cat_loc[10][2], 
                  int cheese_loc[10][2], 
                  int mouse_loc[1][2], 
                  int cats, 
                  int cheeses, 
                  int depth, 
                  double gr[graph_size][4]), 
        int agentId, 
        int depth, 
        int maxDepth, 
        double alpha, 
        double beta)
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

  Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
  list and agent positions are stored.  

  Note that in this case, the path will contain a single move - at the top level, this function will provide
  an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
       all game states down to the maximum search depth specified by the user. In order to do that,
       the function needs to be called with the correct state at each specific node in the mini-max
       search tree.

       The game state is composed of:

      * Mouse, cat, and cheese positions (and number of cats and cheeses)
      
       At the top level (when this function is called by the mini-max driver code), the game state
       correspond to the current situation of the game. But once you start recursively calling
       this function for lower levels of the search tree the positions of agents will have changed.
       
       Therefore, you will need to define local variables to keep the game state at each node of the
       mini-max search tree, and you will need to update this state when calling recursively so that
       the search does the right thing.

       This function *must check* whether:
      * A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
        at which point it calls the utility function to get a value
      * Maximum search depth has been reached (depth==maxDepth), at which point it will also call
        the utility function to get a value
      * Otherwise, call recursively using the candidate configuration to find out what happens
        deeper into the mini-max tree.

   Arguments:
    gr[graph_size][4]       - This is an adjacency list for the maze
    path[1][2]      - Your function will return the optimal mini-max move in this array.
    minmax_cost[size_X][size_Y] - An array in which your code will store the
              minimax value for maze locations expanded by
              the search *when called for the mouse, not
              for the cats!*

              This array will be used to provide a visual 
              display of minimax values during the game.

    cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
           but there can be fewer). Only valid cat locations are 0 to (cats-1)
    cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
               but possibly fewer). Valid locations are 0 to (cheeses-1)
    mouse_loc[1][2] - Mouse location - there can be only one!
    mode - Search mode selection:
          mode = 0  - No alpha-beta pruning
          mode = 1  - Alpha-beta pruning

    (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
        - This is a pointer to the utility function which returns a value for a specific game configuration

           NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing            that requires knowledge of the maze for computing the utility values.

          * How to call the utility function from within this function : *
          - Like any other function:
            u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
            
    agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
  
    depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
    
    maxDepth: maximum desired search depth - once reached, your code should somehow return
        a minimax utility value for this location.

    alpha. beta: alpha and beta values passed from the parent node to constrain search at this
           level.

   Return values:
    Your search code will directly update data passed-in as arguments:
    
    - Mini-Max value  : Notice this function returns a double precision number. This is
            the minimax value at this level of the tree. It will be used 
            as the recursion backtracks filling-in the mini-max values back
            from the leaves to the root of the search tree. 

    - path[1][2]    : Your MiniMax function will return the location for the agent's 
            next location (i.e. the optimal move for the agent). 
    - minmax_cost[size_X][size_Y]   :  Your search code will update this array to contain the
               minimax value for locations that were expanded during
               the search. This must be done *only* for the mouse.

               Values in this array will be in the range returned by
               your utility function.

    * Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
          or the location of the mouse - if you try, the driver code will know it *
      
    That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO: Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *    as per the minimax algorithm.
 *  
 *    Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *    along the path.
 *
 *    You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *    file and explain in your code why they are needed and how they are used.
 *
 *    Recursion should appear somewhere.
 *
 *    MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *    in the maze, you must update minmax_cost[][] for that location.
 *
 *    How you design your solution is up to you. But:
 *
 *    - Document your implementation by adding concise and clear comments in this file
 *    - Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

 // Stub so that the code compiles/runs - This will be removed and replaced by your code!
  int x = mouse_loc[0][0],
    y = mouse_loc[0][1],
    at_terminal = checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses),
    temp_mouse_loc[1][2],
    temp_cat_loc[10][2],
    next_agent,
    next_idx,
    cat_idx,
    curr,
    i;
  double v;
  for (i=0;i<10;i++) {
    temp_cat_loc[i][0] = cat_loc[i][0];
    temp_cat_loc[i][1] = cat_loc[i][1];
  }
  if (depth == 0) {
    path[0][0] = x;
    path[0][1] = y;
  }

  // set default values for alpha
  if ((mode == 1) && (depth == 0)) {
    alpha = INT_MIN;
    beta = INT_MAX;
  }
  if (at_terminal || (depth >= maxDepth)) {
    minmax_cost[x][y] = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
    v = minmax_cost[x][y];
  // mouse
  } else if (agentId == 0) {
    v = INT_MIN;
    int mouse_idx = x + (y * size_X);
    for (i=0;i<4;i++) {
      next_idx = getIndex(i, x, y);
      if (next_idx >= 0 && next_idx < graph_size && gr[mouse_idx][i]) {
        temp_mouse_loc[0][0] = next_idx % size_X;
        temp_mouse_loc[0][1] = next_idx / size_X;

        v = max(v, MiniMax(gr, path, minmax_cost, cat_loc, cats, 
                        cheese_loc, cheeses, temp_mouse_loc, mode, utility, 
                        1, depth+1, maxDepth, alpha, beta));
        if (v > alpha) {
          alpha = v;
          if (depth == 0) {
            path[0][0] = temp_mouse_loc[0][0];
            path[0][1] = temp_mouse_loc[0][1];
          }
        }
        minmax_cost[x][y] = v;
        if (mode == 1 && beta <= alpha) {
          break;
        }
      }
    }
  // cats
  } else {
    v = INT_MAX;
    x = cat_loc[agentId - 1][0];
    y = cat_loc[agentId - 1][1];
    cat_idx = x + (y * size_X),
      next_agent;
    for (i = 0; i < 4; i++) {
      next_idx = getIndex(i, x, y);
      if (next_idx >= 0 && next_idx < 1024 && gr[cat_idx][i] == 1) {
        temp_cat_loc[agentId-1][0] = next_idx % size_X;
        temp_cat_loc[agentId-1][1] = next_idx / size_X;

        if (agentId < cats) {
          next_agent = agentId + 1;
        } else {
          next_agent = 0;
        }
        v = min(v, MiniMax(gr, path, minmax_cost, temp_cat_loc, cats, 
                      cheese_loc, cheeses, mouse_loc, mode, utility, 
                      next_agent, depth+1, maxDepth, alpha, beta));

        beta = min(beta, v);

        if (mode == 1 && beta <= alpha) {
          break;
        }
      }
    }

  }
  return v;
}

int bfs (int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
  int x = mouse_loc[0][0],
    y = mouse_loc[0][1],
    idx = x + y * size_X,
    prev_node[graph_size],
    visited[graph_size],
    next_idx,
    found = 0,
    step=0,
    curr,
    i,
    j;

  for (i=0;i<graph_size;i++) {
    prev_node[i] = -1;
    visited[i] = 0;
  }

  Queue *queue = initializeQueue();
  queue->enqueue(queue, idx);

  while (!found) {
    curr = queue->dequeue(queue);
    if (visited[curr] <= 0 &&
      !loc_in_locs(cat_loc, curr, cats) &&
      curr >= 0 &&
      curr < graph_size) {

      visited[curr] = ++step;
      for (int i = 0; i < 4; i ++) {
        next_idx = getIndex(i, curr % size_X, curr / size_Y);
        if (gr[curr][i]) {
          queue->enqueue(queue, next_idx);
          if (visited[next_idx] <= 0) {
            prev_node[next_idx] = curr;
          }
        }
      }
    }
    found = queue->is_empty(queue) || loc_in_locs(cheese_loc, curr, cheeses);
  }
  // free the container
  while (!queue->is_empty(queue)) {
    queue->dequeue(queue);
  }

  i = 0;
  while (curr >= 0) {
    curr = prev_node[curr];
    i += 1;
  }
  return i;
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
  This function computes and returns the utility value for a given game configuration.
  As discussed in lecture, this should return a positive value for configurations that are 'good'
  for the mouse, and a negative value for locations that are 'bad' for the mouse.

  How to define 'good' and 'bad' is up to you. Note that you can write a utility function
  that favours your mouse or favours the cats, but that would be a bad idea... (why?)

  Input arguments:

    cat_loc - Cat locations
    cheese_loc - Cheese locations
    mouse_loc - Mouse location
    cats - # of cats
    cheeses - # of cheeses
    depth - current search depth
    gr - The graph's adjacency list for the maze

    These arguments are as described in A1. Do have a look at your solution!
 */
  // find the closest cheese distance
  double points, curr, total,
    cheese_closests=INT_MAX,cheese_pts=0, cheese_bfs=0,
    cat_closests=INT_MAX,cat_pts=0,
    wall_pts=0,
    other_pts = 0;

  int i, x, y, idx;
  x = mouse_loc[0][0];
  y = mouse_loc[0][1];
  idx = x + y * size_X;

  //wall pts
  total = 0;
  for (i=0;i<4;i++) {
    total += gr[idx][i];
  }
  wall_pts = 2 * (4 - total);
   
  //mouse pts
  total = 0;
  for (i=0; i<cheeses;i++) {
    curr = abs(cheese_loc[i][0] - x) + abs(cheese_loc[i][1] - y);
    cheese_closests = min(cheese_closests, curr);
    total += curr;
  }
  if (cheese_closests < 5) {
    cheese_bfs = bfs(cat_loc, cheese_loc, mouse_loc, cats, cheeses, gr);
  } else {
    cheese_bfs = cheese_closests;
  }
  cheese_pts = -(cheese_bfs * 1.5) + (size_X + size_Y - total);
  //cat pts
  total = 0;
  for (i=0; i<cats;i++) {
    curr = abs(cat_loc[i][0] - x) + abs(cat_loc[i][1] - y);
    cat_closests = min(cat_closests, curr);
    total += curr;
  }
  cat_pts = -size_X/max(cat_closests, 1) + total/cats;


  points = cat_pts + cheese_pts + other_pts;
  return points;

}

int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}

