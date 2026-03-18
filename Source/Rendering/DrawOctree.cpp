#include "stdafx.h"
#include "DrawOctree.h"

AlgGeom::DrawOctree::DrawOctree(Octree& oct)
{
    Component* component = new Component;

    std::vector<Octree::Node*> nodes = oct.getLeafNodes();
    
    for (unsigned i = 0; i < nodes.size(); i++)
    {
        AABB nodeAABB = nodes[i]->getAABB();

        // Geometry
        const vec3 minPosition(nodeAABB.getMin()), maxPosition(nodeAABB.getMax());
        
        const std::vector<vec3> points
        {
            vec3(minPosition[0], minPosition[1], maxPosition[2]),		
            vec3(maxPosition[0], minPosition[1], maxPosition[2]),
            vec3(minPosition[0], minPosition[1], minPosition[2]),	    
            vec3(maxPosition[0], minPosition[1], minPosition[2]),
            vec3(minPosition[0], maxPosition[1], maxPosition[2]),		
            vec3(maxPosition[0], maxPosition[1], maxPosition[2]),
            vec3(minPosition[0], maxPosition[1], minPosition[2]),		
            vec3(maxPosition[0], maxPosition[1], minPosition[2])
        };
        
        for (int pointIdx = 0; pointIdx < points.size(); ++pointIdx)
        {
            component->_vertices.push_back(VAO::Vertex{ points[pointIdx] });
        }

        // Topology
        unsigned vertNBox = 8;

        component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), 
        {   0 + vertNBox * i, 1 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            1 + vertNBox * i, 3 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            2 + vertNBox * i, 0 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            3 + vertNBox * i, 2 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            4 + vertNBox * i, 5 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            5 + vertNBox * i, 7 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            6 + vertNBox * i, 4 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            7 + vertNBox * i, 6 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            0 + vertNBox * i, 4 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            1 + vertNBox * i, 5 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            2 + vertNBox * i, 6 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
            3 + vertNBox * i, 7 + vertNBox * i, RESTART_PRIMITIVE_INDEX });
    }

    component->completeTopology();
    this->_components.push_back(std::unique_ptr<Component>(component)); 
    this->buildVao(component);
}

AlgGeom::DrawOctree::DrawOctree(std::vector<Octree::Node*>& nodes)
{
    Component* component = new Component;

    for (unsigned i = 0; i < nodes.size(); i++)
    {
        AABB nodeAABB = nodes[i]->getAABB();

        // Geometry
        const vec3 minPosition(nodeAABB.getMin()), maxPosition(nodeAABB.getMax());

        const std::vector<vec3> points
        {
            vec3(minPosition[0], minPosition[1], maxPosition[2]),
            vec3(maxPosition[0], minPosition[1], maxPosition[2]),
            vec3(minPosition[0], minPosition[1], minPosition[2]),
            vec3(maxPosition[0], minPosition[1], minPosition[2]),
            vec3(minPosition[0], maxPosition[1], maxPosition[2]),
            vec3(maxPosition[0], maxPosition[1], maxPosition[2]),
            vec3(minPosition[0], maxPosition[1], minPosition[2]),
            vec3(maxPosition[0], maxPosition[1], minPosition[2])
        };

        for (int pointIdx = 0; pointIdx < points.size(); ++pointIdx)
        {
            component->_vertices.push_back(VAO::Vertex{ points[pointIdx] });
        }

        // Topology
        unsigned vertNBox = 8;

        component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(),
            { 0 + vertNBox * i, 1 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                1 + vertNBox * i, 3 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                2 + vertNBox * i, 0 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                3 + vertNBox * i, 2 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                4 + vertNBox * i, 5 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                5 + vertNBox * i, 7 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                6 + vertNBox * i, 4 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                7 + vertNBox * i, 6 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                0 + vertNBox * i, 4 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                1 + vertNBox * i, 5 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                2 + vertNBox * i, 6 + vertNBox * i, RESTART_PRIMITIVE_INDEX,
                3 + vertNBox * i, 7 + vertNBox * i, RESTART_PRIMITIVE_INDEX });
    }

    component->completeTopology();
    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}