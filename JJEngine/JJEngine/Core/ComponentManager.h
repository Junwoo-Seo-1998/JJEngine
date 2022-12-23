/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/21/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include <vector>

class ComponentManager {
	struct Components;
	std::shared_ptr<Components> components{};
public:
	ComponentManager();
	~ComponentManager();
	template<typename T>
	void AddComponent(T& data);

	template<typename T>
	void RemoveComponents();
	template<typename T>
	std::vector<T&> GetComponents();

	void Update(double dt);
};
