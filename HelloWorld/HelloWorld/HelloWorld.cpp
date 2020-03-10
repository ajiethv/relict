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

	//set up camera component (0)
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
		ECS::GetComponent<Camera>(camera).Zoom(40.f);
		ECS::GetComponent<HorizontalScroll>(camera).SetCam(&ECS::GetComponent<Camera>(camera));
		ECS::GetComponent<HorizontalScroll>(camera).SetOffset(0.f);
		ECS::GetComponent<VerticalScroll>(camera).SetCam(&ECS::GetComponent<Camera>(camera));
		ECS::GetComponent<VerticalScroll>(camera).SetOffset(0.f);

		//set camera
		unsigned int bitHolder = EntityIdentifier::VertiScrollCameraBit() | EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(camera, bitHolder, "Main Camera");
		ECS::SetIsMainCamera(camera, true);
	}

	//set up main player (1)
	{
		//create entity
		auto player = ECS::CreateEntity();
		EntityIdentifier::MainPlayer(player);

		//attach components
		ECS::AttachComponent<Sprite>(player);
		ECS::AttachComponent<Transform>(player);
		ECS::AttachComponent<Stats>(player);

		//set files
		std::string fileName = "Stamina.png";

		//set components
		ECS::GetComponent<Sprite>(player).LoadSprite(fileName, 6, 6);
		ECS::GetComponent<Transform>(player).SetPosition(0.f, 0.f, 0.f);
		ECS::GetComponent<Stats>(player).SetHealth(3.f);
		ECS::GetComponent<Stats>(player).SetStamina(50.f);

		//set player
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::StatsBit();
		ECS::SetUpIdentifier(player, bitHolder, "Main Character");
		ECS::SetIsMainPlayer(player, true);
	}

	//set up map collision box (2)
	{
		//create entity
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Transform>(collide);

		//set components
		ECS::GetComponent<Transform>(collide).SetScale(500, 500, 0);
		ECS::GetComponent<Transform>(collide).SetPosition(0.f, 0.f, 0.f);

		//set map collision
		unsigned int bitHolder = EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Arena collision");
	}

	//set up attack hitbox (3)
	{
		//set animation file
		auto animations = File::LoadJSON("Slash.json");

		//create entity
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Transform>(collide);
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<AnimationController>(collide);

		std::string fileName = "Slash.png";
		auto& animController = ECS::GetComponent<AnimationController>(collide);
		animController.InitUVs(fileName);

		//set animations
		animController.AddAnimation(animations["Slash"]);

		animController.SetActiveAnim(0);

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 20, 20, true, &animController);
		ECS::GetComponent<Transform>(collide).SetScale(15, 15, 0);
		ECS::GetComponent<Transform>(collide).SetPosition((20.f / 3.f), 0.f, 0.f);

		//set attack
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Attack box");
	}

	//set up map sprite (4)
	{
		//create entity
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "map.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 580, 580);
		ECS::GetComponent<Transform>(collide).SetPosition(0.f, 0.f, 10.f);

		//set map
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Map");
	}

	//set up pause sprite (5)
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
		ECS::GetComponent<Transform>(collide).SetPosition(0.f, 0.f, 0.f);

		//set pause
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "Pause");
	}

	//set up health sprites (6, 7, 8)
	for (int i = 0; i < 3; i++) {
		{
			//create entity
			auto health = ECS::CreateEntity();

			//attach components
			ECS::AttachComponent<Sprite>(health);
			ECS::AttachComponent<Transform>(health);

			//set file
			std::string fileName = "Heart.png";

			//set components
			ECS::GetComponent<Sprite>(health).LoadSprite(fileName, 10, 10);
			ECS::GetComponent<Transform>(health).SetPosition((70.f + (12.f * i)), 90.f, 90.f);

			//set health
			unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
			ECS::SetUpIdentifier(health, bitHolder, "Health");
		}
	}

	//set up stamina sprite (9)
	{
		//create entity
		auto stamina = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(stamina);
		ECS::AttachComponent<Transform>(stamina);

		//set file
		std::string fileName = "Stamina.png";

		//set components
		ECS::GetComponent<Sprite>(stamina).LoadSprite(fileName, 10, 2);
		ECS::GetComponent<Transform>(stamina).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 5.f, 0.f);

		//set stamina
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(stamina, bitHolder, "Health");
	}

	//set up clounds sprite (10)
	{
		//create entity
		auto clouds = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(clouds);
		ECS::AttachComponent<Transform>(clouds);

		//set files
		std::string fileName = "clouds.png";
		
		//set components
		ECS::GetComponent<Sprite>(clouds).LoadSprite(fileName, 580, 580);
		ECS::GetComponent<Transform>(clouds).SetPosition(0.f, 0.f, 80.f);

		//set clouds
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(clouds, bitHolder, "Clouds");
	}

	//set up player sprite (11)
	{
		//set animation file
		auto animations = File::LoadJSON("Character.json");

		//create entity
		auto player = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(player);
		ECS::AttachComponent<Transform>(player);
		ECS::AttachComponent<AnimationController>(player);

		//set files
		std::string fileName = "Character.png";
		auto& animController = ECS::GetComponent<AnimationController>(player);
		animController.InitUVs(fileName);

		//set animations
		animController.AddAnimation(animations["IdleRight"]);	//Animation 0
		animController.AddAnimation(animations["MoveRight"]);	//Animation 1
		animController.AddAnimation(animations["AttackRight"]);	//Animation 2
		animController.AddAnimation(animations["DamageRight"]);	//Animation 3
		animController.AddAnimation(animations["IdleLeft"]);	//Animation 4
		animController.AddAnimation(animations["MoveLeft"]);	//Animation 5
		animController.AddAnimation(animations["AttackLeft"]);	//Animation 6
		animController.AddAnimation(animations["DamageLeft"]);	//Animation 7

		animController.SetActiveAnim(0);

		//set components
		ECS::GetComponent<Sprite>(player).LoadSprite(fileName, 14, 14, true, &animController);
		ECS::GetComponent<Transform>(player).SetPosition(0.f, 0.f, 55.f);

		//set player
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(player, bitHolder, "Main Character Sprite");
	}
	//Set up Death Screen(12)
	{
		//create entity
		auto title = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(title);
		ECS::AttachComponent<Transform>(title);

		//set file
		std::string fileName = "deathscreen.png";

		//set components
		ECS::GetComponent<Sprite>(title).LoadSprite(fileName, 225, 125);
		ECS::GetComponent<Transform>(title).SetPosition(vec3(0.f, 0.f, -110.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(title, bitHolder, "Death Screen");
	}
	{
		//set up continue button(13)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "fight.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 75, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-200.f, -59.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "fight");
	}
	{
		//set the red hover effect for continue (14)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "fightred.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 75, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-200.f, -59.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "red fight");
	}
	{
		//set up continue button(15)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "fade.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 75, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-200.f, -59.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "fade");
	}
	{
		//set the red hover effect for continue (16)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "fadered.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 75, 50);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-200.f, -59.f, -100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "red fade");
	}

	//set up wave counter sprite (12)
	{
		//create entity
		auto wave = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(wave);
		ECS::AttachComponent<Transform>(wave);

		//set files
		std::string fileName = "Wave.png";

		//set components
		ECS::GetComponent<Sprite>(wave).LoadSprite(fileName, 14, 5);
		ECS::GetComponent<Transform>(wave).SetPosition(-50.f * aspectRatio, 53.f, 90.f);

		//set clouds
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(wave, bitHolder, "Wave:");
	}

	//set up stamina border sprite (13)
	{
		//create entity
		auto staminaBorder = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(staminaBorder);
		ECS::AttachComponent<Transform>(staminaBorder);

		//set files
		std::string fileName = "barback.png";

		//set components
		ECS::GetComponent<Sprite>(staminaBorder).LoadSprite(fileName, 32 * aspectRatio, 9);
		ECS::GetComponent<Transform>(staminaBorder).SetPosition(-30.f * aspectRatio - ((BackEnd::GetAspectRatio() - 1) * 25), -54.f, 89.f);

		//set clouds
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(staminaBorder, bitHolder, "Stamina border");
	}

	//set the camera to focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}