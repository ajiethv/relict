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

		//set up camera component (0)
		ECS::AttachComponent<Camera>(entity);

		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();

		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//set camera
		unsigned int bitHolder = EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Main Camera");
		ECS::SetIsMainCamera(entity, true);
	}

	//set up title screen (1)
	{
		//create entity
		auto title = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(title);
		ECS::AttachComponent<Transform>(title);

		//set file
		std::string fileName = "title_screen.png";

		//set components
		ECS::GetComponent<Sprite>(title).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 200);
		ECS::GetComponent<Transform>(title).SetPosition(vec3(0.f, 0.f, 0.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(title, bitHolder, "Title");
	}
	{
		//set up start button(2)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "start.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 150, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, 0.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "start");
	}
	{
		//set the red hover effect for start (3)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "hoverstart.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 150, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(4.f, 0.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "redstart");
	}

	//set up the load screen(4)
	{
		//create entity
		auto LoadScreen = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(LoadScreen);
		ECS::AttachComponent<Transform>(LoadScreen);

		//set files
		std::string fileName = "LoadScreenEmpty.png";

		//set components
		ECS::GetComponent<Sprite>(LoadScreen).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 200);
		ECS::GetComponent<Transform>(LoadScreen).SetPosition(vec3(0.f, 0.f, 0.f));

		//set load screen
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(LoadScreen, bitHolder, "Load Screen");
	}
	{
		//set up start button(5)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "quit.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 150, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, -85.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "quit");
	}
	{
		//set the red hover effect for start (6)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "quitred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 150, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(4.f, -85.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "redquit");
	}
	{
		//set the red hover effect for start (7)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "tutorialno.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 75, 25);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(4.f, -15.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "tutorialbox");
	}
	{
		//set the red hover effect for start (8)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "quitred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 75, 25);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(4.f, -15.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "redquit");
	}
}