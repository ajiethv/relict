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

	
	m_scenes.push_back(new MainMenu("Menu"));
	m_scenes.push_back(new HelloWorld("Game"));

	m_activeScene = m_scenes[0];
	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
	m_register = m_activeScene->GetScene();
}

bool Game::Run()
{
	std::string fileName = "temp.png";
	//while window is still open
	while (m_window->isOpen()) {
		//while on the main menu
		while (m_window->isOpen() && m_activeScene == m_scenes[0]) {
			//Update the backend
			BackEnd::Update(m_register);

			//Clear window with clearColor
			m_window->Clear(m_clearColor);
			//Draws the game
			BackEnd::Draw(m_register);
			//Flips the windows
			m_window->Flip();

			BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
			CheckEvents();

			//does the window have keyboard focus?
			if (Input::m_windowFocus)
			{
				//Accept all input
				AcceptInput();
			}
		}

		if (m_activeScene == m_scenes[1] && m_initialStartup) {
			//access sprites
			fileName = "tempInvunerable.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
			fileName = "temp3.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
			fileName = "temp4.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
			fileName = "temp5.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
			fileName = "temp.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
			//set everything
			ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).SetHealth(3.f);
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(0.f, -50.f, 100.f);
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetRotationAngleZ(0.f);
			float xOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 3.f), yOffset = 0.f;
			ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + xOffset, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + yOffset, 0.f);
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
			for (int x : m_bullet) {
				ECS::DestroyEntity(x);
			}
			m_bullet.clear();
			for (int x : m_enemy) {
				ECS::DestroyEntity(x);
			}
			m_enemy.clear();
			m_removeEntity.clear();
			m_invunerability = 0.f;
			m_initialStartup = false;
			ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
			ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
		}

		while (m_window->isOpen() && ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).GetHealth() > 0)
		{
			//Update timer
			Timer::Update();

			//Update the backend
			BackEnd::Update(m_register);

			//checks the speed cap
			if (m_speedCap >= 0.01f) {
				//Clear window with clearColor
				m_window->Clear(m_clearColor);

				//Updates the game
				Update();
				//Draws the game
				BackEnd::Draw(m_register);

				//Flips the windows
				m_window->Flip();

				//Polls events and then checks them
				BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
				ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() / BackEnd::GetAspectRatio(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY(), ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).GetPositionZ()));
				CheckEvents();

				//does the window have keyboard focus?
				if (Input::m_windowFocus)
				{
					//Accept all input
					AcceptInput();
				}

				//reset the speed cap
				m_speedCap = 0.f;
			}
			else {
				//count until the speed cap is reached
				m_speedCap += Timer::deltaTime;
			}

			while (pause) {
				//Update timer
				Timer::Update();
				//Update the backend
				BackEnd::Update(m_register);
				//Clear window with clearColor
				m_window->Clear(m_clearColor);
				//Draws the game
				BackEnd::Draw(m_register);
				//Flips the windows
				m_window->Flip();

				BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
				CheckEvents();
			}
		}
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		std::cout << "You died - Press \"Enter\" to respawn - Press \"Esc\" to quit\n"; //REPLACE WITH A DEATH SCREEN ================================================================================================================================================================================================================================================
		if (Input::m_windowFocus) {
			if (Input::GetKeyUp(Key::Escape)) {
				//close the game
				m_window->Close();
			}
			if (Input::GetKeyUp(Key::Enter)) {
				//reset everything
				ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).SetHealth(3.f);
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(0.f, 0.f, 100.f);
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetRotationAngleZ(0.f);
				float xOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 3.f), yOffset = 0.f;
				ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + xOffset, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + yOffset, 0.f);
				ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
				for (int x : m_bullet) {
					ECS::DestroyEntity(x);
				}
				m_bullet.clear();
				for (int x : m_enemy) {
					ECS::DestroyEntity(x);
				}
				m_enemy.clear();
				m_removeEntity.clear();
				m_invunerability = 0.f;
			}
		}
		Input::ResetKeys();
	}

	return true;
}

void Game::Update()
{
	/*if (startgame == false) {
		m_activeScene = m_scenes[0];
		std::cout << '!';
	}
	else if (startgame == true) {
		m_activeScene = m_scenes[1];
		std::cout << '?';
	}*/
	if (m_activeScene == m_scenes[0]) {

	}
	else {
		//start tutorial
		if (m_tutorial) {
			if (m_enemy.size() < 1) {
				//set up tutorial enemies
				{
					//create entity
					auto enemy = ECS::CreateEntity();

					//attach components
					ECS::AttachComponent<Sprite>(enemy);
					ECS::AttachComponent<Transform>(enemy);
					ECS::AttachComponent<Enemy>(enemy);

					//set files
					std::string fileName = "temp.png";

					//set components
					ECS::GetComponent<Sprite>(enemy).LoadSprite(fileName, 10, 10);
					ECS::GetComponent<Transform>(enemy).SetPosition(vec3(0.f, 0.f, 100.f));

					float angle = 0;
					vec2 playerPos = vec2(ECS::GetComponent<Transform>(enemy).GetPositionX() - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - ECS::GetComponent<Transform>(enemy).GetPositionY());

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

					ECS::GetComponent<Transform>(enemy).SetRotationAngleZ(angle * (PI / 180.f));
					ECS::GetComponent<Enemy>(enemy).SetType(1);
					ECS::GetComponent<Enemy>(enemy).SetAttackSpeed(100);

					//set player
					unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::EnemyBit();
					ECS::SetUpIdentifier(enemy, bitHolder, "Ttuorial Enemy");
					m_enemy.push_back(enemy);
				}
				{
					//create entity
					auto controls = ECS::CreateEntity();

					//attach components
					ECS::AttachComponent<Sprite>(controls);
					ECS::AttachComponent<Transform>(controls);

					//set file
					std::string fileName = "move_tooltip.png";

					//set components
					ECS::GetComponent<Sprite>(controls).LoadSprite(fileName, 50, 50);
					ECS::GetComponent<Transform>(controls).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 50, 100.f));

					//set player
					unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
					ECS::SetUpIdentifier(controls, bitHolder, "tooltip");
					m_tooltip = controls;
				}
				//set the camera to focus on the main player
				ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
			}
		}
		//make enemies attack you
		for (int i : m_enemy) {
			if (ECS::GetComponent<Enemy>(i).GetAttackSpeed() >= 0) {
				ECS::GetComponent<Enemy>(i).SetAttackSpeed(ECS::GetComponent<Enemy>(i).GetAttackSpeed() - 1);
			}
			else {
				//normal enemy attack pattern
				if (ECS::GetComponent<Enemy>(i).GetAttackPattern() == 1) {
					//create bullet
					{
						//create entity
						auto bullet = ECS::CreateEntity();

						//attach components
						ECS::AttachComponent<Sprite>(bullet);
						ECS::AttachComponent<Transform>(bullet);
						ECS::AttachComponent<Bullet>(bullet);

						//set files
						std::string fileName = "temp4.png";

						//set components
						ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 100.f));
						ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ()), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ())));
						ECS::GetComponent<Bullet>(bullet).SetType(2);
						if (rand() % 5 == 0) {
							ECS::GetComponent<Bullet>(bullet).SetType(3);
							fileName = "temp5.png";
						}
						if (m_tutorial) {
							ECS::GetComponent<Bullet>(bullet).SetType(m_tutorialBullet);
							if (m_tutorialBullet == 1) fileName = "temp3.png";
							else if (m_tutorialBullet == 2) fileName = "temp4.png";
							else if (m_tutorialBullet == 3) fileName = "temp5.png";
						}
						ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

						//set player
						unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
						ECS::SetUpIdentifier(bullet, bitHolder, "Enemy");

						//add bullet to vector
						m_bullet.push_back(bullet);
						m_bulletSprite.push_back(true);
					}
				}
				//boss 1 attack pattern
				else if (ECS::GetComponent<Enemy>(i).GetAttackPattern() == 2) {
					for (int j = 0; j < 8; j++) {
						//create a bullet
						{
							//create entity
							auto bullet = ECS::CreateEntity();

							//attach components
							ECS::AttachComponent<Sprite>(bullet);
							ECS::AttachComponent<Transform>(bullet);
							ECS::AttachComponent<Bullet>(bullet);

							//set files
							std::string fileName = "temp3.png";

							//set components
							ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);
							ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 100.f));
							if (j % 2 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + (j * PI / 4.f + m_bossBulletOffset)), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + (j * PI / 4.f + m_bossBulletOffset))));
							}
							else {
								ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + (j * PI / 4.f + m_bossBulletOffset + PI / 80.f)), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + (j * PI / 4.f + m_bossBulletOffset + PI / 80.f))));
							}
							ECS::GetComponent<Bullet>(bullet).SetType(1);
							if (rand() % 10 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetType(3);
								fileName = "temp5.png";
								ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);
							}

							//set player
							unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
							ECS::SetUpIdentifier(bullet, bitHolder, "Enemy");

							//add bullet to vector
							m_bullet.push_back(bullet);
							m_bulletSprite.push_back(true);
						}
					}
					if (m_bossBulletOffset >= PI / 40.f * 19.f) {
						m_bossBulletOffset = 0;
					}
					else {
						m_bossBulletOffset += PI / 40.f;
					}
				}

				//set the camera to focus on the main player
				ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

				ECS::GetComponent<Enemy>(i).SetAttackSpeed(20);
				if (ECS::GetComponent<Enemy>(i).GetType() == 1) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(20);
				}
				else if (ECS::GetComponent<Enemy>(i).GetType() == 2) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(10);
				}
				if (m_tutorial) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(300);
				}
			}
		}

		//check bullet hitboxes
		vec2 playerPos = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
		for (int i = 0; i < m_bullet.size(); i++) {
			ECS::GetComponent<Transform>(m_bullet[i]).SetPosition(ECS::GetComponent<Transform>(m_bullet[i]).GetPositionX() + ECS::GetComponent<Bullet>(m_bullet[i]).GetVelocity().x, ECS::GetComponent<Transform>(m_bullet[i]).GetPositionY() + ECS::GetComponent<Bullet>(m_bullet[i]).GetVelocity().y, 100.f);
			vec2 bulletPos = vec2(ECS::GetComponent<Transform>(m_bullet[i]).GetPositionX(), ECS::GetComponent<Transform>(m_bullet[i]).GetPositionY());

			if (m_bulletSprite[i] == false && (abs(bulletPos.x - playerPos.x) < 100.f * BackEnd::GetAspectRatio() || abs(bulletPos.y - playerPos.y) < 100.f)) {
				ECS::AttachComponent<Sprite>(m_bullet[i]);
				std::string fileName = "temp3.png";
				if (ECS::GetComponent<Bullet>(m_bullet[i]).GetType() == 2) {
					fileName = "temp4.png";
				}
				else if (ECS::GetComponent<Bullet>(m_bullet[i]).GetType() == 3) {
					fileName = "temp5.png";
				}
				ECS::GetComponent<Sprite>(m_bullet[i]).LoadSprite(fileName, 3, 3);
				m_bulletSprite[i] = true;
			}
			else if (m_bulletSprite[i] == true && (abs(bulletPos.x - playerPos.x) > 100.f * BackEnd::GetAspectRatio() || abs(bulletPos.y - playerPos.y) > 100.f)) {
				ECS::RemoveComponent<Sprite>(m_bullet[i]);
				m_bulletSprite[i] = false;
			}

			//if a bullet hits the player
			if (sqrt((bulletPos.x - playerPos.x) * (bulletPos.x - playerPos.x) + (bulletPos.y - playerPos.y) * (bulletPos.y - playerPos.y)) <= ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f) {
				if (m_invunerability <= 0.f) {
					if (!m_tutorial) ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).SetHealth(ECS::GetComponent<HealthBar>(EntityIdentifier::MainPlayer()).GetHealth() - 1);
					m_invunerability = 2.f;
					m_removeEntity.push_back(m_bullet[i]);
				}
			}

			//if a bullet hits the arena wall
			if (!(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x)* (ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y)) + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x <= ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f)) {
				m_removeEntity.push_back(m_bullet[i]);
				if (m_tutorialBullet == 1) {
					m_tutorialBullet++;
					std::string fileName = "attack_tooltip.png";
					ECS::GetComponent<Sprite>(m_tooltip).LoadSprite(fileName, 30, 50);
				}
			}

			//if a reflected bullet hits an enemy
			if (ECS::GetComponent<Bullet>(m_bullet[i]).GetReflected()) {
				for (int j : m_enemy) {
					vec2 enemyPos = vec2(ECS::GetComponent<Transform>(j).GetPositionX(), ECS::GetComponent<Transform>(j).GetPositionY());
					if (sqrt((bulletPos.x - enemyPos.x) * (bulletPos.x - enemyPos.x) + (bulletPos.y - enemyPos.y) * (bulletPos.y - enemyPos.y)) <= ECS::GetComponent<Transform>(j).GetScale().x / 2.f + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f) {
						ECS::GetComponent<Enemy>(j).SetHealth(ECS::GetComponent<Enemy>(j).GetHealth() - 1);
						m_removeEntity.push_back(m_bullet[i]);
						if (ECS::GetComponent<Enemy>(j).GetHealth() <= 0) m_removeEntity.push_back(j);
						if (m_tutorial) {
							m_tutorial = false;
							ECS::DestroyEntity(m_tooltip);
							m_tooltip = 0;
						}
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
			m_invunerability -= m_speedCap;

			//set the player sprite to flash
			std::string fileName;
			if ((m_invunerability <= 2.f && m_invunerability > 2.f / 9.f * 8.f) || (m_invunerability <= 2.f / 9.f * 7.f && m_invunerability > 2.f / 9.f * 6.f) || (m_invunerability <= 2.f / 9.f * 5.f && m_invunerability > 2.f / 9.f * 4.f) || (m_invunerability <= 2.f / 9.f * 3.f && m_invunerability > 2.f / 9.f * 2.f) || (m_invunerability <= 2.f / 9.f && m_invunerability > 0.f)) {
				fileName = "tempInvunerable.png";
			}
			else {
				fileName = "temp.png";
			}
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 10, 10);
		}

		//remove all necessary entities
		for (int i = 0; i < m_removeEntity.size(); i++) {
			for (int j = 0; j < m_removeEntity.size(); j++) {
				if (i != j && m_removeEntity[i] == m_removeEntity[j]) {
					m_removeEntity.erase(m_removeEntity.begin() + j);
					j = -1;
					i = -1;
					break;
				}
			}
		}
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
					m_bulletSprite.erase(m_bulletSprite.begin() + count);
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
	if (m_activeScene == m_scenes[0]) {
		
	}
	else {
		//move player
		vec2 position = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
		if (Input::GetKey(Key::W) && Input::GetKey(Key::A)) {
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + sqrt(0.5f));
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - sqrt(0.5f));
		}
		else if (Input::GetKey(Key::W) && Input::GetKey(Key::D)) {
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + sqrt(0.5f));
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + sqrt(0.5f));
		}
		else if (Input::GetKey(Key::S) && Input::GetKey(Key::A)) {
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - sqrt(0.5f));
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - sqrt(0.5f));
		}
		else if (Input::GetKey(Key::S) && Input::GetKey(Key::D)) {
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - sqrt(0.5f));
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + sqrt(0.5f));
		}
		else {
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
		}
		vec2 newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());

		//move player along the edge of the arena
		if (!(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x)* (ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y)) + ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x <= ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f)) {
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(position.x - (position.x - ECS::GetComponent<Transform>(2).GetPositionX()) / ECS::GetComponent<Transform>(2).GetScale().x, position.y - (position.y - ECS::GetComponent<Transform>(2).GetPositionY()) / ECS::GetComponent<Transform>(2).GetScale().y, 100);
		}

		//reset new position
		newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());

		//move attack hitbox
		ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(3).GetPositionX() + (newPosition.x - position.x), ECS::GetComponent<Transform>(3).GetPositionY() + (newPosition.y - position.y), 0.f);

		//rotate only normal enemies
		for (int i : m_enemy) {
			if (ECS::GetComponent<Enemy>(i).GetType() == 1) {
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
		if (m_tutorial && m_tooltip != 0) {
			ECS::GetComponent<Transform>(m_tooltip).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 50, 100.5f));
		}
	}
}

void Game::KeyboardDown()
{
	if (Input::GetKey(Key::Escape)) {
		pause = !pause;
		ECS::GetComponent<Transform>(5).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY(), 101.f);
	}
}

void Game::KeyboardUp()
{
	if (m_activeScene == m_scenes[0]) {
		/*if (Input::GetKeyUp(Key::Enter)) {
			m_activeScene = m_scenes[1];
			m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
		}*/
	}
	else {
	}
}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	if (m_activeScene == m_scenes[0]) {
		
	}
	else {
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

		//rotate attack hitbox
		float xOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 3.f) * cos(angle * (PI / 180.f)), yOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 3.f) * sin(angle * (PI / 180.f));
		ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + xOffset, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + yOffset, 0.f);
	}
	//std::cout << evnt.x / (float(BackEnd::GetWindowWidth()) / 100.f) << " " << evnt.y / (float(BackEnd::GetWindowHeight()) / 100.f) << "\n";

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	if (pause) {
		pause = !pause;
		ECS::GetComponent<Transform>(5).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY(), 0.f);
		std::cout << "woah" << std::endl;
	}
	else {
		if (m_activeScene == m_scenes[0]) {
			//startgame = true;
			m_activeScene = m_scenes[1];
			m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
			m_register = m_activeScene->GetScene();
			std::cout << "HEAVEN OR HELL: LET'S ROCK" << std::endl;
		}
		else {
			std::cout << "cut" << std::endl;
			//if you left click
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				//check each bullet
				for (int i : m_bullet) {
					//if the bullet can be destroyed
					if (ECS::GetComponent<Bullet>(i).GetType() == 2) {
						//check if the attack hitbox is colliding with the bullet
						vec2 bulletPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY()), attackPos = vec2(ECS::GetComponent<Transform>(3).GetPositionX(), ECS::GetComponent<Transform>(3).GetPositionY());
						if (sqrt((bulletPos.x - attackPos.x) * (bulletPos.x - attackPos.x) + (bulletPos.y - attackPos.y) * (bulletPos.y - attackPos.y)) <= ECS::GetComponent<Transform>(3).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
							m_removeEntity.push_back(i);
						}
						if (m_tutorialBullet == 2) m_tutorialBullet++;
					}
					//if the bullet can be reflected
					else if (ECS::GetComponent<Bullet>(i).GetType() == 3) {
						//check if the attack hitbox is colliding with the bullet
						vec2 bulletPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY()), attackPos = vec2(ECS::GetComponent<Transform>(3).GetPositionX(), ECS::GetComponent<Transform>(3).GetPositionY());
						if (sqrt((bulletPos.x - attackPos.x) * (bulletPos.x - attackPos.x) + (bulletPos.y - attackPos.y) * (bulletPos.y - attackPos.y)) <= ECS::GetComponent<Transform>(3).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
							ECS::GetComponent<Bullet>(i).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetRotationAngleZ()), sin(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetRotationAngleZ())));
							ECS::GetComponent<Bullet>(i).SetReflected(true);
						}
					}
				}
			}
		}
	}

	m_click = false;
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
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