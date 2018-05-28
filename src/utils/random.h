#ifndef	RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include <algorithm>

#define RANDOM_MAX 32767

namespace utils {

	int random();

	int randomGen(int i);

	float random(float min, float max);

	float randomNorm();

	void seed(long seed);

	long getSeed();

	template< class RandomIt >
	void shuffle(RandomIt first, RandomIt last) {
		std::random_shuffle(first, last, randomGen);
	}
}

#endif