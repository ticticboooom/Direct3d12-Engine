#pragma once
#include<string>
#include<ios>
#include<fstream>
#include "Structures.h"

class BinaryModelLoader
{
public:
	D3DENGINE_API BinaryModelLoader(const char* filePath, bool hasAnimations);
	D3DENGINE_API ~BinaryModelLoader();
	D3DENGINE_API std::shared_ptr<Structures::VerticesIndicesFromBin> GetFinalData() const { return m_finalData; };
private:
	std::shared_ptr<Structures::VerticesIndicesFromBin> m_finalData;
};

