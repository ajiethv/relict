#include "Credits.h"
#include "Game.h"


Credits::Credits(std::string name)
	: Scene(name)
{
}

void Credits::InitScene(float windowWidth, float windowHeight)
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
	{
		//set the credits screen(1)
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
		//set the button for back button (2)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "backto.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 300, 97);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(0.f, -75.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "backbutton");
	}
	{
		//set the red hover effect for back button (3)
		auto collide = ECS::CreateEntity();

		//attach components
		ECS::AttachComponent<Sprite>(collide);
		ECS::AttachComponent<Transform>(collide);

		//set file
		std::string fileName = "backtored.png";

		//set components
		ECS::GetComponent<Sprite>(collide).LoadSprite(fileName, 300, 97);
		ECS::GetComponent<Transform>(collide).SetPosition(vec3(-0.5f, -75.f, 100.f));

		//set player
		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
		ECS::SetUpIdentifier(collide, bitHolder, "backtored");
	}


}