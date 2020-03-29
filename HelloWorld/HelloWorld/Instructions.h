#pragma once

#include "Scene.h"

class Instructions : public Scene
{
public:
	Instructions(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;


}; 
