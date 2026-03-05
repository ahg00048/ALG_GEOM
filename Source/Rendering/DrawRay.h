#pragma once

/* 
 * File:   DrawRay.h
 * Author: lidia
 *
 * Created on 19 de enero de 2021, 20:00
 */

#include "RayLine.h"
#include "RayLine3d.h"
#include "Model3D.h"

namespace AlgGeom
{
    class DrawRay : public Model3D 
    {
    public:
        DrawRay(RayLine& t);
        DrawRay(RayLine3d& t);
        DrawRay(const DrawRay& drawRay) = delete;
        ~DrawRay() override {};
    };
}

