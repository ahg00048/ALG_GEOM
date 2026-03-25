#pragma once

#include "Model3D.h"
#include "Octree.h"

namespace AlgGeom
{
	class DrawOctree : public Model3D
	{
	public:
		DrawOctree() = delete;
		DrawOctree(Octree& oct, bool triangles = false);
		DrawOctree(std::vector<Octree::Node*>& nodes, bool triangles = false);
		~DrawOctree() override {};
	};
};