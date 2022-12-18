/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/17/2022
End Header-------------------------------------------------------- */
#pragma once
class Scene
{
public:
	Scene() {};
	Scene(std::string) {};
	virtual ~Scene() {};
	virtual void Load() = 0;
	virtual void Update(double dt) = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;
private:

};