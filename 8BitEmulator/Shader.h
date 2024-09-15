#pragma once
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace Renderer {

	class __declspec(dllimport) Shader {
	public:

		// TODO: Set basic shader for pixel render

		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);
		Shader();

		void use();

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
	};
}