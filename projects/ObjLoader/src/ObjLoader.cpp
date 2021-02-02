#include "ObjLoader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

// Borrowed from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
#pragma region String Trimming

// trim from start (in place)
//so that there aren't any unnescary stuff in your obj (s)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

#pragma endregion 


VertexArrayObject::sptr ObjLoader::OBJLoader(const std::string& filename)
{
	//Vertext portions
	//initilializing variables
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_vertexTexture;
	std::vector< glm::vec3 > temp_vertexNormals;

	//Face vectors
	//initilializing variables
	std::vector< unsigned int > vertexIndices, texcoordIndices, normalIndices;

	//inilializes mesh and files
	MeshBuilder<VertexPosNormTexCol> mesh;
	std::string line;

	std::ifstream fileName;
	fileName.open(filename, std::ios::binary);

	// If our file fails to open, we will throw an error
	if (!fileName) {
		throw std::runtime_error("Failed to open file");
	}

	//Reading one line a time
	while (std::getline(fileName, line))
	{
		trim(line);
		if (line.substr(0, 1) == "#")
		{
			// Comment, do nothing
		}
		else if (line.substr(0, 2) == "v ") //Vertex position
		{
			std::istringstream ss = std::istringstream(line.substr(2));
			glm::vec3 vert;
			ss >> vert.x >> vert.y >> vert.z;
			temp_vertices.push_back(vert); //inputs the vertices from the obj to  vec3
			
		}
		else if (line.substr(0, 3) == "vt ")
		{
			std::istringstream ss = std::istringstream(line.substr(2));
			glm::vec2 vt;
			ss >> vt.x >> vt.y;
			temp_vertexTexture.push_back(vt); //Vertext texture

		}
		else if (line.substr(0, 3) == "vn ") //Vertext normal
		{
			std::istringstream ss = std::istringstream(line.substr(2));
			glm::vec3 vn;
			ss >> vn.x >> vn.y >> vn.z;
			temp_vertexNormals.push_back(vn);

		}
		else if (line.substr(0, 2) == "f ") //Faces
		{
			std::string fileLineSubstr = line.substr(2);
			std::string fileLineWithoutSlashes = fileLineSubstr;

			//Remove all of the slashes
			for (int i = 0; i < fileLineWithoutSlashes.length(); i++)
			{
				if (fileLineWithoutSlashes[i] == '/')
					fileLineWithoutSlashes[i] = ' ';
			}

			std::istringstream ss = std::istringstream(fileLineWithoutSlashes);
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], texcoordIndex[3], normalIndex[3];

			ss >> vertexIndex[0] >> texcoordIndex[0] >> normalIndex[0]
			   >> vertexIndex[1] >> texcoordIndex[1] >> normalIndex[1] 
			   >> vertexIndex[2] >> texcoordIndex[2] >> normalIndex[2];
			
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			texcoordIndices.push_back(texcoordIndex[0]);
			texcoordIndices.push_back(texcoordIndex[1]);
			texcoordIndices.push_back(texcoordIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		
	}

	//puts all the vertices, faces, normals in to the addvertex and addinex
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];

		unsigned int texcoordIndex = texcoordIndices[i];
		glm::vec2 texcoord = temp_vertexTexture[vertexIndex - 1];

		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_vertexNormals[vertexIndex - 1];

		mesh.AddVertex(vertex, normal, texcoord, glm::vec4(1));
		mesh.AddIndex(i);
	}

	//Loaded Object
	return mesh.Bake();
}

