#include "HelloWorld.h"
#include "Game.h"


HelloWorld::HelloWorld(std::string name)
	: Scene(name)
{
}

void HelloWorld::InitScene(float windowWidth, float windowHeight)
{
	//set up window
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	//set up camera component
	{
		//create entity
		auto camera = ECS::CreateEntity();
		EntityIdentifier::MainCamera(camera);

		//attach camera components
		ECS::AttachComponent<Camera>(camera);
		ECS::AttachComponent<HorizontalScroll>(camera);
		ECS::AttachComponent<VerticalScroll>(camera);

		vec4 temp = ECS::GetComponent<Camera>(camera).GetOrthoSize();

		ECS::GetComponent<Camera>(camera).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(camera).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
		ECS::GetComponent<HorizontalScroll>(camera).SetCam(&ECS::GetComponent<Camera>(camera));
		ECS::GetComponent<HorizontalScroll>(camera).SetOffset(0.f);
		ECS::GetComponent<VerticalScroll>(camera).SetCam(&ECS::GetComponent<Camera>(camera));
		ECS::GetComponent<VerticalScroll>(camera).SetOffset(0.f);

		//set camera
		unsigned int bitHolder = EntityIdentifier::VertiScrollCameraBit() | EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(camera, bitHolder, "Main Camera");
		ECS::SetIsMainCamera(camera, true);
	}

	//set up main player
	{
		//set animation file
		//auto animations = File::LoadJSON("JSON FILE NAME");

		//create entity
		auto player = ECS::CreateEntity();
		EntityIdentifier::MainPlayer(player);

		//attach components
		ECS::AttachComponent<Sprite>(player);
		ECS::AttachComponent<Transform>(player);
		ECS::AttachComponent<HealthBar>(player);
		//ECS::AttachComponent<AnimationController>(player);

		//set files
		std::string fileName = "temp.png";
		//auto& animController = ECS::GetComponent<AnimationController>(player);
		//animController.InitUVs(fileName);

		//set animations
		//animController.AddAnimation(animations["ANIMATION NAME"]);

		//animController.SetActiveAnim(0);

		//set components
		ECS::GetComponent<Sprite>(player).LoadSprite(fileName, 10, 10/*, true, &animController*/);
		ECS::GetComponent<Transform>(player).SetPosition(vec3(0.f, -40.f, 100.f));
		ECS::GetComponent<HealthBar>(player).SetHealth(3.f);

		//set player
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()/* | EntityIdentifier::AnimationBit()*/ | EntityIdentifier::HealthBarBit();
		ECS::SetUpIdentifier(player, bitHolder, "Main Character");
		ECS::SetIsMainPlayer(player, true);
	}

	//set up map collision box
	{
		//create entity
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Transform>(collide);

		//set components
		ECS::GetComponent<Transform>(collide).SetScale(500, 500, 0);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, 0.f, 98.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Arena collision");
	}

	//set up attack hitbox
	{
		//create entity
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set files
		std::string fileName = "temp2.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Transform>(3).SetPosition((20.f / 3.f), 0.f, 0.f);

		//set player
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Attack box");
	}

	//set up map sprite
	{
		//create entity
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "arena.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 580, 580);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, 0.f, 98.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Arena");
	}

	//set up pause sprite
	{
		//create entity
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "pause.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 200, 200);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, 0.f, 0.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Pause");
	}

	//set the camera to focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}