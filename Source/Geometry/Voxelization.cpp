#include "stdafx.h"

#include "Voxelization.h"


// Public methods

//Voxelization::Voxelization(TriangleModel* model, const vec3& voxelSize): _model(model), _voxelSize(voxelSize)
//{
//	AABB aabb = model->getAABB();
//	vec3 aabbSize = aabb.size();
//	_splits = glm::ceil(aabbSize / voxelSize);
//	_aabb = AABB(aabb.center() - vec3(_splits) / vec3(2.0f) * _voxelSize, aabb.center() + vec3(_splits) / vec3(2.0f) * _voxelSize);
//
//	_voxel = new Voxel**[_splits.x];
//	for (unsigned x = 0; x < _splits.x; ++x)
//	{
//		_voxel[x] = new Voxel*[_splits.y];
//		for (unsigned y = 0; y < _splits.y; ++y)
//		{
//			_voxel[x][y] = new Voxel[_splits.z];
//			for (unsigned z = 0; z < _splits.z; ++z)
//			{
//				_voxel[x][y][z]._aabb = AABB(_aabb.min() + vec3(x, y, z) * _voxelSize, _aabb.min() + vec3(x + 1, y + 1, z + 1) * _voxelSize);
//			}
//		}
//	}
//}

Voxelization::~Voxelization()
{
	for (unsigned x = 0; x < _splits.x; ++x)
	{
		for (unsigned y = 0; y < _splits.y; ++y)
		{
			delete[] _voxel[x][y];
		}
		delete _voxel[x];
	}
	delete[] _voxel;
}

void Voxelization::printData() const
{
	unsigned numOccupiedVoxels = 0;
	unsigned numInnerVoxels = 0;
	unsigned numOuterVoxels = 0;

	for (unsigned x = 0; x < _splits.x; ++x)
	{
		for (unsigned y = 0; y < _splits.y; ++y)
		{
			for (unsigned z = 0; z < _splits.z; ++z)
			{
				numOccupiedVoxels += static_cast<unsigned>(_voxel[x][y][z]._status == Voxel::OCCUPIED);
				numInnerVoxels += static_cast<unsigned>(_voxel[x][y][z]._status == Voxel::INNER);
				numOuterVoxels += static_cast<unsigned>(_voxel[x][y][z]._status == Voxel::OUTER);
			}
		}
	}

	std::cout << "Number of filled voxels: " << numOccupiedVoxels << std::endl;
	std::cout << "Number of inner voxels: " << numInnerVoxels << std::endl;
	std::cout << "Number of outer voxels: " << numOuterVoxels << std::endl;
}

// Protected methods

ivec3 Voxelization::getIndices(const vec3& point) const
{
	Vect3d aabbMin = _aabb.getMin();
	vec3 aabbMinVec3(aabbMin.getX(), aabbMin.getY(), aabbMin.getZ());
	
	return ivec3((point - aabbMinVec3) / _voxelSize);
}

void Voxelization::insertPoint(const vec3& point, unsigned triangleIdx) const
{
	ivec3 indices = this->getIndices(point);
	_voxel[indices.x][indices.y][indices.z]._triangles.insert(triangleIdx);
	_voxel[indices.x][indices.y][indices.z]._status = Voxel::OCCUPIED;
}
