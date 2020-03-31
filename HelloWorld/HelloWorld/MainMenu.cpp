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
	//set up camera component (0)
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
		//set the tutorial box (7)
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
		//set the checkmark for tutorial (8)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "tutorialcheck.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 75, 25);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(4.f, -15.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "check");
	}
	{
		//set the button for instructions (9)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "Instruct.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 125, 55);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, -30.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "instruct");
	}
	{
		//set the red hover effect for instructions (10)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "Instructred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 125, 55);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-0.5f, -30.f, 99.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "instructred");
	}
	{
		//set the button for instructions (11)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "option.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 125, 55);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, -47.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "option");
	}
	{
		//set the red hover effect for instructions (12)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "optionred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 125, 55);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-0.5f, -47.f, 99.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "optionred");
	}
	{
		//set the button for credits (13)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "credits.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 125, 55);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, -65.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "credits");
	}
	{
		//set the red hover effect for credits (14)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "creditsred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 125, 55);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-0.5f, -65.f, 99.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "creditred");
	}
	{
		//set the instructions screen(15)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "empty.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 200);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, 0.f, -101.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "instructionscrn");
	}
	{
		//set the option screen(16)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "optionscrn.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 200);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, 0.f, -101.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "optionscrn");
	}
	{
		//set the credits screen(17)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "empty.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), -200);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, 0.f, 101.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "creditscrn");
	}
	{
	//set the button for back button (18)
	auto collide = ECS::CreateEntity();

	//attach components
	ECS::AttachComponent<Sprite>(collide);
	ECS::AttachComponent<Transform>(collide);

	//set file
	std::string fileName = "backto.png";

	//set components
	ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 300, 97);
	ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, -75.f, -100.f));

	//set player
	unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
	ECS::SetUpIdentifier(collide, bitHolder, "backbutton");
	}
	{
	//set the red hover effect for back button (19)
	auto collide = ECS::CreateEntity();

	//attach components
	ECS::AttachComponent<Sprite>(collide);
	ECS::AttachComponent<Transform>(collide);

	//set file
	std::string fileName = "backtored.png";

	//set components
	ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 300, 97);
	ECS::GetComponent<Transform>(collide).SetPosition(vec3(-0.5f, -75.f, -100.f));

	//set player
	unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
	ECS::SetUpIdentifier(collide, bitHolder, "backtored");
	}
	{
		//set the button for on button (20)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "on.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 50, 25);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(37.f, 10.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "on");
	}
	{
		//set the red hover effect for on button (21)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "onred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 50, 25);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(37.f, 10.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "onred");
	}
	{
		//set the button for on button (22)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "off.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 50, 25);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(125.f, 10.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "off");
	}
	{
		//set the red hover effect for on button (23)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "offred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 50, 25);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(125.f, 10.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "offred");
	}
	{
		//set the button for on button (24)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "full.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200, 80);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-85.f, -35.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "full");
	}
	{
		//set the red hover effect for on button (25)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "fullred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200, 80);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-85.f, -35.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "fullred");
	}
	{
		//set the button for on button (26)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "window.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200, 80);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(85.f, -35.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "window");
	}
	{
		//set the red hover effect for on button (27)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "windowred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200, 80);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(85.f, -35.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "windowred");
	}
}