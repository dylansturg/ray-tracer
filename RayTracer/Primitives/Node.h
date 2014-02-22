#ifndef BBVH_NODE_H
#define BBVH_NODE_H

#include "Surface.h"
#include "BoundingBox.h"
#include <vector>
#include <utility>
#include <algorithm>
#include "../GenVector.h"
#include "../util/SortComparators.h"

using namespace std;

class Node: public Surface
{
public:


    Node()
    {

    }

	Node(vector<Surface*>* triangleList, vector<Node>* nodeLists, int left, int right)
	{
		this->nodeList = nodeLists;
		this->triangles = triangleList;
		bBox = BoundingBox(triangleList, left, right);
		this->isLeaf = 0;

		this->right = -1;

		int range = right - left;

		if (range == 1){
			this->isLeaf = 1;
			this->left = left;
			return;
		}

		if (range == 2){
			this->isLeaf = 1;
			this->left = left;
			this->right = left + 1;
			return;
		}

		pair<float, int> splitVal = bBox.getSplit();
		sort(triangleList->begin() + left, triangleList->begin() + right, comparators[splitVal.second]);

		int patitionIndex = findBoundary(triangleList, left, right, splitVal.second, splitVal.first);

		if (patitionIndex <= left || patitionIndex >= right){
			patitionIndex = left + range / 2;
		}

		nodeLists->push_back(Node(triangleList, nodeLists, left, patitionIndex));
		this->left = nodeLists->size() - 1;
		nodeLists->push_back(Node(triangleList, nodeLists, patitionIndex, right));
		this->right = nodeLists->size() - 1;


	}

    virtual ~Node()
    {

    }

    virtual int height()
    {
		return 0;
    }

    virtual bool intersect(Ray &ray)
    {
		if (this->bBox.intersect(ray))
		{
			if (!this->isLeaf)
			{
				this->nodeList->at(left).intersect(ray);
				if (this->right > 0)
				{
					this->nodeList->at(right).intersect(ray);
				}
			}
			else
			{
				this->triangles->at(left)->intersect(ray);
				if (this->right >= 0){
					this->triangles->at(right)->intersect(ray);
				}
			}
			return true;
		}
		return false;
    }

    virtual Vector3 getCenter() const
    {
        return this->bBox.getCenter();
    }

    BoundingBox getBoundingbox() const
    {
        return this->bBox;
    }

    virtual Vector3 getMaxBoundaries() const
    {
        return this->bBox.getMaxBoundaries();
    }
    virtual Vector3 getMinBoundaries() const
    {
        return this->bBox.getMinBoundaries();
    }

private:
    int left, right;
    BoundingBox bBox;
	vector<Node>* nodeList;
	vector<Surface*>* triangles;
	int isLeaf;

    int findBoundary(vector<Surface *> *surfaces, int start, int end, int axis,
                     int midValue)
    {
        for (int i = start; i < end; i++)
        {
            if ((*surfaces)[i]->getCenter()[axis] > midValue)
            {
                return i;
            }
        }
        return start;
    }

};

#endif
