#pragma once

#include "Scene.h"

class Credits : public Scene
{
public:
	Credits(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;


};

