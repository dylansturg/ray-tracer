/*
 * SortComparators.h
 *
 *  Created on: Jan 10, 2014
 *      Author: dylan
 */

#ifndef SORTCOMPARATORS_H_
#define SORTCOMPARATORS_H_

#include "../Primitives/Surface.h"

bool compareXAxis(Surface* surface1, Surface* surface2){
	return surface1->getCenter()[0] < surface2->getCenter()[0];
}

bool compareYAxis(Surface* surface1, Surface* surface2){
	return surface1->getCenter()[1] < surface2->getCenter()[1];
}

bool compareZAxis(Surface* surface1, Surface* surface2){
	return surface1->getCenter()[2] < surface2->getCenter()[2];
}

typedef bool (*comparator) (Surface*, Surface*);

static comparator comparators[] = {compareXAxis, compareYAxis, compareZAxis};


#endif /* SORTCOMPARATORS_H_ */
