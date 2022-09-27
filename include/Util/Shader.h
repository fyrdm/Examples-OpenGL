#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Util
{
	class Shader
	{
    public:
        // the program ID
        unsigned int ID;
        std::string vertexCodeStr;
        std::string fragmentCodeStr;

        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);
        // use/activate the shader
        void use();
        // utility uniform functions
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;

        void printSourceCode();
    private:
        void checkCompileErrors(unsigned int shader, std::string type);
	};
}

