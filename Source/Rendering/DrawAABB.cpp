#include "stdafx.h"
#include "DrawAABB.h"


AlgGeom::DrawAABB::DrawAABB(AABB& aabb) : Model3D()
{
    Component* component = this->getVoxel();
    vec3 aabbCenter(aabb.getCenter());
    vec3 extent(aabb.getExtent());

    for (VAO::Vertex& vertex : component->_vertices)
    {
        vertex._position = (vertex._position * 2.0f * extent) + aabbCenter;
    }

    this->buildVao(component);
    this->_components.push_back(std::unique_ptr<Component>(component));        
}