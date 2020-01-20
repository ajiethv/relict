#pragma once

#include "Scene.h"

class MainMenu : public Scene
{
public:
	MainMenu(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;


};
