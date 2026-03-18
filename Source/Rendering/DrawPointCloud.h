#pragma once

/* 
 * File:   DrawPointCloud.h
 * Author: lidia
 *
 * Created on 20 de enero de 2021, 13:29
 */

#include "PointCloud.h"
#include "PointCloud3d.h"
#include "Model3D.h"

namespace AlgGeom
{
    class DrawPointCloud : public Model3D 
    {
    public:
        DrawPointCloud(PointCloud& t);
        DrawPointCloud(PointCloud3d& t);
        DrawPointCloud(std::vector<Vect3d>& points);
        DrawPointCloud(const DrawPointCloud& drawPointCloud) = delete;
        ~DrawPointCloud() override {};
    };
}

