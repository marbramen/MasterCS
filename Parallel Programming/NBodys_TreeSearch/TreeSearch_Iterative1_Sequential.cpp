
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define N 10
#define HOME 0
#define INF 10000000
#define NO_CITY -1

struct tipeTour{
   int cities[N + 1]; // one more because is a cycle
   int countCities;
   int length;
};

struct tipeStackTour {
   tipeTour* tour;            // Partial tour             
   tipeStackTour* beforeTour;  // Next record(stack) on stack     
};

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
void addCityToTour(tipeTour* tour, int city, int length){    
    tour->cities[tour->countCities] = city;
    tour->countCities++;
    tour->length += length;
}

void removeLastCity(tipeTour* tour, int length){
    tour->countCities--;
    tour->length -= length;    
}

tipeTour* replicateTour(tipeTour* tour){
    tipeTour* tempTour = (tipeTour*)malloc(sizeof(tipeTour));
    for(int i = 0; i <= N; i++)
        tempTour->cities[i] = tour->cities[i];
    tempTour->countCities = tour->countCities;
    tempTour->length = tour->length;
    return tempTour; 
}

void pushTourToStack(tipeStackTour** stack, tipeTour* tour){
    tipeStackTour* tempStack = (tipeStackTour*)malloc(sizeof(tipeStackTour));
    tempStack->tour = replicateTour(tour);
    tempStack->beforeTour = *stack;
    *stack = tempStack;
}

//Update current length, number of cities of the current stack
void pop(tipeTour** tour, tipeStackTour** stack){       
    *tour = (*stack)->tour;
    *stack = (*stack)->beforeTour;
}

bool empty(tipeStackTour* stack){
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
bool feasible(int city, int nbr, tipeTour* tour){
   bool flag = false;
   for(int i = 0; i < tour->countCities && !flag; i++)
      flag = tour->cities[i] == nbr;
   return !flag && (tour->length + graphCity[city][nbr] < bestTour.length);
}

void updateBestTour(tipeTour* tour, int city){
    for(int i = 0; i < tour->countCities; i++)
        bestTour.cities[i] = tour->cities[i];
    bestTour.cities[N] = HOME;
    bestTour.countCities = N + 1;
    bestTour.length = tour->length + graphCity[city][HOME];
}

/*==== PRINT THE TOUR  ===*/
void printTour(tipeTour* tour) {   
   printf("Lenght of the tour: %d\nCities path: ", tour->length);
   for (int i = 0; i < tour->countCities; i++)
         printf("%d%c", tour->cities[i], i + 1 != tour->countCities ? ' ' : '\n');   
}  

/** ==== DEPTH FIRST SEARCH FOR TRAVELING SALESPERSON PROBLEM === **/
void dfs(){
    int city;
    tipeStackTour* stack = (tipeStackTour*)malloc(sizeof(tipeStackTour));
    tipeTour* tour = (tipeTour*)malloc(sizeof(tipeTour));    
  
    bestTour.length = INF;
    initTour(tour);
    addCityToTour(tour, HOME, 0);    

    stack->tour = tour;
    stack->beforeTour = NULL;    

    while(!empty(stack)){
        pop(&tour, &stack);
        city = tour->cities[tour->countCities-1];      
        if(tour->countCities == N){            
            if(tour->length + graphCity[city][HOME] < bestTour.length)
                updateBestTour(tour, city);
        }else{
            for(int nbr = N - 1; nbr > 0; nbr--)
                if(feasible(city, nbr, tour)){
                    addCityToTour(tour, nbr, graphCity[city][nbr]);
                    pushTourToStack(&stack, tour);
                    removeLastCity(tour, graphCity[city][nbr]);
                }
        }        
    }
}

void imprime(){
   for(int i = 0; i < N; i++)
      for(int j = 0; j < N; j++)
         printf("%d%c", graphCity[i][j], j + 1 != N ? ' ' : '\n');
}

int main(){
   initGraphCity();
   imprime();
   initTour(&bestTour);   
   dfs();
   printTour(&bestTour);
   return 0;
}