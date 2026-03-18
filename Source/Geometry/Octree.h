#pragma once

#include <vector>
#include <array>
#include <stack>

#include "Triangle3d.h"
#include "TriangleModel.h"
#include "RandomUtilities.h"
#include "Vect3d.h"

class Octree
{
public:
	class Node
	{
	public:
		enum TypeColorNode
		{
			WHITE = 0,
			BLACK,
			GRAY
		};

		const static unsigned int N_CHILDREN = 8;
	
	private:
		std::vector<Triangle3d*> _triangles;
		Vect3d _min;
		Vect3d _max;
		int _level;
		Octree* _oct;
		Node* _children;
		TypeColorNode _type;

	private:
		void createChildren();

	public:
		Node();
		Node(int level, Vect3d min, Vect3d max, Octree* oct);
		~Node();

		const Vect3d& getMin() const { return _min; }
		const Vect3d& getMax() const { return _max; }
		Node* getChildren() const{ return _children; }
		int getLevel() const { return _level; }
		TypeColorNode getType() const { return _type; }
		
		AABB getAABB() const { return AABB(_min, _max); }

		void setMin(const Vect3d& min) { _min = min; }
		void setMax(const Vect3d& max) { _max = max; }
		void setOctree(Octree* oct) { _oct = oct; }

		void insertTriangle(Triangle3d* tri);
		
		bool hasChildren();
		void destroyChildren();
		void classifyColor(TriangleModel& model);

		bool operator==(const Node& other);
	};

private:
	std::vector<Triangle3d> _triangles;
	Node _root;
	bool _optimized;
	TriangleModel* _model;

public:
	static const unsigned int MAX_TRI_NODE = 40;
	static const unsigned int MAX_LEVELS = 8;

public:
	Octree();
	Octree(TriangleModel& model);
	~Octree();

	void clear();

	bool isOptimized() const { return _optimized; }

	void setOptimized(bool opt) { _optimized = opt; }
	
	void insertTriangle(Triangle3d& tri) { _root.insertTriangle(&tri); }
	bool isInsideModel(const Vect3d& v);
	void classifyColor();

	std::vector<Node*> getLeafNodes();
};

