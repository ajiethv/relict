#include "Game.h"

#include <random>
#include <conio.h>

Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	//Scene names and clear colors
	m_name = "Relict";
	m_clearColor = vec4(0.15f, 0.33f, 0.58f, 1.f);

	//Initializes the backend
	BackEnd::InitBackEnd(m_name);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	m_scenes.push_back(new HelloWorld("New Scene"));
	m_activeScene = m_scenes[0];

	//m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
	m_register = m_activeScene->GetScene();
}

bool Game::Run()
{
	//DELETE ===================================================================================================================================================
	for (int i = 4; i <= 11; i++) {
		m_enemy.push_back(i);
	}
	//While window is still open
	while (m_window->isOpen()) {
		while (m_window->isOpen() && ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).GetHealth() > 0)
		{
			//Clear window with clearColor
			m_window->Clear(m_clearColor);
			//Updates the game
			Update();
			//Draws the game
			BackEnd::Draw(m_register);

			//Draws ImGUI
			if (m_guiActive)
				GUI();

			//Flips the windows
			m_window->Flip();

			//Polls events and then checks them
			BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
			CheckEvents();

			//does the window have keyboard focus?
			if (Input::m_windowFocus)
			{
				//Accept all input
				AcceptInput();
			}
		}
		std::cout << "You died - Press \"Enter\" to respawn - Press \"Esc\" to quit\n"; //REPLACE WITH A DEATH SCREEN ================================================================================================================================================================================================================================================
		if (Input::m_windowFocus) {
			if (Input::GetKeyUp(Key::Escape)) {
				m_window->Close();
			}
			if (Input::GetKeyUp(Key::Enter)) {
				ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).SetHealth(3.f);
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(0.f, 0.f, 100.f);
				ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY(), 99);
				for (int x : m_bullet){
					ECS::DestroyEntity(x);
				}
				m_bullet.clear();
				for (int x : m_enemy) {
					ECS::DestroyEntity(x);
				}
				m_enemy.clear();
			}
		}
		Input::ResetKeys();
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	//make enemies attack you
	for (int i : m_enemy) {
		if (ECS::GetComponent<Enemy>(i).GetAttackSpeed() >= 0) {
			ECS::GetComponent<Enemy>(i).SetAttackSpeed(ECS::GetComponent<Enemy>(i).GetAttackSpeed() - 1);
		}
		else {
			{
				//create entity
				auto bullet = ECS::CreateEntity();

				//attach components
				ECS::AttachComponent<Sprite>(bullet);
				ECS::AttachComponent<Transform>(bullet);
				ECS::AttachComponent<Bullet>(bullet);

				//set files
				std::string fileName = "temp2.png";

				//set components
				ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);
				ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 100.f));
				ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ()), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ())));

				//set player
				unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
				ECS::SetUpIdentifier(bullet, bitHolder, "Enemy");

				//add bullet to vector
				m_bullet.push_back(bullet);
			}

			//set the camera to focus on the main player
			ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
			ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

			if (ECS::GetComponent<Enemy>(i).GetType() == 1) {
				ECS::GetComponent<Enemy>(i).SetAttackSpeed(10);
			}
		}
	}

	//move bullets
	for (int i : m_bullet) {
		ECS::GetComponent<Transform>(i).SetPosition(ECS::GetComponent<Transform>(i).GetPositionX() + ECS::GetComponent<Bullet>(i).GetVelocity().x, ECS::GetComponent<Transform>(i).GetPositionY() + ECS::GetComponent<Bullet>(i).GetVelocity().y, 100.f);
	}

	//check bullet hitboxes
	vec2 playerPos = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
	for (int i : m_bullet) { 
		vec2 bulletPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY());
		//if a bullet hits the player
		if (sqrt((bulletPos.x - playerPos.x)* (bulletPos.x - playerPos.x) + (bulletPos.y - playerPos.y) * (bulletPos.y - playerPos.y)) <= ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
			if (m_invunerability <= 0.f) {
				ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).SetHealth(ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).GetHealth() - 1);
				m_invunerability = 2.f;
				m_removeEntity.push_back(i);
			}
		}

		//if a bullet hits the arena wall
		if (!(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x)* (ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y)) + ECS::GetComponent<Transform>(i).GetScale().x <= ECS::GetComponent<Transform>(i).GetScale().x / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f)) {
			m_removeEntity.push_back(i);
		}

		//if a reflected bullet hits an enemy
		if (ECS::GetComponent<Bullet>(i).GetReflected()) {
			for (int j : m_enemy) {
				vec2 enemyPos = vec2(ECS::GetComponent<Transform>(j).GetPositionX(), ECS::GetComponent<Transform>(j).GetPositionY());
				if (sqrt((bulletPos.x - enemyPos.x) * (bulletPos.x - enemyPos.x) + (bulletPos.y - enemyPos.y) * (bulletPos.y - enemyPos.y)) <= ECS::GetComponent<Transform>(j).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
					ECS::GetComponent<Enemy>(j).SetHealth(ECS::GetComponent<Enemy>(j).GetHealth() - 1);
					m_removeEntity.push_back(i);
					if (ECS::GetComponent<Enemy>(j).GetHealth() <= 0) m_removeEntity.push_back(j);
				}
			}
		}
	}
	//check enemy hitboxes
	for (int i : m_enemy) {
		vec2 enemyPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY());
		if (sqrt((enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) + (enemyPos.y - playerPos.y) * (enemyPos.y - playerPos.y)) <= ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
			//condition for walking into an enemy
		}
	}

	//count down the invunerability
	if (m_invunerability > 0) {
		m_invunerability -= Timer::deltaTime;
		std::cout << m_invunerability << "\n";
	}

	//remove all necessary entities
	bool deleted;
	int count;
	for (int i : m_removeEntity) {
		deleted = false;
		count = -1;
		ECS::DestroyEntity(i);
		for (int j : m_bullet) {
			count++;
			if (i == j) {
				m_bullet.erase(m_bullet.begin() + count);
				deleted = true;
				break;
			}
		}
		if (!deleted) {
			count = -1;
			for (int j : m_enemy) {
				count++;
				if (i == j) {
					m_enemy.erase(m_enemy.begin() + count);
					break;
				}
			}
		}
	}
	m_removeEntity.clear();
}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register, m_activeScene);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
		MouseClick(BackEnd::GetClickEvent());

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());
}

void Game::AcceptInput()
{
	XInputManager::Update();

	//Just calls all the other input functions 
	GamepadInput();

	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::KeyboardHold()
{
	//move player
	vec2 position = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
	if (Input::GetKey(Key::W)) {
		ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + 1);
	}
	if (Input::GetKey(Key::A)) {
		ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - 1);
	}
	if (Input::GetKey(Key::S)) {
		ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 1);
	}
	if (Input::GetKey(Key::D)) {
		ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + 1);
	}
	vec2 newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());

	//move player along the edge of the arena
	if (!(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x)* (ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y)) + ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x <= ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f)) {
		ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(position.x - (position.x - ECS::GetComponent<Transform>(2).GetPositionX()) / ECS::GetComponent<Transform>(2).GetScale().x, position.y - (position.y - ECS::GetComponent<Transform>(2).GetPositionY()) / ECS::GetComponent<Transform>(2).GetScale().y, 100);
	}

	//reset new position
	newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());

	//move attack hitbox
	ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(3).GetPositionX() + (newPosition.x - position.x), ECS::GetComponent<Transform>(3).GetPositionY() + (newPosition.y - position.y), 100);

	//rotate enemies
	for (int i : m_enemy) {
		float angle = 0;
		vec2 playerPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX() - newPosition.x, newPosition.y - ECS::GetComponent<Transform>(i).GetPositionY());

		if (playerPos.x <= 0.f && playerPos.y >= 0.f) {
			angle = abs(atan(playerPos.y / playerPos.x) * (180.f / PI));
		}
		else if (playerPos.x > 0.f && playerPos.y >= 0.f) {
			angle = atan(playerPos.x / playerPos.y) * (180.f / PI) + 90.f;
		}
		else if (playerPos.x >= 0.f && playerPos.y < 0.f) {
			angle = abs(atan(playerPos.y / playerPos.x) * (180.f / PI)) + 180.f;
		}
		else {
			angle = atan(playerPos.x / playerPos.y) * (180.f / PI) + 270.f;
		}

		ECS::GetComponent<Transform>(i).SetRotationAngleZ(angle * (PI / 180.f));
	}
}

void Game::KeyboardDown()
{
	
}

void Game::KeyboardUp()
{
	
}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	//Rotate player
	vec2 mousePos = vec2((BackEnd::GetWindowWidth() / 2.f) - evnt.x, (BackEnd::GetWindowHeight() / 2.f) - evnt.y);
	float angle;
	if (mousePos.x <= 0.f && mousePos.y >= 0.f) {
		angle = abs(atan(float(mousePos.y) / float(mousePos.x)) * (180.f / PI));
	}
	else if (mousePos.x > 0.f && mousePos.y >= 0.f) {
		angle = atan(float(mousePos.x) / float(mousePos.y)) * (180.f / PI) + 90.f;
	}
	else if (mousePos.x >= 0.f && mousePos.y < 0.f) {
		angle = abs(atan(float(mousePos.y) / float(mousePos.x)) * (180.f / PI)) + 180.f;
	}
	else {
		angle = atan(float(mousePos.x) / float(mousePos.y)) * (180.f / PI) + 270.f;
	}
	ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetRotationAngleZ(angle * (PI / 180.f));

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}

	//std::cout << evnt.x / (float(BackEnd::GetWindowWidth()) / 100.f) << " " << evnt.y / (float(BackEnd::GetWindowHeight()) / 100.f) << "\n";

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() == ECS::GetComponent<Transform>(3).GetPositionX() && ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() == ECS::GetComponent<Transform>(3).GetPositionY()) {
		vec2 mousePos = vec2((BackEnd::GetWindowWidth() / 2.f) - evnt.x, (BackEnd::GetWindowHeight() / 2.f) - evnt.y);
		float angle;
		if (mousePos.x <= 0.f && mousePos.y >= 0.f) {
			angle = abs((atan(float(mousePos.y) / float(mousePos.x))) * (180.f / PI));
		}
		else if (mousePos.x > 0.f && mousePos.y >= 0.f) {
			angle = (atan(float(mousePos.x) / float(mousePos.y))) * (180.f / PI) + 90.f;
		}
		else if (mousePos.x >= 0.f && mousePos.y < 0.f) {
			angle = abs((atan(float(mousePos.y) / float(mousePos.x))) * (180.f / PI)) + 180.f;
		}
		else {
			angle = (atan(float(mousePos.x) / float(mousePos.y))) * (180.f / PI) + 270.f;
		}

		//rotate hitbox
		float xOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 2.f) * cos(angle * (PI / 180.f)), yOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 2.f) * sin(angle * (PI / 180.f));
		ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + xOffset, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + yOffset, 99);
	}
	else {
		ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY(), 99);
		m_click = false;
	}
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}

void Game::GamepadInput()
{
	XInputController* tempCon;

	for (int i = 0; i < 3; i++) {
		if (XInputManager::ControllerConnected(i)) {
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.0f);
			tempCon->SetTriggerDeadZone(0.0f);

			GamepadStroke(tempCon);
			GamepadUp(tempCon);
			GamepadDown(tempCon);
			GamepadStick(tempCon);
			GamepadTrigger(tempCon);
		}
	}
}

void Game::GamepadStroke(XInputController* con)
{
	if (con->IsButtonStroked(Buttons::A)) {
		//printf("A Stroked\n");
	}
}

void Game::GamepadUp(XInputController* con)
{
	if (con->IsButtonReleased(Buttons::RB)) {
		//printf("RB Released\n");
	}
}

void Game::GamepadDown(XInputController* con)
{
	if (con->IsButtonPressed(Buttons::THUMB_LEFT)) {
		//printf("Left Thumbstick Clicked In.\n");
	}
}

void Game::GamepadStick(XInputController* con)
{
	Stick sticks[2];
	con->GetSticks(sticks);

	if (sticks[0].x < -0.1f) {
		//printf("Left Pointing\n");
	}
	if (sticks[0].x > 0.1f) {
		//printf("Right Pointing\n");
	}

	if (sticks[0].y < -0.1f) {
		//printf("Down Pointing\n");
	}
	if (sticks[0].y > 0.1f) {
		//printf("Up Pointing\n");
	}
}

void Game::GamepadTrigger(XInputController* con)
{
	Triggers triggers;
	con->GetTriggers(triggers);

	if (triggers.RT > 0.0f) {
		
	}
}