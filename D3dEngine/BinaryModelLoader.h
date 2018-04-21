#pragma once
#include<string>
#include<ios>
#include<fstream>
#include "Structures.h"

/**
 * @brief loads a file from a given directory
 * Used with coxl files as this is what it is set up to do
 * 
 */
class D3DENGINE_API BinaryModelLoader
{
public:
	 BinaryModelLoader(const char* filePath, bool hasAnimations);
	~BinaryModelLoader();
	std::shared_ptr<Structures::VerticesIndicesFromBin> GetFinalData() const { return m_finalData; };
private:
	std::shared_ptr<Structures::VerticesIndicesFromBin> m_finalData; /// The final data that can be retrieved
};

