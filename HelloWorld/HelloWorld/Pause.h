#pragma once

#include "Scene.h"

class Pause : public Scene
{
public:
	Pause(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;


};