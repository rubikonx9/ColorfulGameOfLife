#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <GL3/gl3w.h>
#include "GL.h"

class Shader {
public:
	unsigned shader;
	unsigned type;

	Shader(std::string & shaderFile, unsigned shaderType);
	bool isOK();
	unsigned getType();
	virtual ~Shader();
protected:
	bool parseFile(std::string & filename, std::string & buffer, bool versionDefined = false);
};

#endif /* SHADER_H_ */
