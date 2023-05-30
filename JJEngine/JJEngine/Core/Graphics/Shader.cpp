/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#include "Shader.h"
#include<glad.h>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "Core/Utils/File.h"
#include "Core/Utils/Log.h"

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(m_ShaderProgram);
}

std::shared_ptr<Shader> Shader::CreateShaderFromString(const ShaderSource& srcs)
{
	return std::shared_ptr<Shader>(new Shader{srcs, false});
}

std::shared_ptr<Shader> Shader::CreateShaderFromFile(const ShaderSource& srcs)
{
	return std::shared_ptr<Shader>(new Shader{ srcs, true});
}


void Shader::Use() const
{
	glUseProgram(m_ShaderProgram);
}

void Shader::SetInt(const std::string& name, const int value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, const float value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloatVector3(const std::string& name, const std::vector<glm::vec3>& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform3fv(location, value.size(), glm::value_ptr(value[0]));
}


void Shader::SetFloat4(const std::string& name, const glm::vec4& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMatVector4(const std::string& name, const std::vector<glm::mat4>& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, value.size(), GL_FALSE, glm::value_ptr(value[0]));
}

unsigned Shader::GetShaderProgram()
{
	return m_ShaderProgram;
}

Shader::Shader()
	:m_ShaderProgram(0)
{

}

Shader::Shader(const ShaderSource& shaderSrc, bool is_file)
	:m_ShaderProgram(0)
{
	m_ShaderProgram = glCreateProgram();

	std::vector<unsigned> shaders;
	for(auto& [type, src]:shaderSrc)
	{
		unsigned shader = 0;
		if(is_file)
		{
			std::vector<std::string> loaded_strings;
			for (const auto& filePath : src)
			{
				loaded_strings.push_back(File::ReadFileToString(filePath));
			}
			shader = CompileShader(type, loaded_strings);
		}
		else
		{
			shader = CompileShader(type, src);
		}
		glAttachShader(m_ShaderProgram, shader);
		shaders.push_back(shader);
	}

	glLinkProgram(m_ShaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		static_assert(true, "ERROR::SHADER::PROGRAM::LINK_FAILED\n");
	}

	//no need only program
	for (auto shader : shaders)
		glDeleteShader(shader);
}


int Shader::GetUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		//std::cout << "error on shader location! - " << name << std::endl;
		EngineLog::Warn("error on shader location! - {}", name);
		return -1;
	}
	return location;
}

unsigned Shader::CompileShader(ShaderType type, const std::vector<std::string>& src)
{
	GLenum glsl_type = GL_NONE;
	switch (type)
	{
	case ShaderType::VertexShader:
		glsl_type = GL_VERTEX_SHADER;
		break;
	case ShaderType::FragmentShader:
		glsl_type = GL_FRAGMENT_SHADER;
		break;
	}

	GLuint shader = glCreateShader(glsl_type);
	const int src_count = static_cast<int>(src.size());
	std::vector<const char*> src_array;
	std::vector<int> src_len;

	for (auto& src_string :src)
	{
		src_array.push_back(src_string.c_str());
		src_len.push_back(static_cast<int>(src_string.length()));
	}

	glShaderSource(shader, src_count, src_array.data(), src_len.data());
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		static_assert(true, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}
	return shader;
}




