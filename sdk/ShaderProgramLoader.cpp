#include "ShaderProgramLoader.h"

bool ShaderProgramLoader::loadProgram(const GLContext& context)
{
	GLContextGetter get(context);
	bool isOk = loadProgramImpl(context);
	return isOk;
}