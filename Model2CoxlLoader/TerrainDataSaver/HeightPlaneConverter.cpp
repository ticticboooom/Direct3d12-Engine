#include "stdafx.h"
#include "HeightPlaneConverter.h"
#include "Structures.h"
#include <fstream>
#include <ios>
#include <sstream>
HeightPlaneConverter::HeightPlaneConverter(Structures::VerticesIndices data, std::string outPath)
{
	auto vertices = data.vertices;
	auto minX = (*vertices)[0].pos.x;
	auto maxX = (*vertices)[0].pos.x;
	for (auto& vertex : *vertices) {
		if (vertex.pos.x < minX) {
			minX = vertex.pos.x;
		}
		if (vertex.pos.x > maxX) {
			maxX = vertex.pos.x;
		}
	}
	auto adder = abs(minX);
	auto sqrt = std::sqrt(vertices->size() - 1);
	auto sizeX = (maxX + adder) * sqrt;
	auto heightMap = std::make_shared<std::vector<std::vector<float>>>(sizeX);
	for (auto& z : *heightMap) {
		z = std::vector<float>(sizeX);
	}

	for (auto& vertex : *vertices) {
		auto gridX = static_cast<int>((vertex.pos.x + adder) * sqrt - 1);
		auto gridZ = static_cast<int>((vertex.pos.x + adder) * sqrt - 1);

		(*heightMap)[gridX][gridZ] = vertex.pos.y;
	}

	
	auto file = std::ofstream(outPath, std::ios::binary | std::ios::out);
	for (auto z : *heightMap) {
		file.write(reinterpret_cast<char*>(z.data()), sizeof(float) * z.size());
	}
	file.close();
}


HeightPlaneConverter::~HeightPlaneConverter()
{
}
