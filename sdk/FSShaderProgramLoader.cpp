#include "FSShaderProgramLoader.h"

#include <boost/filesystem.hpp>
#include <iostream>

namespace
{
	const std::string VertexFile   = "vertex";
	const std::string FragmentFile = "fragment";
}

FSShaderProgramLoader::FSShaderProgramLoader(const std::string& _folder) : folder(_folder)
{
}


FSShaderProgramLoader::~FSShaderProgramLoader(void)
{
}

bool FSShaderProgramLoader::loadProgramImpl(const GLContext& context)
{
	using namespace boost::filesystem;

	tmpProgram = ShaderProgram(context);

	path p = path(folder);
	if (!exists(p) || !is_directory(folder)){
		setLastError(folder + " not exists!");
		return false;
	}

	for (auto file = directory_iterator(p); file != directory_iterator(); ++file){
		path filePath = file->path();
		if (is_regular_file(filePath)){
			GLenum type;
			if (filePath.filename().string().substr(0, VertexFile.length()) == VertexFile){
				type = GL_VERTEX_SHADER;
			}else if (filePath.filename().string().substr(0, FragmentFile.length()) == FragmentFile){
				type = GL_FRAGMENT_SHADER;
			}else
				continue;
			if (!processFile(filePath, type)) {
				return false;
			}
		}
	}

	if (!tmpProgram.compile()){
		setLastError(getLastError() + "\r\n" + "Can't compile program (" + std::string(p.generic_string()) + "): " + tmpProgram.getLastError());
		return false;
	}

	setProgram(tmpProgram);

	return true;
}

bool FSShaderProgramLoader::processFile(const boost::filesystem::path p, GLenum type)
{
	using namespace boost::filesystem;

	std::ifstream file;
	file.open(canonical(p).generic_string(), std::ofstream::in);
	if (!file.is_open()){
		setLastError(getLastError() + "\r\n" + "Can't open file: " + std::string(canonical(p).generic_string()));
		return false;
	}

	std::string str;
	std::string text;

    while(std::getline(file, str))
		text.append(str + "\n");
	
	file.close();

	Shader shader(text, type);
	if (!shader.compile()){
		setLastError(getLastError() + "\r\n" + "Can't compile shader (" + std::string(p.generic_string()) + "): " + shader.getLastError());
		return false;
	}

	tmpProgram.attachShader(shader);
	return true;
}
