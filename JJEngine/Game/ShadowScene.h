#pragma once
#include "Core/Scene.h"
#include "Core/Graphics/Graphics.h"

#include "Core/Graphics/VertexArray.h"
#include "Core/Camera.h"
#include "Core/Graphics/Shader.h"
class ShadowScene : public Scene
{

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

inline std::shared_ptr<ShadowScene> sc;