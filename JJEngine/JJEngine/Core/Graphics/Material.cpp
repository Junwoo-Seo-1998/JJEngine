#include "Material.h"

#include "glad.h"
#include "Shader.h"
#include "Texture.h"

#include "Core/Utils/Assert.h"
std::shared_ptr<Material> Material::Create(std::shared_ptr<Shader> shader)
{
	return std::make_shared<Material>(shader);
}

Material::Material(std::shared_ptr<Shader> shader)
	:m_Shader(shader)
{
}

Material::~Material()
{
}

void Material::Bind(int textureSlotStart)
{
	m_Shader->Use();

	for (auto& [name, value]:m_MaterialValues)
	{
		//changed later
		{
			auto& type = value.type();

			if (type == typeid(int))
				m_Shader->SetInt(name, std::any_cast<int>(value));
			else if (type == typeid(float))
				m_Shader->SetFloat(name, std::any_cast<float>(value));
			else if (type == typeid(glm::vec3))
				m_Shader->SetFloat3(name, std::any_cast<glm::vec3>(value));
			else if (type == typeid(glm::vec4))
				m_Shader->SetFloat4(name, std::any_cast<glm::vec4>(value));
			else
				ENGINE_ASSERT(false, "Not Supported Type");
		}
	}

	BindTextures(textureSlotStart);
}

void Material::Set(const std::string& name, const std::shared_ptr<Texture>& texture)
{
	m_Textures[name] = texture;
}

std::shared_ptr<Shader> Material::GetShader()
{
	return m_Shader;
}

void Material::BindTextures(int textureSlotStart)
{
	for (auto& [name, texture]:m_Textures)
	{
		texture->BindTexture(textureSlotStart);
		m_Shader->SetInt(name, textureSlotStart);
		++textureSlotStart;
	}
}
