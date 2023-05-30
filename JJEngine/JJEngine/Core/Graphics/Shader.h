/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#pragma once
#include<string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

enum class ShaderType
{
	VertexShader=0,
	FragmentShader,
	//TessellationShader,
	//GeometryShader,
	TypeCounts,
};


using ShaderSource = std::unordered_map<ShaderType, std::vector<std::string>>;


class Shader
{
public:
	~Shader();
	static std::shared_ptr<Shader> CreateShaderFromString(const ShaderSource& srcs);
	static std::shared_ptr<Shader> CreateShaderFromFile(const ShaderSource& srcs);

	void Use() const;
	void SetInt(const std::string& name, const int value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetFloat3(const std::string& name, const glm::vec3& value) const;
	void SetFloatVector3(const std::string& name, const std::vector<glm::vec3>& value) const;
	void SetFloat4(const std::string& name, const glm::vec4& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetMatVector4(const std::string& name, const std::vector<glm::mat4>& value) const;

	//void SetTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned int unit = 0);
	unsigned GetShaderProgram();
private:
	Shader();
	Shader(const ShaderSource& shaderSrc, bool is_file);

	int GetUniformLocation(const std::string& name) const;
	unsigned CompileShader(ShaderType type, const std::vector<std::string>& src);
	unsigned m_ShaderProgram;
};

