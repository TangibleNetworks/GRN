// Regulatory network - show excitatory and inhibitory coupling

#include <math.h>
#include<TN05.h>                      // Requires TN.h, TN.cpp, Keywords.txt in folder <Arduino>/Libraries/TN/

// Model parameters
float dt = 0.05;                    // size of time step for Euler method
float population = 0;               // initial populations are exactly zero
float old_population = 0;
float growth_rate = 0.1; // intrinsic growth rate
float couplingPos = 0.5;
float couplingNeg = 0.6;
const float population_max = 20;

TN Tn = TN(0.0,population_max);    // Create TN object.

void setup () {}                   // setup of the Unit is handled by the constructor of the TN class. Nice.

void loop() {
    
  old_population = population;  

  updatePopulation();
  Tn.analogWrite(population);
  Tn.colour(0, round(population*255/population_max), 0);    // basal        -> green

  delay(10);                     // length of delay to be tuned..
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void updatePopulation(){
// if switch is not pressed, update population based on Lotka-Volterra dynamics:
  if (!Tn.sw()){
    population += growth_rate*old_population*dt*(-0.5 + Tn.pot());
    for (int i=0; i<3; i++) {
      if (Tn.isConnected(i)){
        population += dt*couplingPos*Tn.analogRead(i)*old_population;
      }
    }
    for (int i=3; i<6; i++) {
      if (Tn.isConnected(i)){
        population -= dt*couplingNeg*Tn.analogRead(i)*old_population;
      }
    }
  }
  // if switch is pressed, increase the population and ignore the LV dynamics:
  else{
    population += 0.02;	
  }

  // bound the population at population_max
  population = min(population,population_max);
  
}


