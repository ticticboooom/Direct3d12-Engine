#pragma once
#include "Structures.h"
#include <fstream>
#include <ios>
#include <sstream>
class BinaryConverter
{
public:
	BinaryConverter(Structures::VerticesIndices data, std::string outputPath);
	~BinaryConverter();
private:
	void WriteVertices(std::shared_ptr<std::vector<Structures::VertexTexCoordNormal>> data);
	void WriteIndices(std::shared_ptr<std::vector<unsigned long>> data);
	void WriteAnimations(std::shared_ptr<Structures::AnimationForExport> data, UINT frameCount);
	std::ostringstream m_data;
	std::ofstream m_file;
};

