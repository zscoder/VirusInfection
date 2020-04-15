#include "Random.h"

mt19937 Random::rng(chrono::steady_clock::now().time_since_epoch().count());

