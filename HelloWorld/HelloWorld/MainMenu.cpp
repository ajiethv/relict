#include "MainMenu.h"
#include "Game.h"


MainMenu::MainMenu(std::string name)
	: Scene(name)
{
}

void MainMenu::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;
	ECS::AttachRegister(m_sceneReg);
	float aspectRatio = windowWidth / windowHeight;
	{
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//attach camera components
		ECS::AttachComponent<Camera>(camera);

		vec4 temp = ECS::GetComponent<Camera>(camera).GetOrthoSize();

		ECS::GetComponent<Camera>(camera).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(camera).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//set camera
		unsigned int bitHolder = EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(camera, bitHolder, "Main Camera");
		ECS::SetIsMainCamera(camera, true);
	}

	//set up title entity
	{
		//create entity
		auto title = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(title);
		ECS::AttachComponent<Transform>(title);

		//set file
		std::string fileName = "title_upscale.png";

		//set components
		ECS::GetComponent<Sprite>(title).LoadSprite(fileName, 50, 25);
		ECS::GetComponent<Transform>(title).SetPosition(vec3(0.f, 0.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(title, bitHolder, "Title");
	}
}