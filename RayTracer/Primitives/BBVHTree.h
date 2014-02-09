#ifndef BBVH_TREE_H
#define BBVH_TREE_H

#include "Surface.h"
#include "Node.h"
#include <vector>

using namespace std;

class BBVHTree : public Surface
{
public:
    BBVHTree(vector<Surface *> *sceneObjects)
    {
        this->root = new Node(sceneObjects, 0, sceneObjects->size());
    }

    BBVHTree()
    {
        this->root = NULL;
    }

    ~BBVHTree()
    {
        if (this->root != NULL)
            delete this->root;
    }

    void initializeTree(vector<Surface *> *sceneObjects)
    {
        this->root = new Node(sceneObjects, 0, sceneObjects->size());
    }

    virtual bool intersect(Ray &ray)
    {
        return this->root->intersect(ray);
    }
    virtual Vector3 getCenter() const
    {
        return this->root->getCenter();
    }

    virtual int height()
    {
        return this->root->height();
    }

    virtual Vector3 getMaxBoundaries() const
    {
        return this->root->getMaxBoundaries();
    }
    virtual Vector3 getMinBoundaries() const
    {
        return this->root->getMinBoundaries();
    }

private:
    Node *root;
};

#endif
