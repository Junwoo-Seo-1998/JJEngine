#pragma once
#include "Core/Scene.h"
#include "Core/Graphics/RenderingLayer.h"

#include "Core/Graphics/VertexArray.h"
#include "Core/Camera.h"
#include "Core/Graphics/Shader.h"
class ShadowScene : public Scene
{
	FreeCamera freeCam;
	RenderingLayer graphics;
	std::shared_ptr<VertexArray> shadowSceneVAO;
	std::shared_ptr<Shader> shader;
public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void OnDisable() override {};
	void AddObject(const Model& obj)
	{
		objectGroups.push_back(obj);
	}
private:
	std::vector<Model> objectGroups;

};