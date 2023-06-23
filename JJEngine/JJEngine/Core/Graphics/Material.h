#pragma once
#include <any>
#include <memory>
#include <string>
#include <unordered_map>
class Texture;
class Shader;

class Material
{
public:
	static std::shared_ptr<Material> Create(std::shared_ptr<Shader> shader);
	Material(std::shared_ptr<Shader> shader);
	virtual ~Material();
	//will be changed later so that we can use without knowing slots
	void Bind(int textureSlotStart = 0);

	template<typename T>
	void Set(const std::string& name, const T& value)
	{
		m_MaterialValues[name] = value;
	}

	void Set(const std::string& name, const std::shared_ptr<Texture>& texture);

	std::shared_ptr<Shader> GetShader();

private:
	void BindTextures(int textureSlotStart);

	std::shared_ptr<Shader> m_Shader;
	//for now, will be changed using buffer
	std::unordered_map<std::string, std::any> m_MaterialValues;

	//for now, will be changed later using buffer
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
};
