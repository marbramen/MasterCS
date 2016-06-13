
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>

#define N 15
#define HOME 0
#define NO_CITY -1
#define INF 10000000

struct tipeTour{
   int cities[N + 1]; // one more because is a cycle
   int countCities;
   int length;
};


// int graphCity[N + 1][N + 1] = {{0,99,1,99},{99,0,99,1},{99,1,0,98},{1,99,23,0}}; // Complete DiGraph
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
   int i,j;
   for(i = 0; i < N; i++){
      for(j = 0; j < N; j++){
         if(i != j) 
            graphCity[i][j] = gRanNum(10., 50.0);
         else 
            graphCity[i][j] = 0;
      }
   }
}

void addCity(tipeTour* tour, int city){
    int length = graphCity[tour->cities[tour->countCities - 1]][city];
    tour->cities[tour->countCities] = city;
    tour->length += length;
    tour->countCities++;
}

void removeLastCity(tipeTour* tour){
    int countCities = tour->countCities;
    int length = graphCity[tour->cities[countCities - 2]][tour->cities[countCities - 1]];
    tour->countCities--;
    tour->length -= length;
}

void updateBestTour(tipeTour* tour, int city){
    for(int i = 0; i < tour->countCities; i++)
      bestTour.cities[i] = tour->cities[i];
    bestTour.cities[N] = HOME;
    bestTour.countCities = N + 1;
    bestTour.length = tour->length + graphCity[city][HOME];
}

//the search if the neighbor was visited is sequential
bool feasible(int city, int neighCity, tipeTour* tour){
   bool flag = false;
   for(int i = 0; i < tour->countCities && !flag; i++)
      flag = tour->cities[i] == neighCity;
   return !flag && (tour->length + graphCity[city][neighCity] < bestTour.length);
}

//Initilize a tour with no visited cities
//that condition is neccesary for DFS
void initTour(tipeTour* tour){      
   for(int i = 0; i <= N; i++)
      tour->cities[i] = NO_CITY;
   tour->length = 0;
   tour->countCities = 0;
}

/*==== PRINT THE TOUR  ===*/
void printTour(tipeTour* tour) {   
   printf("Lenght of the tour: %d\nCities path:", tour->length);
   int i;
   for (i = 0; i < tour->countCities; i++)
         printf("%d%c", tour->cities[i], i + 1 != tour->countCities ? ' ' : '\n');   
}

/*==== DEPTH FIRST SEARCH FOR TRAVELING SALESPERSON PROBLEM ===*/


void dfs(tipeTour* tour, int city, int length){
    tour->cities[tour->countCities] = city;
    tour->countCities++;
    tour->length += length;
    if(tour->countCities == N){
        if(tour->length + graphCity[city][HOME] < bestTour.length)
            updateBestTour(tour, city);      
    }else{
        for(int nbr = 1; nbr < N; nbr++){
            if(feasible(city, nbr, tour)){          
                dfs(tour, nbr, graphCity[city][nbr]);
                removeLastCity(tour);
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
   // imprime();
   initTour(&bestTour);   
   bestTour.length = INF;
   tipeTour* tour = (tipeTour*)malloc(sizeof(tipeTour));
   initTour(tour);
   dfs(tour, HOME, 0);
   printTour(&bestTour);
   return 0;
}