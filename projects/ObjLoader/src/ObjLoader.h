#pragma once
#include "MeshFactory.h"
#include "MeshBuilder.h"

class ObjLoader
{
public:
	static VertexArrayObject::sptr OBJLoader(const std::string& filename);
	
protected:
	ObjLoader() = default;
	~ObjLoader() = default;
};

