/*   Copyright (C) 2023 Lidia Ortega Alvarado, Alfonso Lopez Ruiz
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see https://github.com/AlfonsoLRz/AG2223.
*/

#pragma once

#include "Model3D.h"
#include "TriangleModel.h"

namespace AlgGeom
{
	class DrawMesh : public Model3D
	{
	protected:
		Assimp::Importer _assimpImporter;

	protected:
		Model3D::Component* processMesh(aiMesh* mesh, const aiScene* scene, const std::string& folder);
		void processNode(aiNode* node, const aiScene* scene, const std::string& folder);

	public:
		DrawMesh(TriangleModel& triangleModel);
		DrawMesh(std::vector<Triangle3d*>& triangles);
		DrawMesh(std::vector<Triangle3d>& triangles);
		DrawMesh(const DrawMesh& drawMesh) = delete;
		~DrawMesh() override {};

		DrawMesh* loadModelOBJ(const std::string& path);
	};
}

