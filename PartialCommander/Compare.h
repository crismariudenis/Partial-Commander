#include "Folder.h"

#pragma once


class Compare
{
public:
	bool nameCompare(Folder a, Folder b);
	bool sizeCompare(Folder a, Folder b);
	bool timeCompare(Folder a, Folder b);
};