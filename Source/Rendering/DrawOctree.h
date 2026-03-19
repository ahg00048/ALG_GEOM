#pragma once

#include "Model3D.h"
#include "Octree.h"

namespace AlgGeom
{
	class DrawOctree : public Model3D
	{
	public:
		DrawOctree() = delete;
		DrawOctree(Octree& oct);
		DrawOctree(std::vector<Octree::Node*>& nodes);
		~DrawOctree() override {};
	};
};