/*
  CSC D84 - Unit 3 - Reinforcement Learning

  This file contains stubs for implementing the Q-Learning method
  for reinforcement learning as discussed in lecture. You have to
  complete two versions of Q-Learning.

  * Standard Q-Learning, based on a full-state representation and
    a large Q-Table
  * Feature based Q-Learning to handle problems too big to allow
    for a full-state representation

  Read the assignment handout carefully, then implement the
  required functions below. Sections where you have to add code
  are marked

  **************
  *** TO DO:
  **************

  If you add any helper functions, make sure you document them
  properly and indicate in the report.txt file what you added.

  Have fun!

  DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
  management being done properly, and for memory leaks.

  Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

int valid_move(double gr[max_graph_size][4], int curr, int move) {
  int next;
  if (move == 0) {
    next = curr - 32;
  } else if (move == 1) {
    next = curr + 1;
  } else if (move == 2) {
    next = curr + 32;
  } else {
    next = curr - 1;
  }
  return gr[curr][move];
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

int sum(double l[], int size) {
   /* This function returns the sum of the list*/
  int i;
  double res;
  for (i=0;i<size;i++) {
    res += l[i];
  }
  return (int)res;
}

/* check if its a cheese or cat */
int loc_in_locs(int locations[][2], int goal[2], int size) {
  int i;
  for (i=0;i<size;i++) {
    if (locations[i][0] == goal[0] && locations[i][1] == goal[1]) {
      return 1;
    }
  }
  return 0;
}

void QLearn_update(int s, int a, double r, int s_new, double *QTable){
 /*
   This function implementes the Q-Learning update as stated in Lecture. It
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.

   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table

   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not
   have to change their values. Use them as they are.

   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double max = INT_MIN,
    curr;
  for (int i = 0; i<4; i++) {
    curr = *(QTable+(4*s_new)+i);
    if (curr > max) {
      max = curr;
    }
  }

  *(QTable+(4*s)+a) += alpha * (r + (lambda*max) - *(QTable+(4*s) + a));

}

int QLearn_action(double gr[max_graph_size][4],
  int mouse_pos[1][2],
  int cats[5][2],
  int cheeses[5][2],
  double pct,
  double *QTable,
  int size_X,
  int graph_size){
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.

     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.

     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.

     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left

     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!

   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of

        graph_size^3 x 4

   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese.
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.

   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall.

   So all in all, you have a big table.

   For example, on an 8x8 maze, the Q-table will have a size of

       64^3 x 4  entries

       with

       size_X = 8   <--- size of one side of the maze
       graph_size = 64    <--- Total number of nodes in the graph

   Indexing within the Q-table works as follows:

     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n

     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!

     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]

     (yes, it's a linear array, no shorcuts with brackets!)

     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  // the move to be made,
  int a,
      value,
      move_action = 0,
      mouse_index = mouse_pos[0][0] + (mouse_pos[0][1]*size_X),
      cat_index = cats[0][0] + (cats[0][1]*size_X),
      cheese_index = cheeses[0][0] + (cheeses[0][1]*size_X),
      state = mouse_index + (cat_index*graph_size) + (cheese_index*graph_size*graph_size),
      r = rand() % 100;

  double max = INT_MIN;
  if (mouse_index >= 0 && mouse_index < 1024) {
    if ((r <= (pct*100)) || pct == 1) {
      // for each move
      for (a = 0; a < 4; a ++) {
        if (valid_move(gr, mouse_index, a)) {
          value = *(QTable+(4*state)+a);
          if (value > max) {
            max = value;
            move_action = a;
          }
        }
      }
    } else {
      do {
        move_action = rand() % 4;
      } while (!valid_move(gr, mouse_index, move_action));
    }
  }

  return (move_action);

}

double QLearn_reward(double gr[max_graph_size][4],
  int mouse_pos[1][2],
  int cats[5][2],
  int cheeses[5][2],
  int size_X,
  int graph_size) {
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position.

    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the
    mouse.

    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward.

    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double dist_cat_closest = INT_MAX,
    dist_cat_total = 0,
    total_cats = 0,
    dist_cheese_avg = 0,
    dist_cheese_closest  = INT_MAX,
    total_cheeses = 0,
    curr,
    result;

  int eaten, ate_cheese,
    x = mouse_pos[0][0],
    y = mouse_pos[0][1],
    i;
  // got eaten
  eaten = loc_in_locs(cats, mouse_pos[0], 5);
  // ate cheese;
  ate_cheese = loc_in_locs(cheeses, mouse_pos[0], 5);


  for (i=0; i<5;i++) {
    if (cats[i][0] != -1 && cats[i][1] != -1) {
      curr = abs(cats[0][0] - x) + abs(cats[0][1] - y);
      if (curr < dist_cat_closest) {
        dist_cat_closest = curr;
      }
      dist_cheese_avg += curr;
      total_cats++;
    }
    if (cheeses[i][0] != -1 && cheeses[i][1] != -1) {
      curr = abs(cheeses[0][0] - x) + abs(cheeses[0][1] - y);
      if (curr < dist_cheese_closest) {
        dist_cheese_closest = curr;
      }
      dist_cheese_avg += curr;
      total_cheeses++;
    }
  }


  if (ate_cheese) {
    result = max_graph_size;
  } else if(eaten) {
    result = -max_graph_size / 2;
  } else {
    result = (max_graph_size / pow(dist_cheese_closest, 2)) - 10 * dist_cheese_closest + 3 * dist_cat_closest;
  }
  return result;
}

void feat_QLearn_update(double gr[max_graph_size][4],
  double weights[25],
  double reward,
  int mouse_pos[1][2],
  int cats[5][2],
  int cheeses[5][2],
  int size_X,
  int graph_size) {
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)

    Your code must then evaluate the update and apply it to the weights in the weight array.
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/


  int maxA,
    i;

  double features[25],
         qsa,
         maxU = INT_MIN;

  for (i=0;i<25;i++) {
    features[i] = 0;
  }

  maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
  evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
  qsa = Qsa(weights, features);

  for (int f=0; f<numFeatures; f++) {
    // if there is no weight meaning no feature? stub value for testing
    weights[f] += alpha*(reward + lambda*(maxU) - qsa)*(features[f]);
  }
}

int feat_QLearn_action(double gr[max_graph_size][4],
  double weights[25],
  int mouse_pos[1][2],
  int cats[5][2],
  int cheeses[5][2],
  double pct,
  int size_X,
  int graph_size) {
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.

    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.

    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.

    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  int next_move,
    mouse_idx = mouse_pos[0][0] + (mouse_pos[0][1]*size_X),
    r = rand() % 100,
    maxA;

  double maxU = INT_MIN;

  if (r <= pct*100) {
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
    next_move = maxA;
  } else {
    next_move = rand() % 4;
    while(!valid_move(gr, mouse_idx, next_move)) {
      next_move = rand() % 4;
    }
  }
  return next_move;
}

void evaluateFeatures(
  double gr[max_graph_size][4],
  double features[25],
  int mouse_pos[1][2],
  int cats[5][2],
  int cheeses[5][2],
  int size_X,
  int graph_size) {
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all thmaxQsae feature values in the features[] array.

   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!

   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.

   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */
  features[0] = 1;closest_cat_distance(cats, mouse_pos);
  features[1] = 1;total_cat_distance(cats, mouse_pos);
  features[2] = 1;closest_cheese_distance(cheeses, mouse_pos);
  features[3] = 1;total_cheese_distance(cheeses, mouse_pos);
  features[4] = 1;num_walls(gr, mouse_pos);
}

double Qsa(double weights[25], double features[25]) {
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/
  double sum = 0;
  int i;
  for (i=0;i<numFeatures;i++) {
    sum += weights[i] * features[i];
  }
  return sum;
}

void maxQsa(double gr[max_graph_size][4],
  double weights[25],
  int mouse_pos[1][2],
  int cats[5][2],
  int cheeses[5][2],
  int size_X,
  int graph_size,
  double *maxU,
  int *maxA){
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.

   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall.
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  int x = mouse_pos[0][0],
    y = mouse_pos[0][1],
    mouse_idx = x + y * size_X,
    i,
    next_move,
    new_pos[1][2],
    moves[4] = {
      mouse_idx - size_X, //up
      mouse_idx + 1,      //right
      mouse_idx + size_X, //bot
      mouse_idx -1        //left
    };

  double max_qsa = INT_MIN,
    features[25],
    curr_qsa;

  for (i=0;i<25;i++) {
    features[i] = 0;
  }

  for (i=0;i<4;i++) {
    if (valid_move(gr, mouse_idx, i)) {
      new_pos[0][0] = moves[i] % 32;
      new_pos[0][1] = moves[i] / 32;
      evaluateFeatures(gr, features, new_pos, cats, cheeses, size_X, graph_size);
      curr_qsa = Qsa(weights, features);
      if (curr_qsa > max_qsa) {
        max_qsa = curr_qsa;
        next_move = i;
      }
    }
  }

  *maxU=max_qsa;  // <--- stubs! your code will compute actual values for these two variables!
  *maxA=next_move;
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/


/***
 * Feature 1
 */
double closest_cat_distance(int cats[5][2], int mouse_pos[1][2]) {
  int closest_cat = -1,
    curr,
    i;

  for (i=0; i<5;i++) {
    if (cats[i][0] != -1 && cats[i][1] != -1) {
      curr = abs(cats[0][0] - mouse_pos[0][1]) + abs(cats[0][1] - mouse_pos[0][1]);
      if (curr < closest_cat) {
        closest_cat = curr;
      }
    }
  }
  return closest_cat;
}


/***
 * Feature 2
 */
double total_cat_distance(int cats[5][2], int mouse_pos[1][2]) {
  int total_cat_dist = 0,
    i;

  for (i=0; i<5;i++) {
    if (cats[i][0] != -1 && cats[i][1] != -1) {
      total_cat_dist += abs(cats[0][0] - mouse_pos[0][1]) + abs(cats[0][1] - mouse_pos[0][1]);
    }
  }
  return total_cat_dist;
}

/***
 * Feature 3
 */
double closest_cheese_distance(int cheeses[5][2], int mouse_pos[1][2]) {
  int closest_cheeses = -1,
    curr,
    i;

  for (i=0; i<5;i++) {
    if (cheeses[i][0] != -1 && cheeses[i][1] != -1) {
      curr = abs(cheeses[0][0] - mouse_pos[0][1]) + abs(cheeses[0][1] - mouse_pos[0][1]);
      if (curr < closest_cheeses) {
        closest_cheeses = curr;
      }
    }
  }
  return closest_cheeses;
}

/***
 * Feature 4
 */
double total_cheese_distance(int cheeses[5][2], int mouse_pos[1][2]) {
  int total_cheese_dist = 0,
    i;

  for (i=0; i<5;i++) {
    if (cheeses[i][0] != -1 && cheeses[i][1] != -1) {
      total_cheese_dist += abs(cheeses[0][0] - mouse_pos[0][0]) + abs(cheeses[0][1] - mouse_pos[0][1]);
    }
  }
  return total_cheese_dist;
}


/***
 * Feature 5
 */
double num_walls(double gr[max_graph_size][4], int mouse_pos[1][2]) {
  int num_walls = 0,
    i;

  for (i=0;i<4;i++) {
    num_walls += gr[mouse_pos[0][0] + 32 * mouse_pos[0][1]][i];
  }
  return num_walls;
}