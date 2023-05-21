#pragma once
#include "Core/Scene.h"
#include "Core/Graphics/Graphics.h"

#include "Core/Graphics/VertexArray.h"
#include "Core/Camera.h"
#include "Core/Graphics/Shader.h"
class ShadowScene : public Scene
{
	FreeCamera freeCam;
	Graphics graphics;
	std::shared_ptr<VertexArray> shadowSceneVAO;
	std::shared_ptr<Shader> shader;
public:
	virtual void Load() override;
	virtual void Update(double dt) override;
	virtual void Draw() override;
	virtual void Unload() override {};
	void AddObject(const Model& obj)
	{
		objectGroups.push_back(obj);
	}
private:
	std::vector<Model> objectGroups;

};