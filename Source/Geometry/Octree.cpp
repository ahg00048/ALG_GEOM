#include "stdafx.h"
#include "Octree.h"

// Node

Octree::Node::Node(int level, Vect3d min, Vect3d max, Octree* oct) :
	_level(level), _min(min), _max(max), _oct(oct), _type(TypeColorNode::WHITE), _children(nullptr)
{

}

Octree::Node::Node() :
	_level(-1), _min(), _max(), _oct(nullptr), _type(TypeColorNode::WHITE), _children(nullptr)
{

}

Octree::Node::~Node()
{
	_oct = nullptr;

	if (_children)
	{
		delete _children;
		_children = nullptr;
	}
}

bool Octree::Node::operator==(const Node& other)
{
	bool result = (_min == other._min) && (_max == other._max) && (_level == other._level);
	
	return result;
}

void Octree::Node::destroyChildren()
{
	delete[] _children;

	_children = nullptr;
}

void Octree::Node::createChildren()
{
	double minX = _min.getX(); 
	double minY = _min.getY();
	double minZ = _min.getZ();
	double maxX = _max.getX(); 
	double maxY = _max.getY();
	double maxZ = _max.getZ();
	
	double medX = (maxX + minX) / 2; 
	double medY = (maxY + minY) / 2;
	double medZ = (maxZ + minZ) / 2;

	_children = new Node[Node::N_CHILDREN];

	_children[6] = Node(_level + 1, Vect3d(minX, medY, medZ), Vect3d(medX, maxY, maxZ), _oct);
	_children[4] = Node(_level + 1, Vect3d(minX, medY, minZ), Vect3d(medX, maxY, medZ), _oct);
	_children[5] = Node(_level + 1, Vect3d(medX, medY, minZ), Vect3d(maxX, maxY, medZ), _oct);
	_children[7] = Node(_level + 1, Vect3d(medX, medY, medZ), Vect3d(maxX, maxY, maxZ), _oct);
	_children[2] = Node(_level + 1, Vect3d(minX, minY, medZ), Vect3d(medX, medY, maxZ), _oct);
	_children[0] = Node(_level + 1, Vect3d(minX, minY, minZ), Vect3d(medX, medY, medZ), _oct);
	_children[1] = Node(_level + 1, Vect3d(medX, minY, minZ), Vect3d(maxX, medY, medZ), _oct);
	_children[3] = Node(_level + 1, Vect3d(medX, minY, medZ), Vect3d(maxX, medY, maxZ), _oct);
}

bool Octree::Node::hasChildren() 
{
	return _children;
}

void Octree::Node::insertTriangle(Triangle3d* tri)
{
	if (_level == Octree::MAX_LEVELS)
	{
		_triangles.push_back(tri);
	
		if (_oct->isOptimized() && _triangles.size() > Octree::MAX_TRI_NODE)
			_oct->setOptimized(false);
	}
	else if (hasChildren()) 
	{
		for (int i = 0; i < Octree::Node::N_CHILDREN; i++)
		{
			AABB aabb = _children[i].getAABB();

			if (tri->triAABB(aabb))
			{
				_children[i].insertTriangle(tri);
			}
		}
	}
	else if (_triangles.size() == Octree::MAX_TRI_NODE)
	{
		createChildren();
	
		for (int i = 0; i < Octree::Node::N_CHILDREN; i++)
		{
			AABB aabb = _children[i].getAABB();

			for (Triangle3d* nTri : _triangles)
			{
				if (nTri->triAABB(aabb))
				{
					_children[i].insertTriangle(nTri);
				}
			}

			if (tri->triAABB(aabb))
			{
				_children[i].insertTriangle(tri);
			}
		}

		_triangles.clear();
	}
	else
	{
		_triangles.push_back(tri);
	}
}

void Octree::Node::classifyColor(TriangleModel& model)
{
	if (!_triangles.empty())
	{
		_type = TypeColorNode::GRAY;
		return;
	}

	AABB aabb(_min, _max);
	Vect3d center = aabb.getCenter();
	Vect3d dest1(RandomUtilities::getUniformRandom(_min.getX(), _max.getX()),
		RandomUtilities::getUniformRandom(_min.getY(), _max.getY()),
		RandomUtilities::getUniformRandom(_min.getZ(), _max.getZ()));
	Vect3d dest2(RandomUtilities::getUniformRandom(_min.getX(), _max.getX()),
		RandomUtilities::getUniformRandom(_min.getY(), _max.getY()),
		RandomUtilities::getUniformRandom(_min.getZ(), _max.getZ()));

	RayLine3d r1(center, dest1);
	RayLine3d r2(center, dest2);

	unsigned int r1NInter = model.rayTraversal(r1).size();
	unsigned int r2NInter = model.rayTraversal(r2).size();

	if (r1NInter % 2 == 0 && r2NInter % 2 == 0) // Par
	{
		_type = TypeColorNode::WHITE;
		return;
	} 
	else if (r1NInter % 2 != 0 && r2NInter % 2 != 0) // Impar
	{
		_type = TypeColorNode::BLACK;
		return;
	} 	

	Vect3d dest3(RandomUtilities::getUniformRandom(_min.getX(), _max.getX()),
		RandomUtilities::getUniformRandom(_min.getY(), _max.getY()),
		RandomUtilities::getUniformRandom(_min.getZ(), _max.getZ()));
	
	RayLine3d r3(center, dest3);
	
	r1NInter = model.rayTraversal(r3).size();

	if (r1NInter % 2 == 0)
	{
		_type = TypeColorNode::WHITE;
	} 
	else
	{
		_type = TypeColorNode::BLACK;
	}
}

// Octree

Octree::Octree() :
	_triangles(), _root(), _optimized(false)
{

}

Octree::Octree(TriangleModel& model) :
	_optimized(false), _model(&model), _root()
{
	_triangles = _model->getFaces();

	Vect3d min = (*_triangles.begin()).getA();
	Vect3d max = (*_triangles.begin()).getA();

	for (Triangle3d& tri : _triangles)
	{
		AABB triAABB = tri.getAABB();

		Vect3d aabbMin = triAABB.getMin();
		Vect3d aabbMax = triAABB.getMax();
		
		// MIN
		if (aabbMin.getX() < min.getX())
			min.setX(aabbMin.getX());
		if (aabbMin.getY() < min.getY())
			min.setY(aabbMin.getY());
		if (aabbMin.getZ() < min.getZ())
			min.setZ(aabbMin.getZ());

		// MAX
		if (aabbMax.getX() > max.getX())
			max.setX(aabbMax.getX());
		if (aabbMax.getY() > max.getY())
			max.setY(aabbMax.getY());
		if (aabbMax.getZ() > max.getZ())
			max.setZ(aabbMax.getZ());
	}
	
	_root = Octree::Node(0, min, max, this);

	for (Triangle3d& tri : _triangles)
	{
		_root.insertTriangle(&tri);
	}
}

Octree::~Octree()
{
	clear();
	_model = nullptr;
}

void Octree::classifyColor()
{
	std::vector<Octree::Node*> nodes = getLeafNodes();

	unsigned int i = 0;
	for (Octree::Node* n : nodes)
	{	
		n->classifyColor(*_model);
	}
}

void Octree::clear()
{
	std::stack<Octree::Node*> nodes;
	std::vector<Octree::Node*> vNodes;
	nodes.push(&_root);

	while (!nodes.empty())
	{
		Node* curr = nodes.top();
	
		while (curr->hasChildren() && std::find(vNodes.begin(), vNodes.end(), curr) == vNodes.end())
		{
			Node* children = curr->getChildren();
			for (int i = 0; i < Octree::Node::N_CHILDREN; i++)
			{
				nodes.push(&children[i]);
			}

			vNodes.push_back(curr);
			curr = nodes.top();
		}

		if (curr->hasChildren()) 
			curr->destroyChildren();
		
		curr = nullptr;

		nodes.pop();
	}
}

bool Octree::isInsideModel(const Vect3d& v)
{
	bool isInside = true;
	Node* curr = &_root;

	Vect3d min = curr->getAABB().getMin();
	Vect3d max = curr->getAABB().getMax();

	if (!(v.getX() >= min.getX() && v.getX() <= max.getX() &&
		v.getY() >= min.getY() && v.getY() <= max.getY() &&
		v.getZ() >= min.getZ() && v.getZ() <= max.getZ()))
	{
		isInside = false;
	}

	while (isInside && curr->hasChildren())
	{
		isInside = false;

		Node* children = curr->getChildren();
		for (int i = 0; i < Octree::Node::N_CHILDREN; i++)
		{
			min = children[i].getAABB().getMin();
			max = children[i].getAABB().getMax();

			if (v.getX() >= min.getX() && v.getX() <= max.getX() &&
				v.getY() >= min.getY() && v.getY() <= max.getY() &&
				v.getZ() >= min.getZ() && v.getZ() <= max.getZ())
			{
				curr = &children[i];
				isInside = true;
				break;
			}
		}
	}

	if (curr->getType() == Node::TypeColorNode::BLACK)
	{
		return true;
	}
	else if (curr->getType() == Node::TypeColorNode::WHITE)
	{
		return false;
	}
	else
	{
		AABB aabb = curr->getAABB();
		Vect3d dest1(RandomUtilities::getUniformRandom(aabb.getMin().getX(), aabb.getMax().getX()),
					RandomUtilities::getUniformRandom(aabb.getMin().getY(), aabb.getMax().getY()),
					RandomUtilities::getUniformRandom(aabb.getMin().getZ(), aabb.getMax().getZ()));
		Vect3d dest2(RandomUtilities::getUniformRandom(aabb.getMin().getX(), aabb.getMax().getX()),
					RandomUtilities::getUniformRandom(aabb.getMin().getY(), aabb.getMax().getY()),
					RandomUtilities::getUniformRandom(aabb.getMin().getZ(), aabb.getMax().getZ()));
		
		Vect3d vAux = v;

		RayLine3d r1(vAux, dest1);
		RayLine3d r2(vAux, dest2);

		unsigned int r1NInter = _model->rayTraversal(r1).size();
		unsigned int r2NInter = _model->rayTraversal(r2).size();
		
		if (r1NInter % 2 == 0 && r2NInter % 2 == 0) // Par
		{
			return false;
		}
		else if (r1NInter % 2 != 0 && r2NInter % 2 != 0) // Impar
		{
			return true;
		}

		Vect3d dest3(RandomUtilities::getUniformRandom(aabb.getMin().getX(), aabb.getMax().getX()),
			RandomUtilities::getUniformRandom(aabb.getMin().getY(), aabb.getMax().getY()),
			RandomUtilities::getUniformRandom(aabb.getMin().getZ(), aabb.getMax().getZ()));

		RayLine3d r3(vAux, dest3);

		r1NInter = _model->rayTraversal(r3).size();

		if (r1NInter % 2 == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

std::vector<Octree::Node*> Octree::getLeafNodes()
{
	std::vector<Octree::Node*> result;
	std::stack<Octree::Node*> nodes;
	std::vector<Octree::Node*> vNodes;
	nodes.push(&_root);

	while (!nodes.empty())
	{
		Node* curr = nodes.top();

		while (curr->hasChildren() && std::find(vNodes.begin(), vNodes.end(), curr) == vNodes.end())
		{
			Node* children = curr->getChildren();
			for (int i = 0; i < Octree::Node::N_CHILDREN; i++)
			{
				nodes.push(&children[i]);
			}

			vNodes.push_back(curr);
			curr = nodes.top();
		}

		if (!curr->hasChildren())
			result.push_back(curr);

		curr = nullptr;

		nodes.pop();
	}

	return result;
}