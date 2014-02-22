#ifndef BBVH_TREE_H
#define BBVH_TREE_H

#include "Surface.h"
#include "Node.h"
#include <vector>

using namespace std;

class BBVHTree : public Surface
{
public:
	vector<Node>* nodes;
    BBVHTree(vector<Surface *> *sceneObjects)
    {
		this->nodes = new vector<Node>();
		this->nodes->push_back(Node(sceneObjects, this->nodes, 0, sceneObjects->size()));
    }

    BBVHTree()
    {

    }

	BBVHTree(vector<Node>* nodes)
	{
		this->nodes = nodes;
	}

    ~BBVHTree()
    {
    }

	vector<Node>* getNodeList(){
		return this->nodes;
	}

    void initializeTree(vector<Surface *> *sceneObjects)
    {
		this->nodes = new vector<Node>();
		this->nodes->push_back(Node(sceneObjects, this->nodes, 0, sceneObjects->size()));
    }

    virtual bool intersect(Ray &ray)
    {
		return this->nodes->at(this->nodes->size() - 1).intersect(ray);
    }
    virtual Vector3 getCenter() const
    {
        return this->nodes->at(this->nodes->size() - 1).getCenter();
    }

    virtual int height()
    {
		return this->nodes->at(this->nodes->size() - 1).height();
    }

    virtual Vector3 getMaxBoundaries() const
    {
		return this->nodes->at(this->nodes->size() - 1).getMaxBoundaries();
    }
    virtual Vector3 getMinBoundaries() const
    {
		return this->nodes->at(this->nodes->size() - 1).getMinBoundaries();
    }

private:

};

#endif
