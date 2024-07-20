#include "main.h"


titian::GUISection::GUISection(const String& name, const LayerPackage& package)
	: BenchmarkInfo(name)
	, LayerHolder(package)
{}
