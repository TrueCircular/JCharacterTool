#include "pch.h"
#include "Converter.h"

Converter::Converter()
{
	_importer = make_shared<Assimp::Importer>();
}

Converter::~Converter()
{
}
void Converter::ReadAssetFile(wstring file)
{
}

