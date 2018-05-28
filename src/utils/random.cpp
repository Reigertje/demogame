#include "random.h"
#include <algorithm>
#include <iostream>

#ifdef _WIN32
static long long int random_next = 1;
#endif

#ifdef __linux
static long random_next = 1;
#endif

/* CROSS_RANDOM_MAX assumed to be 32767 */
int utils::random() {
    random_next = random_next * 1103515245 + 12345;
    int result = ((unsigned)(random_next/65536) % RANDOM_MAX);
  
    return result;
}

int utils::randomGen(int i) {
	return random() % i;
}

void utils::seed(long seed) {
   	random_next = seed;
}

long utils::getSeed() {
	return random_next;
}

float utils::random(float min, float max) {
	return min + (max - min) * randomNorm();
}

float utils::randomNorm() {
	return (float)random() / RANDOM_MAX; 
}
