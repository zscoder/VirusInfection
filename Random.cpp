#include "Random.h"

mt19937 Random::rng(chrono::steady_clock::now().time_since_epoch().count()); //initialize our random-number generator with time as seed

