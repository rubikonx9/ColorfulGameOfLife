#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <string>
#include <GL/gl3w.h>

#include "Shader.h"

class Program {
protected:
	bool checkShader(Shader & shader);
public:
	unsigned program;

	Program(std::string vertexShaderFile, std::string fragmentShaderFile);
	bool isOK();
	bool validate();
	void use();
	void dontUse();
	int uniform(std::string uniformName);
	int attribute(std::string attributeName);
	virtual ~Program();
};

#endif /* PROGRAM_H_ */
