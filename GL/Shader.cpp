#include "Shader.h"

Shader::Shader(std::string & shaderFile, unsigned shaderType)
: type(shaderType) {
	std::string buffer;
	if(!parseFile(shaderFile, buffer)) {
		std::cerr << "\nError parsing file: '" << shaderFile << "'";
		shader = 0;
		return;
	}

	shader = glCreateShader(type);
	const char * string = buffer.c_str();
	const int len = buffer.length();
	glShaderSource(shader, 1, &string, &len);
	glCompileShader(shader);

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	std::clog << "\n" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			<< " shader compile status: " << (status ? "OK" : "FAIL" );
	int length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	char infoLog[length];
	glGetShaderInfoLog(shader, length, 0, infoLog);
	if(infoLog[0]) std::cerr << "\n*-----\n" << shaderFile << "\n*-----\n" << infoLog << "*-----";
	if(!status) shader = 0;
}

bool Shader::parseFile(std::string & filename, std::string & buffer, bool versionDefined) {
	std::ifstream file;
	file.open(filename.c_str());
	if(!file.is_open()) {
		std::cerr << "\nCannot open file: '" << filename << "'";
		return false;
	}
	std::string line;
	while(std::getline(file, line)) {
		if(line.substr(0, 9) == "#include ") {
			std::string nextFilename;
			try {
				nextFilename = line.substr(9, std::string::npos);
				nextFilename = nextFilename.substr(1, nextFilename.size() - 2);	//get rid of ""
			} catch(std::out_of_range & e) {
				std::cerr << "\nSyntax error at '#include' in file: '" << filename << "' (no file specified?)";
				file.close();
				return false;
			}
			if(!parseFile(nextFilename, buffer, versionDefined)) {
				std::cerr << "\nError parsing file: '" << nextFilename << "'";
				file.close();
				return false;
			}
		} else if(line.substr(0, 8) == "#version") {
			if(!versionDefined) {
				buffer.append(line);
				buffer.append("\n");
				versionDefined = true;
			}
		} else {
			buffer.append(line);
			buffer.append("\n");
		}
	}
	file.close();
	return true;
}

unsigned Shader::getType() {
	return type;
}

bool Shader::isOK() {
	return glIsShader(shader);
}

Shader::~Shader() {
	if(glIsShader(shader)) {
		glDeleteShader(shader);
	}
}
