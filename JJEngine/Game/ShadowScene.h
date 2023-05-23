#pragma once
#include "Core/Scene.h"
#include "Core/Graphics/RenderingLayer.h"

#include "Core/Graphics/VertexArray.h"
#include "Core/Camera.h"
#include "Core/Graphics/Shader.h"
class ShadowScene : public Scene
{
	FreeCamera freeCam;
	FreeCamera light1;
	FreeCamera light2;

	RenderingLayer graphics;
	std::shared_ptr<VertexArray> shadowSceneVAO;
	std::shared_ptr<FrameBuffer> shadowSceneSampleFBO;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> shadowShader;

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