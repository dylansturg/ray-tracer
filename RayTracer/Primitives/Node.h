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
        this->left = NULL;
        this->right = NULL;
    }

    Node(vector<Surface *> *sceneObjects, int startIndex, int endIndex)
    {
        int len = endIndex - startIndex;
        this->boundingBox = BoundingBox(sceneObjects, startIndex, endIndex);
        if (len == 1)
        {
            this->left = (*sceneObjects)[startIndex];
            this->right = NULL;
        }
        else if (len == 2)
        {
            this->left = (*sceneObjects)[startIndex];
            this->right = (*sceneObjects)[startIndex + 1];
        }
        else
        {
            pair<float, int> split = this->boundingBox.getSplit();
            float mid = split.first;
            int axis = split.second;

            sort(sceneObjects->begin() + startIndex,
                 sceneObjects->begin() + endIndex, comparators[axis]);
            int partitionBoundary = findBoundary(sceneObjects, startIndex,
                                                 endIndex, axis, mid);
            if (partitionBoundary == startIndex
                    || partitionBoundary == endIndex)
            {
                partitionBoundary = startIndex + len / 2;
            }


            this->left = new Node(sceneObjects, startIndex, partitionBoundary);
            this->right = new Node(sceneObjects, partitionBoundary, endIndex);
        }

    }

    virtual ~Node()
    {
        if (this->left != NULL){
            delete this->left;
        }
        if (this->right != NULL){
            delete this->right;
        }
    }

    virtual int height()
    {
        int leftHeight = this->left->height();
        int rightHeight = this->right == NULL ? 0 : this->right->height();

        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    virtual bool intersect(Ray &ray)
    {
        if (this->boundingBox.intersect(ray))
        {
            this->left->intersect(ray);
            if (this->right != NULL)
                this->right->intersect(ray);
            return true;
        }
        return false;
    }

    virtual Vector3 getCenter() const
    {
        return this->boundingBox.getCenter();
    }

    BoundingBox getBoundingbox() const
    {
        return this->boundingBox;
    }

    virtual Vector3 getMaxBoundaries() const
    {
        return this->boundingBox.getMaxBoundaries();
    }
    virtual Vector3 getMinBoundaries() const
    {
        return this->boundingBox.getMinBoundaries();
    }

private:
    Surface *left, *right;
    BoundingBox boundingBox;

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
