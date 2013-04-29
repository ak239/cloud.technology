#pragma once

#include <string>
#include "gl.h"

class GLApp
{
public:
	bool glewInitialize();
	void glutInitialize(int args, char* argv[]);

	int exec();

	const std::string& getLastError() const
	{
		return lastError;
	}

private:
	std::string lastError;
};

