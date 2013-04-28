#pragma once

#include <boost/filesystem.hpp>
#include "ShaderProgramLoader.h"

/**
* Load shaders from folder
* vertex shader   from "vertex"   subfolder
* fragment shader from "fragment" subfolder
*/
class FSShaderProgramLoader : public ShaderProgramLoader
{
public:
	FSShaderProgramLoader(const std::string& _folder);
	~FSShaderProgramLoader(void);

private:
	bool loadProgramImpl(const GLContext& context);

	std::string   folder;
	ShaderProgram tmpProgram;

	void processFile(const boost::filesystem::path p, GLenum type);  
};

