
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define N 10
#define HOME 0
#define NO_CITY -1
#define INF 10000000

typedef struct {
   int cities[N + 1]; // one more because is a cycle
   int countCities;
   int length;
} tipeTour;

typedef struct stackStruct {
   tipeTour* tour_p;            // Partial tour             
   int city;                    // City current 
   int length;                  // Length accumulate    
   struct tipeStack* nextTour;  // Next record(stack) on stack     
} tipeStack;

int graphCity[N + 1][N + 1]; // Complete DiGraph
int lengthTotal;
tipeTour bestTour;

int gRanNum(double min, double max){
   int res = (max -  min) * ((double)rand() / (double)RAND_MAX) + min;
   // For avoiding don't exist path among cities
   if( res == 0) return gRanNum(min, max);
   return res;
}

void initGraphCity(){
   srand(time(NULL));
   for(int i = 0; i < N; i++){
      for(int j = 0; j < N; j++){
         if(i != j) 
            graphCity[i][j] = gRanNum(10.0, 50.0);
         else 
            graphCity[i][j] = 0;
      }
   }
}

/*===== STACK OPERATIONS =====*/
tipeTour* replicateTour(tipeTour* tour){
   tipeTour* tempTour = malloc(sizeof(tipeTour));
   for(int i = 0; i < N; i++)
      tempTour->cities[i] = tour->cities[i];
   tempTour->countCities = tour->countCities;
   tempTour->length = tour->length;
   return tempTour;
}

void push(tipeTour* tour, int city, int length, stackStruct** stack){
   stackStruct* tempStack = malloc(sizeof(stackStruct)); 
   tempStack = replicateTour(tour),
   tempStack->city = city;
   tempStack->length = length;
   tempStack->nextTour = *stack;
   *stack = tempStack;
}

//Update current length, number of cities of the current stack
void pop(tipeTour tour, int* city, int* length, stackStruct** stack){
   stackStruct stackTemp = *stack;
   *tour = stackTemp->tour;
   *city = stackTemp->city;
   *length = stackTemp->length;
   *stack = stackTemp->nextTour;
   free(stackTemp);
}

bool empty(stackStruct* stack){
   return stack == NULL;
}

/*====== OPERATIONS INSIDE THE DFS-TSP(Traveling Salesperson Problem) ======== */

//Initilize a tour with no visited cities
//that condition is neccesary for DFS
void initTour(tipeTour* tour){   
   for(int i = 0; i <= N; i++)
      tour->cities[i] = NO_CITY;
   tour->length = 0;
   tour->countCities = 0;
}

//the search if the neighbor was visited is sequential
void feasible(int city, int neighCity, tipeTour* tour){
   bool flag = false;
   for(int i = 0; i < tour->countCities && !flag; i++)
      flag = tour->cities[i] == neighCity;
   return flag && (tour->length + graphCity[city][nbr] < bestTour.length);
}


void isABestTour(int city, tipeTour* tour){
   if(tour->length + graphCity[city][HOME] < bestTour->length){
      for(int i = 0; i < tour->countCities; i++)
         bestTour->cities[i] = tour->cities[i];
      bestTour.cities[N] = HOME;
      bestTour.countCities = N + 1;
      bestTour.length = tour->length + graphCity[city][HOME];
   }
}

/*==== PRINT THE TOUR  ===*/
void printTour(tipeTour* tour) {   
   printf("Lenght of the tour %d:\n Cities path:", tour->length);
   for (int i = 0; i < tour->countCities; i++)
         printf("%d%c", tour->cities[i], i + 1 != tour->countCities ? ' ' : '\n');   
}  /*

/*==== DEPTH FIRST SEARCH FOR TRAVELING SALESPERSON PROBLEM ===*/

void dfs(){
     int city, length;
     tipeTour* tour;
     tipeStack* stack;
     tour = malloc(sizeof(tipeTour));

     initTour(tour);
     stack->tour = tour;
     stack->city = HOME;
     stack->length = 0;
     stack->nextTour = NULL;

     while(!empty(stack)){
         pop(&tour, &city, &length, &stack);
         tour->cities[tour->countCities] = city;
         tour->length += length;
         tour->countCities += 1;
         if(tour->countCities == N)
            isABestTour(city, &tour);
         else
            for(int neighbor = N-1; neighbor > 0; neighbor--)   
               if(feasible(city, neighbor, tour))
                  push(tour, neighbor, graphCity[city][neighbor], &stack);
     }
     free(tour);
}

int main(){
   initGraphCity();
   initTour(&bestTour);
   bestTour.length = INF;
   dfs();
   printTour(&bestTour);
   return 0;
}