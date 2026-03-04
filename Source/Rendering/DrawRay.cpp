#include "stdafx.h"
#include "DrawRay.h"

// Public methods

AlgGeom::DrawRay::DrawRay(RayLine& ray) : Model3D()
{
    double pointAbsDistance = 1000.0;

    Component* component = new Component;
    component->_vertices.insert(component->_vertices.end(), {
            VAO::Vertex { vec3(ray.getA().getX(), ray.getA().getY(), .0f) },
            VAO::Vertex { vec3(ray.getPoint(pointAbsDistance).getX(), ray.getPoint(pointAbsDistance).getY(), .0f)}
        });

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), { 0, 1 });
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();

    this->buildVao(component);
}

AlgGeom::DrawRay::DrawRay(RayLine3d& ray) : Model3D()
{
    double pointAbsDistance = 1000.0;

    Component* component = new Component;
    component->_vertices.insert(component->_vertices.end(), {
            VAO::Vertex { vec3(ray.getOrigin().getX(), ray.getOrigin().getY(), ray.getOrigin().getZ()) },
            VAO::Vertex { vec3(ray.getPoint(pointAbsDistance).getX(), ray.getPoint(pointAbsDistance).getY(), ray.getPoint(pointAbsDistance).getZ()) }
        });

    component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), { 0, 1 });
    this->_components.push_back(std::unique_ptr<Component>(component));

    component->completeTopology();

    this->buildVao(component);
}