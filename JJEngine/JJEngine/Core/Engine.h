/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
class Window;
class JJEngine
{
	JJEngine();

	Window* window{};

public:
	~JJEngine();
	static std::shared_ptr<JJEngine> instance();
	void update();
};

