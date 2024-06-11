#include "main.h"


titian::GUISection::GUISection(const std::string& name, const LayerPackage& package)
	: BenchmarkInfo(name)
	, LayerHolder(package)
{}
