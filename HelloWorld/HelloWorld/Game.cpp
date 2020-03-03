#include "Game.h"

#include <random>
#include <conio.h>

std::string RedBulletSprite = "RedBullet.png", YellowBulletSprite = "OrangeBullet.png", GreenBulletSprite = "GreenBullet.png", GreenBulletSpriteRef = "WhiteBullet.png", RedBulletSpriteBig = "RedBullet2.png";
std::vector<Particle> particle;

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
	m_clearColor = vec4(122.f / 255.f, 121.f / 255.f, 128.f / 255.f, 1.f);

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

		//Load everything
		if (m_activeScene == m_scenes[1] && m_initialStartup) {
			//access sprites
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(RedBulletSprite, 6, 6);
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(YellowBulletSprite, 6, 6);
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(GreenBulletSprite, 6, 6);
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(GreenBulletSpriteRef, 6, 6);
			fileName = "Stamina.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 6, 6);
			fileName = "NoHeart.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 6, 6);
			fileName = "temp3.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 6, 6);
			fileName = "temp4.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 6, 6);
			fileName = "temp.png";
			ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 6, 6);
			//set everything
			ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetHealth(3.f);
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetRotationAngleZ(0.f);
			float xOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 3.f), yOffset = 0.f;
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(0.f, (m_tutorial) ? -40.f : 0.f, 0.f);
			ECS::GetComponent<Transform>(11).SetPosition(0.f, (m_tutorial) ? -40.f : 0.f, 55.f);
			ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + xOffset, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + yOffset, 0.f);
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

		//start tutorial
		if (m_tutorial && m_window->isOpen()) {
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
					std::string fileName = "temp3.png";

					//set components
					ECS::GetComponent<Sprite>(enemy).LoadSprite(fileName, 10, 10);
					ECS::GetComponent<Transform>(enemy).SetPosition(0.f, 0.f, 50.f);
					ECS::GetComponent<Enemy>(enemy).SetType(1);

					float angle = 0;
					vec2 enemyPos = vec2(ECS::GetComponent<Transform>(enemy).GetPositionX() - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - ECS::GetComponent<Transform>(enemy).GetPositionY());

					if (enemyPos.x <= 0.f && enemyPos.y >= 0.f) {
						angle = abs(atan(enemyPos.y / enemyPos.x) * (180.f / PI));
					}
					else if (enemyPos.x > 0.f && enemyPos.y >= 0.f) {
						angle = atan(enemyPos.x / enemyPos.y) * (180.f / PI) + 90.f;
					}
					else if (enemyPos.x >= 0.f && enemyPos.y < 0.f) {
						angle = abs(atan(enemyPos.y / enemyPos.x) * (180.f / PI)) + 180.f;
					}
					else {
						angle = atan(enemyPos.x / enemyPos.y) * (180.f / PI) + 270.f;
					}

					ECS::GetComponent<Transform>(enemy).SetRotationAngleZ(angle * (PI / 180.f));

					//set enemy
					unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::EnemyBit();
					ECS::SetUpIdentifier(enemy, bitHolder, "Tutorial Enemy");
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

					//set tooltip
					unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
					ECS::SetUpIdentifier(controls, bitHolder, "tooltip");
					m_tooltip = controls;
				}
				{
					//create entity
					auto help = ECS::CreateEntity();

					//attach components
					ECS::AttachComponent<Sprite>(help);
					ECS::AttachComponent<Transform>(help);

					//set file
					std::string fileName = "red_bullet_tooltip.png";

					//set components
					ECS::GetComponent<Sprite>(help).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 20);
					ECS::GetComponent<Transform>(help).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + 80, 100.f));

					//set tooltip
					unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
					ECS::SetUpIdentifier(help, bitHolder, "tooltip");
					m_helpTooltip = help;
				}
				//set the camera to focus on the main player
				ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
			}
		}

		while (m_window->isOpen() && ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() > 0)
		{
			//Update timer
			Timer::Update();

			//Update the backend
			BackEnd::Update(m_register);

			//update particle effects
			for (int i = 0; i < particle.size(); i++) {
				if (!particle[i].Update(Timer::deltaTime)) {
					particle.erase(particle.begin() + i);
				}
			}

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

			while (m_window->isOpen() && pause) {
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

				ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() / BackEnd::GetAspectRatio(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY(), ECS::GetComponent<Camera>(EntityIdentifier::MainCamera()).GetPositionZ()));
			}
		}

		if (m_activeScene == m_scenes[1] && ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() <= 0) {
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
					ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetHealth(3.f);
					fileName = "Heart.png";
					for (int i = 6; i < 9; i++) {
						ECS::GetComponent<Sprite>(i).LoadSprite(fileName, 10, 10);
						ECS::GetComponent<Transform>(i).SetPosition((66.f * BackEnd::GetAspectRatio() + ((BackEnd::GetAspectRatio() - 1) * 35) + (12.f * (i - 6))), 90.f, 100.5f);
					}
					fileName = "temp.png";
					ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(0.f, 0.f, 0.f);
					ECS::GetComponent<Transform>(11).SetPosition(0.f, 0.f, 55.f);
					ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetRotationAngleZ(0.f);
					float xOffset = (ECS::GetComponent<Transform>(3).GetScale().x / 3.f), yOffset = 0.f;
					ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + xOffset, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + yOffset, 0.f);
					ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).LoadSprite(fileName, 6, 6);
					for (int x : m_bullet) {
						ECS::DestroyEntity(x);
					}
					m_bullet.clear();
					for (int x : m_enemy) {
						ECS::DestroyEntity(x);
					}
					m_enemy.clear();
					for (int x : m_offscreenEnemyPos) {
						ECS::DestroyEntity(x);
					}
					for (int i = 0; i < 8; i++) {
						m_spawnPoint[i] = 0;
					}
					m_offscreenEnemyPos.clear();
					m_removeEntity.clear();
					m_offscreenBullet.clear();
					m_offscreenEnemy.clear();
					m_invunerability = 0.f;
					m_dodgeTimer = 0.f;
					m_dodgeDirection = vec2(0.f, 0.f);
					m_bossBulletOffsetSpiral = 0.f;
					m_bossBulletOffsetPulse = 0.f;
					m_bossBulletOffsetBig = 0.f;
					m_trackingBulletCount = 0;
					m_waveNum = 0;
					m_enemyNum = 0;
					m_bossesKilled = 0;
					m_enemiesKilled = 0;
				}
			}
			Input::ResetKeys();
		}
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
		//end animations
		if (ECS::GetComponent<AnimationController>(3).GetAnimationDone()) {
			ECS::GetComponent<Transform>(3).SetPositionZ(0);
			ECS::GetComponent<AnimationController>(11).SetActiveAnim(ECS::GetComponent<AnimationController>(11).GetActiveAnim() - ECS::GetComponent<AnimationController>(11).GetActiveAnim() % 4);
		}

		//count waves and get number of enemies in the wave
		if (!m_tutorial && m_enemyNum == 0 && m_enemy.size() == 0) {
			m_waveNum++;
			m_spawnTimer = 200;
			m_enemyNum = (m_waveNum % 5 == 0) ? 1 : m_waveNum + 4;
		}

		//make the player dodge
		if (m_dodgeTimer > 0.f) {
			m_dodgeTimer -= 1.f;
			vec2 position = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
			ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPosition() + vec3(m_dodgeDirection.x, m_dodgeDirection.y, 0.f));
			ECS::GetComponent<Transform>(11).SetPosition(ECS::GetComponent<Transform>(11).GetPosition() + vec3(m_dodgeDirection.x, m_dodgeDirection.y, 0.f));

			//move player along the edge of the arena
			vec2 newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
			float distToEdge = (sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x) * (ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y)) + ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x) - (ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f);
			if (distToEdge > 0) {
				float angle = 0;
				if (newPosition.x <= 0.f && newPosition.y >= 0.f) {
					angle = abs(atan(newPosition.x / newPosition.y) * (180.f / PI)) + 270.f;
				}
				else if (newPosition.x > 0.f && newPosition.y >= 0.f) {
					angle = atan(newPosition.y / newPosition.x) * (180.f / PI) + 180.f;
				}
				else if (newPosition.x >= 0.f && newPosition.y < 0.f) {
					angle = abs(atan(newPosition.x / newPosition.y) * (180.f / PI)) + 90.f;
				}
				else {
					angle = atan(newPosition.y / newPosition.x) * (180.f / PI);
				}
				angle *= (PI / 180.f);
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + distToEdge * cos(angle), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + distToEdge * sin(angle), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionZ());
				ECS::GetComponent<Transform>(11).SetPosition(ECS::GetComponent<Transform>(11).GetPositionX() + distToEdge * cos(angle), ECS::GetComponent<Transform>(11).GetPositionY() + distToEdge * sin(angle), ECS::GetComponent<Transform>(11).GetPositionZ());
				newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
			}

			//move attack hitbox
			ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(3).GetPositionX() + (newPosition.x - position.x), ECS::GetComponent<Transform>(3).GetPositionY() + (newPosition.y - position.y), ECS::GetComponent<Transform>(3).GetPositionZ());

			//move tutorial tooltips
			if (m_tutorial && m_tooltip != 0) {
				ECS::GetComponent<Transform>(m_tooltip).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 50, 100.5f));
				if (m_helpTooltip > 0) ECS::GetComponent<Transform>(m_helpTooltip).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + 80, 100.5f));
			}

			if (m_dodgeTimer <= 0.f && m_tutorial && m_helpTooltip == 0) {
				ECS::DestroyEntity(m_tooltip);
				m_tooltip = 0;
				m_tutorial = false;
			}
		}

		//do all bullet stuff
		vec2 playerPos = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
		//if the bullet is on screen
		for (int i = 0; i < m_bullet.size(); i++) {
			//get the position
			vec2 bulletPos = vec2(ECS::GetComponent<Transform>(m_bullet[i]).GetPositionX(), ECS::GetComponent<Transform>(m_bullet[i]).GetPositionY());

			//if it goes offscreen
			if (abs(bulletPos.x - playerPos.x) > (100.f + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f)* BackEnd::GetAspectRatio() || abs(bulletPos.y - playerPos.y) > (100.f + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().y / 2.f)) {
				//set the offscreen bullet variable
				m_offscreenBullet.push_back(ECS::GetComponent<Bullet>(m_bullet[i]));
				m_offscreenBullet[m_offscreenBullet.size() - 1].SetPosition(bulletPos);
				m_offscreenBullet[m_offscreenBullet.size() - 1].SetScale(vec2(ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x, ECS::GetComponent<Transform>(m_bullet[i]).GetScale().y));
				//delete the entity
				ECS::DestroyEntity(m_bullet[i]);
				m_bullet.erase(m_bullet.begin() + i);
				i--;
				continue;
			}

			//set the position
			ECS::GetComponent<Transform>(m_bullet[i]).SetPosition(ECS::GetComponent<Transform>(m_bullet[i]).GetPositionX() + ECS::GetComponent<Bullet>(m_bullet[i]).GetVelocity().x, ECS::GetComponent<Transform>(m_bullet[i]).GetPositionY() + ECS::GetComponent<Bullet>(m_bullet[i]).GetVelocity().y, 50.f);
			bulletPos = vec2(ECS::GetComponent<Transform>(m_bullet[i]).GetPositionX(), ECS::GetComponent<Transform>(m_bullet[i]).GetPositionY());

			//if a bullet hits the player
			if (sqrt((bulletPos.x - playerPos.x) * (bulletPos.x - playerPos.x) + (bulletPos.y - playerPos.y) * (bulletPos.y - playerPos.y)) <= ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f) {
				if (m_invunerability <= 0.f && m_dodgeTimer <= 0.f) {
					if (!m_tutorial) {
						ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetHealth(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() - 1);
						std::string fileName = "NoHeart.png";
						ECS::GetComponent<Sprite>(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() + 6).LoadSprite(fileName, 10, 10);
					}
					m_invunerability = 2.f;
					m_removeEntity.push_back(m_bullet[i]);
					if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() == 2) m_trackingBulletCount--;
					ECS::GetComponent<AnimationController>(11).SetActiveAnim((ECS::GetComponent<AnimationController>(11).GetActiveAnim() / 4 == 0) ? 3 : 7);
				}
			}

			//if a bullet hits the arena wall
			if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() != 3) {
				if (!(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x)* (ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y)) + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x <= ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f)) {
					m_removeEntity.push_back(m_bullet[i]);
					if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() == 2) m_trackingBulletCount--;
					if (m_tutorial && m_tutorialBullet == 1) {
						m_tutorialBullet++;
						std::string fileName = "attack_tooltip.png";
						ECS::GetComponent<Sprite>(m_tooltip).LoadSprite(fileName, 30, 50);
						fileName = "orange_bullet_tooltip.png";
						ECS::GetComponent<Sprite>(m_helpTooltip).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 20);
					}
					continue;
				}
			}
			else {
				if (!(sqrt((bulletPos.x - ECS::GetComponent<Transform>(2).GetPosition().x)* (bulletPos.x - ECS::GetComponent<Transform>(2).GetPosition().x) + (bulletPos.y - ECS::GetComponent<Transform>(2).GetPosition().y) * (bulletPos.y - ECS::GetComponent<Transform>(2).GetPosition().y)) <= ECS::GetComponent<Transform>(2).GetScale().x / 2.f + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f)) {
					m_removeEntity.push_back(m_bullet[i]);
					if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() == 2) m_trackingBulletCount--;
					if (m_tutorial && m_tutorialBullet == 1) {
						m_tutorialBullet++;
						std::string fileName = "attack_tooltip.png";
						ECS::GetComponent<Sprite>(m_tooltip).LoadSprite(fileName, 30, 50);
						fileName = "orange_bullet_tooltip.png";
						ECS::GetComponent<Sprite>(m_helpTooltip).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 20);
					}
					continue;
				}
			}

			//if a reflected bullet hits an enemy
			if (ECS::GetComponent<Bullet>(m_bullet[i]).GetReflected()) {
				for (int j : m_enemy) {
					vec2 enemyPos = vec2(ECS::GetComponent<Transform>(j).GetPositionX(), ECS::GetComponent<Transform>(j).GetPositionY());
					if (sqrt((bulletPos.x - enemyPos.x) * (bulletPos.x - enemyPos.x) + (bulletPos.y - enemyPos.y) * (bulletPos.y - enemyPos.y)) <= ECS::GetComponent<Transform>(j).GetScale().x / 2.f + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 2.f) {
						ECS::GetComponent<Enemy>(j).SetHealth(ECS::GetComponent<Enemy>(j).GetHealth() - 1);
						m_removeEntity.push_back(m_bullet[i]);
						if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() == 2) m_trackingBulletCount--;
						if (ECS::GetComponent<Enemy>(j).GetHealth() <= 0) {
							m_removeEntity.push_back(j);
							for (int k = 0; k < m_offscreenEnemy.size(); k++) {
								if (j == m_offscreenEnemy[k]) {
									m_offscreenEnemy.erase(m_offscreenEnemy.begin() + k);
									ECS::DestroyEntity(m_offscreenEnemyPos[k]);
									m_offscreenEnemyPos.erase(m_offscreenEnemyPos.begin() + k);
								}
							}
							for (int k = 0; k < 8; k++) {
								if (m_spawnPoint[k] == j) {
									m_spawnPoint[k] = 0;
								}
							}
							if (ECS::GetComponent<Enemy>(j).GetType() < 2) {
								m_enemiesKilled++;
							}
							else {
								m_bossesKilled++;
								if (ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() < 3) {
									ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetHealth(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() + 1);
									std::string fileName = "Heart.png";
									ECS::GetComponent<Sprite>(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() + 5).LoadSprite(fileName, 10, 10);
								}
								ECS::DestroyEntity(m_enemyHealth);
								m_enemyHealth = 0;
							}
						}
						if (m_tutorial) {
							ECS::DestroyEntity(m_helpTooltip);
							m_helpTooltip = 0;
							std::string fileName = "dodge_tooltip.png";
							ECS::GetComponent<Sprite>(m_tooltip).LoadSprite(fileName, 150, 50);
						}
					}
				}
				if (m_removeEntity.size() > 0 && m_removeEntity[m_removeEntity.size() - 1] == m_bullet[i]) continue;
			}

			//if it explodes, make it explode when it is halfway to the wall
			if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() == 1 && !(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x) * (ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y)) + ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x <= ECS::GetComponent<Transform>(m_bullet[i]).GetScale().x / 4.f + ECS::GetComponent<Transform>(2).GetScale().x / 4.f)) {
				m_removeEntity.push_back(m_bullet[i]);
				float randAngle = rand() % 180;
				for (int j = 0; j < 20; j++) {
					//create a bullet
					{
						//create entity
						auto bullet = ECS::CreateEntity();

						//attach components
						ECS::AttachComponent<Sprite>(bullet);
						ECS::AttachComponent<Transform>(bullet);
						ECS::AttachComponent<Bullet>(bullet);

						//set files
						std::string fileName = RedBulletSprite;

						//set components
						ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(m_bullet[i]).GetPositionX(), ECS::GetComponent<Transform>(m_bullet[i]).GetPositionY(), 50.f));
						ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos((randAngle * PI / 360.f) + (j * PI / 10.f)), sin((randAngle * PI / 360.f) + (j * PI / 10.f))));
						ECS::GetComponent<Bullet>(bullet).SetType(1);
						if (rand() % 10 == 0) {
							ECS::GetComponent<Bullet>(bullet).SetType(3);
							fileName = GreenBulletSprite;
						}
						ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);
						ECS::GetComponent<Transform>(bullet).SetScale(3, 3, 0);
							
						//set bullet
						unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
						ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

						//add bullet to vector
						m_bullet.push_back(bullet);
					}
				}
				//set the camera to focus on the main player
				ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

				if (m_removeEntity.size() > 0 && m_removeEntity[m_removeEntity.size() - 1] == m_bullet[i]) continue;
			}
			//make the bullet follow you if it can
			if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() == 2) {
				vec2 acc = vec2(playerPos.x - bulletPos.x, playerPos.y - bulletPos.y).Normalize();
				acc.DivScalar(100.f);
				ECS::GetComponent<Bullet>(m_bullet[i]).SetAcceleration(acc);
				ECS::GetComponent<Bullet>(m_bullet[i]).SetVelocity(ECS::GetComponent<Bullet>(m_bullet[i]).GetVelocity() + ECS::GetComponent<Bullet>(m_bullet[i]).GetAcceleration());
			}
			//make the bullet grow if it does
			if (ECS::GetComponent<Bullet>(m_bullet[i]).GetExtra() == 3) {
				ECS::GetComponent<Sprite>(m_bullet[i]).LoadSprite(RedBulletSpriteBig, sqrt(bulletPos.x * bulletPos.x + bulletPos.y * bulletPos.y) / 1.3, sqrt(bulletPos.x * bulletPos.x + bulletPos.y * bulletPos.y) / 1.3);
			}
		}
		//if it is offscreen
		for (int i = 0; i < m_offscreenBullet.size(); i++) {
			//set the position
			m_offscreenBullet[i].SetPosition(m_offscreenBullet[i].GetPosition() + m_offscreenBullet[i].GetVelocity());
			vec2 bulletPos = m_offscreenBullet[i].GetPosition();

			//if it comes on screen
			if (!(abs(bulletPos.x - playerPos.x) > (100.f + m_offscreenBullet[i].GetScale().x / 2.f)* BackEnd::GetAspectRatio() || abs(bulletPos.y - playerPos.y) > (100.f + m_offscreenBullet[i].GetScale().y / 2.f))) {
				//crete a new bullet entity
				{
					//create entity
					auto bullet = ECS::CreateEntity();

					//attach components
					ECS::AttachComponent<Sprite>(bullet);
					ECS::AttachComponent<Transform>(bullet);
					ECS::AttachComponent<Bullet>(bullet);

					//set files
					std::string fileName = RedBulletSprite;
					if (m_offscreenBullet[i].GetType() == 2) fileName = YellowBulletSprite;
					else if (m_offscreenBullet[i].GetType() == 3) fileName = GreenBulletSprite;
					if (m_offscreenBullet[i].GetReflected()) fileName = GreenBulletSpriteRef;
					if (m_offscreenBullet[i].GetExtra() == 3) fileName = RedBulletSpriteBig;

					//set components
					ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, m_offscreenBullet[i].GetScale().x, m_offscreenBullet[i].GetScale().y);
					ECS::GetComponent<Transform>(bullet).SetPosition(vec3(bulletPos.x, bulletPos.y, 50.f));
					ECS::GetComponent<Bullet>(bullet).SetVelocity(m_offscreenBullet[i].GetVelocity());
					ECS::GetComponent<Bullet>(bullet).SetReflected(m_offscreenBullet[i].GetReflected());
					ECS::GetComponent<Bullet>(bullet).SetType(m_offscreenBullet[i].GetType());
					ECS::GetComponent<Bullet>(bullet).SetExtra(m_offscreenBullet[i].GetExtra());
					if (m_offscreenBullet[i].GetExtra() == 2) ECS::GetComponent<Bullet>(bullet).SetAcceleration(m_offscreenBullet[i].GetAcceleration());

					//set bullet
					unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
					ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

					//add bullet to vector
					m_bullet.push_back(bullet);
				}
				//set the camera to focus on the main player
				ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

				m_offscreenBullet.erase(m_offscreenBullet.begin() + i);
				i--;
				continue;
			}

			//if a bullet hits the arena wall
			if (!(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x)* (ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y)) + 3.f <= 3.f / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f)) {
				if (m_offscreenBullet[i].GetExtra() == 2) m_trackingBulletCount--;
				if (m_tutorial && m_tutorialBullet == 1) {
					m_tutorialBullet++;
					std::string fileName = "attack_tooltip.png";
					ECS::GetComponent<Sprite>(m_tooltip).LoadSprite(fileName, 30, 50);
					fileName = "orange_bullet_tooltip.png";
					ECS::GetComponent<Sprite>(m_helpTooltip).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 20);
				}
				m_offscreenBullet.erase(m_offscreenBullet.begin() + i);
				i--;
				continue;
			}

			//if a reflected bullet hits an enemy
			if (m_offscreenBullet[i].GetReflected()) {
				bool deleteIt = false;
				for (int j : m_enemy) {
					vec2 enemyPos = vec2(ECS::GetComponent<Transform>(j).GetPositionX(), ECS::GetComponent<Transform>(j).GetPositionY());
					if (sqrt((bulletPos.x - enemyPos.x) * (bulletPos.x - enemyPos.x) + (bulletPos.y - enemyPos.y) * (bulletPos.y - enemyPos.y)) <= ECS::GetComponent<Transform>(j).GetScale().x / 2.f + m_offscreenBullet[i].GetScale().x / 2.f) {
						ECS::GetComponent<Enemy>(j).SetHealth(ECS::GetComponent<Enemy>(j).GetHealth() - 1);
						deleteIt = true;
						if (m_offscreenBullet[i].GetExtra() == 2) m_trackingBulletCount--;
						if (ECS::GetComponent<Enemy>(j).GetHealth() <= 0) {
							m_removeEntity.push_back(j);
							for (int k = 0; k < m_offscreenEnemy.size(); k++) {
								if (j == m_offscreenEnemy[k]) {
									m_offscreenEnemy.erase(m_offscreenEnemy.begin() + k);
									ECS::DestroyEntity(m_offscreenEnemyPos[k]);
									m_offscreenEnemyPos.erase(m_offscreenEnemyPos.begin() + k);
								}
							}
							for (int k = 0; k < 8; k++) {
								if (m_spawnPoint[k] == j) {
									m_spawnPoint[k] = 0;
								}
							}
							if (ECS::GetComponent<Enemy>(j).GetType() < 2) {
								m_enemiesKilled++;
							}
							else {
								m_bossesKilled++;
								if (ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() < 3) {
									ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetHealth(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() + 1);
									std::string fileName = "Heart.png";
									ECS::GetComponent<Sprite>(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetHealth() + 5).LoadSprite(fileName, 10, 10);
								}
								ECS::DestroyEntity(m_enemyHealth);
								m_enemyHealth = 0;
							}
						}
						if (m_tutorial) {
							ECS::DestroyEntity(m_helpTooltip);
							m_helpTooltip = 0;
							std::string fileName = "dodge_tooltip.png";
							ECS::GetComponent<Sprite>(m_tooltip).LoadSprite(fileName, 150, 50);
						}
					}
				}
				if (deleteIt) {
					m_offscreenBullet.erase(m_offscreenBullet.begin() + i);
					i--;
					continue;
				}
			}

			//if it explodes, make it explode when it is halfway to the wall
			if (m_offscreenBullet[i].GetExtra() == 1 && !(sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x)* (ECS::GetComponent<Transform>(2).GetPosition().x - bulletPos.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - bulletPos.y)) + m_offscreenBullet[i].GetScale().x <= m_offscreenBullet[i].GetScale().x / 4.f + ECS::GetComponent<Transform>(2).GetScale().x / 4.f)) {

				float randAngle = rand() % 180;
				for (int j = 0; j < 20; j++) {
					//create a bullet
					{
						//create entity
						Bullet bullet;

						//set componenets
						bullet.SetPosition(vec2(m_offscreenBullet[i].GetPosition().x, m_offscreenBullet[i].GetPosition().y));
						bullet.SetScale(vec2(3, 3));
						bullet.SetReflected(false);
						bullet.SetVelocity(vec2(cos((randAngle * PI / 360.f) + (j * PI / 10.f)), sin((randAngle * PI / 360.f) + (j * PI / 10.f))));
						bullet.SetType(1);
						if (rand() % 10 == 0) {
							bullet.SetType(3);
						}

						//add bullet to vector
						m_offscreenBullet.push_back(bullet);
					}
				}

				//delete the big bullet
				m_offscreenBullet.erase(m_offscreenBullet.begin() + i);
				i--;
				continue;
			}
			//make the bullet follow you if it can
			if (m_offscreenBullet[i].GetExtra() == 2) {
				vec2 acc = vec2(playerPos.x - bulletPos.x, playerPos.y - bulletPos.y).Normalize();
				acc.DivScalar(100.f);
				m_offscreenBullet[i].SetAcceleration(acc);
				m_offscreenBullet[i].SetVelocity(m_offscreenBullet[i].GetVelocity() + m_offscreenBullet[i].GetAcceleration());
			}
			//make the bullet grow if it does
			if (m_offscreenBullet[i].GetExtra() == 3) {
				m_offscreenBullet[i].SetScale(vec2(sqrt(bulletPos.x * bulletPos.x + bulletPos.y * bulletPos.y) / 1.3, sqrt(bulletPos.x * bulletPos.x + bulletPos.y * bulletPos.y) / 1.3));
			}
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
						std::string fileName = YellowBulletSprite;

						//set components
						ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
						ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ()), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ())));
						ECS::GetComponent<Bullet>(bullet).SetType(2);
						if (rand() % 3 == 0) {
							ECS::GetComponent<Bullet>(bullet).SetType(3);
							fileName = GreenBulletSprite;
						}
						if (m_tutorial) {
							ECS::GetComponent<Bullet>(bullet).SetType(m_tutorialBullet);
							if (m_tutorialBullet == 1) fileName = RedBulletSprite;
							else if (m_tutorialBullet == 2) fileName = YellowBulletSprite;
							else if (m_tutorialBullet == 3) fileName = GreenBulletSprite;
						}
						ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

						//set bullet
						unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
						ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

						//add bullet to vector
						m_bullet.push_back(bullet);
					}
				}
				//boss 1 attack pattern (spiral)
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
							std::string fileName = RedBulletSprite;

							//set components
							ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
							if (j % 2 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(j * PI / 4.f + m_bossBulletOffsetSpiral), sin(j * PI / 4.f + m_bossBulletOffsetSpiral)));
							}
							else {
								ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(j * PI / 4.f + m_bossBulletOffsetSpiral + PI / 80.f), sin(j * PI / 4.f + m_bossBulletOffsetSpiral + PI / 80.f)));
							}
							ECS::GetComponent<Bullet>(bullet).SetType(1);
							if (rand() % 10 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetType(3);
								fileName = GreenBulletSprite;
							}
							ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

							//set bullet
							unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
							ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

							//add bullet to vector
							m_bullet.push_back(bullet);
						}
					}
					if (m_bossBulletOffsetSpiral >= PI / 40.f * 19.f) {
						m_bossBulletOffsetSpiral = 0;
					}
					else {
						m_bossBulletOffsetSpiral += PI / 40.f;
					}
				}
				//boss 2 attack pattern (pulse)
				else if (ECS::GetComponent<Enemy>(i).GetAttackPattern() == 3) {
					for (int j = 0; j < 40; j++) {
						//create a bullet
						{
							//create entity
							auto bullet = ECS::CreateEntity();

							//attach components
							ECS::AttachComponent<Sprite>(bullet);
							ECS::AttachComponent<Transform>(bullet);
							ECS::AttachComponent<Bullet>(bullet);

							//set files
							std::string fileName = RedBulletSprite;

							//set components
							ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
							ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(j * PI / 20.f + m_bossBulletOffsetPulse), sin(j * PI / 20.f + m_bossBulletOffsetPulse)));
							ECS::GetComponent<Bullet>(bullet).SetType(1);
							if (rand() % 10 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetType(3);
								fileName = GreenBulletSprite;
							}
							ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

							//set bullet
							unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
							ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

							//add bullet to vector
							m_bullet.push_back(bullet);
						}
					}
					if (m_bossBulletOffsetPulse >= PI / 40.f) {
						m_bossBulletOffsetPulse = 0.f;
					}
					else {
						m_bossBulletOffsetPulse += PI / 80.f;
					}
				}
				//boss 3 attack pattern (explode)
				else if (ECS::GetComponent<Enemy>(i).GetAttackPattern() == 4) {
					//create a bullet
					{
						//create entity
						auto bullet = ECS::CreateEntity();

						//attach components
						ECS::AttachComponent<Sprite>(bullet);
						ECS::AttachComponent<Transform>(bullet);
						ECS::AttachComponent<Bullet>(bullet);

						//set components
						ECS::GetComponent<Sprite>(bullet).LoadSprite(RedBulletSprite, 15, 15);
						ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
						ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ()), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ())));
						ECS::GetComponent<Bullet>(bullet).SetType(1);
						ECS::GetComponent<Bullet>(bullet).SetExtra(1);

						//set bullet
						unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
						ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

						//add bullet to vector
						m_bullet.push_back(bullet);
					}
				}
				//boss 4 attack pattern (spray)
				else if (ECS::GetComponent<Enemy>(i).GetAttackPattern() == 5) {
					for (int j = 0; j < 4; j++) {
						//create a bullet
						{
							//create entity
							auto bullet = ECS::CreateEntity();

							//attach components
							ECS::AttachComponent<Sprite>(bullet);
							ECS::AttachComponent<Transform>(bullet);
							ECS::AttachComponent<Bullet>(bullet);

							//set files
							std::string fileName = RedBulletSprite;

							//set components
							ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
							ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + float(rand() % 111 - 55) * PI / 180.f), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + float(rand() % 111 - 55) * PI / 180.f)));
							ECS::GetComponent<Bullet>(bullet).SetType(1);
							if (rand() % 3 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetType(2);
								fileName = YellowBulletSprite;
							}
							if (rand() % 10 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetType(3);
								fileName = GreenBulletSprite;
							}
							ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

							//set bullet
							unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
							ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

							//add bullet to vector
							m_bullet.push_back(bullet);
						}
					}
				}
				//boss 5 attack pattern (follow)
				else if (ECS::GetComponent<Enemy>(i).GetAttackPattern() == 6) {
					if (m_trackingBulletCount < 7) {
						{
							//create entity
							auto bullet = ECS::CreateEntity();

							//attach components
							ECS::AttachComponent<Sprite>(bullet);
							ECS::AttachComponent<Transform>(bullet);
							ECS::AttachComponent<Bullet>(bullet);

							//set files
							std::string fileName = RedBulletSprite;

							//set components
							ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
							ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ() - 90 * PI / 180.f), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ() - 90 * PI / 180.f)));
							ECS::GetComponent<Bullet>(bullet).SetExtra(2);
							ECS::GetComponent<Bullet>(bullet).SetType(1);
							if (rand() % 5 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetType(3);
								fileName = GreenBulletSprite;
							}
							ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

							//set bullet
							unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
							ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

							//add bullet to vector
							m_bullet.push_back(bullet);
							m_trackingBulletCount++;
						}
						{
							//create entity
							auto bullet = ECS::CreateEntity();

							//attach components
							ECS::AttachComponent<Sprite>(bullet);
							ECS::AttachComponent<Transform>(bullet);
							ECS::AttachComponent<Bullet>(bullet);

							//set files
							std::string fileName = RedBulletSprite;

							//set components
							ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
							ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + 90 * PI / 180.f), sin(ECS::GetComponent<Transform>(i).GetRotationAngleZ() + 90 * PI / 180.f)));
							ECS::GetComponent<Bullet>(bullet).SetExtra(2);
							ECS::GetComponent<Bullet>(bullet).SetType(1);
							if (rand() % 5 == 0) {
								ECS::GetComponent<Bullet>(bullet).SetType(3);
								fileName = GreenBulletSprite;
							}
							ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

							//set bullet
							unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
							ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

							//add bullet to vector
							m_bullet.push_back(bullet);
							m_trackingBulletCount++;
						}
					}
				}
				//boss 6 attack pattern (BIG bullets)
				else if (ECS::GetComponent<Enemy>(i).GetAttackPattern() == 7) {
					if (m_bossBulletOffsetBig == 1.f) {
						for (int j = 0; j < 80; j++) {
							//create a bullet
							{
								//create entity
								auto bullet = ECS::CreateEntity();

								//attach components
								ECS::AttachComponent<Sprite>(bullet);
								ECS::AttachComponent<Transform>(bullet);
								ECS::AttachComponent<Bullet>(bullet);

								//set files
								std::string fileName = RedBulletSprite;

								//set components
								ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
								ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(j * PI / 40.f), sin(j * PI / 40.f)));
								ECS::GetComponent<Bullet>(bullet).SetType(1);
								if (rand() % 10 == 0) {
									ECS::GetComponent<Bullet>(bullet).SetType(3);
									fileName = GreenBulletSprite;
								}
								ECS::GetComponent<Sprite>(bullet).LoadSprite(fileName, 3, 3);

								//set bullet
								unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
								ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

								//add bullet to vector
								m_bullet.push_back(bullet);
							}
						}
					}
					else {
						for (int j = 0; j < 4; j++) {
							//create a bullet
							{
								//create entity
								auto bullet = ECS::CreateEntity();

								//attach components
								ECS::AttachComponent<Sprite>(bullet);
								ECS::AttachComponent<Transform>(bullet);
								ECS::AttachComponent<Bullet>(bullet);

								//set components
								ECS::GetComponent<Sprite>(bullet).LoadSprite(RedBulletSpriteBig, 3, 3);
								ECS::GetComponent<Transform>(bullet).SetPosition(vec3(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY(), 50.f));
								ECS::GetComponent<Bullet>(bullet).SetVelocity(vec2(cos(j * PI / 2.f + m_bossBulletOffsetBig), sin(j * PI / 2.f + m_bossBulletOffsetBig)));
								ECS::GetComponent<Bullet>(bullet).SetExtra(3);
								ECS::GetComponent<Bullet>(bullet).SetType(1);

								//set bullet
								unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::BulletBit();
								ECS::SetUpIdentifier(bullet, bitHolder, "Bullet");

								//add bullet to vector
								m_bullet.push_back(bullet);
							}
						}
					}

					//change the offset
					if (m_bossBulletOffsetBig == 0.f) {
						m_bossBulletOffsetBig += PI / 4.f;
					}
					else {
						m_bossBulletOffsetBig = 0.f;
					}
				}

				//set the camera to focus on the main player
				ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));

				ECS::GetComponent<Enemy>(i).SetAttackSpeed(20);
				if (ECS::GetComponent<Enemy>(i).GetType() == 1) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(60);
				}
				else if (ECS::GetComponent<Enemy>(i).GetType() == 2) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(10);
				}
				else if (ECS::GetComponent<Enemy>(i).GetType() == 3) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(40);
				}
				else if (ECS::GetComponent<Enemy>(i).GetType() == 4) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(75);
				}
				else if (ECS::GetComponent<Enemy>(i).GetType() == 5) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(10);
				}
				else if (ECS::GetComponent<Enemy>(i).GetType() == 6) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(60);
				}
				else if (ECS::GetComponent<Enemy>(i).GetType() == 7) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(100);
					if (rand() % 10 == 0) {
						m_bossBulletOffsetBig = 1.f;
						ECS::GetComponent<Enemy>(i).SetAttackSpeed(200);
					}
				}
				if (m_tutorial) {
					ECS::GetComponent<Enemy>(i).SetAttackSpeed(300);
				}
			}

			//if the enemy is offscreen
			if (abs(ECS::GetComponent<Transform>(i).GetPositionX() - playerPos.x) > (100.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f)* BackEnd::GetAspectRatio() || abs(ECS::GetComponent<Transform>(i).GetPositionY() - playerPos.y) > (100.f + ECS::GetComponent<Transform>(i).GetScale().y / 2.f)) {
				bool wasOnScreen = true;
				for (int j : m_offscreenEnemy) {
					if (i == j) {
						wasOnScreen = false;
						break;
					}
				}
				if (wasOnScreen) {
					//add to a vector
					m_offscreenEnemy.push_back(i);
					//create a tip to help you know the position
					{
						//create entity
						auto tip = ECS::CreateEntity();

						//attach components
						ECS::AttachComponent<Sprite>(tip);
						ECS::AttachComponent<Transform>(tip);

						//set files
						std::string fileName = "temp4.png";

						//set components
						ECS::GetComponent<Sprite>(tip).LoadSprite(fileName, 5, 5);

						vec2 tipPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX() - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY() - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
						float divideBy;
						(tipPos.x > tipPos.y) ? divideBy = tipPos.x / 90.f : divideBy = tipPos.y / 90.f;
						tipPos.DivScalar(divideBy);
						tipPos = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + tipPos.x, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + tipPos.y);
						ECS::GetComponent<Transform>(tip).SetPosition(tipPos.x, tipPos.y, 100.f);

						tipPos = vec2(tipPos.x - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), tipPos.y - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
						float angle = 0;
						if (tipPos.x <= 0.f && tipPos.y >= 0.f) {
							angle = abs(atan(tipPos.y / tipPos.x) * (180.f / PI));
						}
						else if (tipPos.x > 0.f && tipPos.y >= 0.f) {
							angle = atan(tipPos.x / tipPos.y) * (180.f / PI) + 90.f;
						}
						else if (tipPos.x >= 0.f && tipPos.y < 0.f) {
							angle = abs(atan(tipPos.y / tipPos.x) * (180.f / PI)) + 180.f;
						}
						else {
							angle = atan(tipPos.x / tipPos.y) * (180.f / PI) + 270.f;
						}
						ECS::GetComponent<Transform>(tip).SetRotationAngleZ(angle * (PI / 180.f));


						//set tip
						unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
						ECS::SetUpIdentifier(tip, bitHolder, "Bullet");

						//add tip to vector
						m_offscreenEnemyPos.push_back(tip);
					}

					//set the camera to focus on the main player
					ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
					ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				}
			}

			vec2 enemyPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY());
			if (sqrt((enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x) + (enemyPos.y - playerPos.y) * (enemyPos.y - playerPos.y)) <= ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
				//condition for walking into an enemy
			}
		}
		//for each enemy that is offscreen
		for (int i = 0; i < m_offscreenEnemy.size(); i++) {
			//if it comes back on screen
			if (abs(ECS::GetComponent<Transform>(m_offscreenEnemy[i]).GetPositionX() - playerPos.x) < (100.f + ECS::GetComponent<Transform>(m_offscreenEnemy[i]).GetScale().x / 2.f) * BackEnd::GetAspectRatio() && abs(ECS::GetComponent<Transform>(m_offscreenEnemy[i]).GetPositionY() - playerPos.y) < (100.f + ECS::GetComponent<Transform>(m_offscreenEnemy[i]).GetScale().y / 2.f)) {
				ECS::DestroyEntity(m_offscreenEnemyPos[i]);
				m_offscreenEnemy.erase(m_offscreenEnemy.begin() + i);
				m_offscreenEnemyPos.erase(m_offscreenEnemyPos.begin() + i);
				i--;
				continue;
			}

			//set the position of the tip
			vec2 tipDirection = vec2(ECS::GetComponent<Transform>(m_offscreenEnemy[i]).GetPositionX() - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(m_offscreenEnemy[i]).GetPositionY() - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY()).Normalize();
			float multiplier;
			(abs(tipDirection.x / BackEnd::GetAspectRatio()) > abs(tipDirection.y)) ? multiplier = 90 * BackEnd::GetAspectRatio() / abs(tipDirection.x) : multiplier = 90 / abs(tipDirection.y);
			vec2 tipPos = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + (tipDirection.x * multiplier), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + (tipDirection.y * multiplier));
			ECS::GetComponent<Transform>(m_offscreenEnemyPos[i]).SetPosition(tipPos.x, tipPos.y, 101.f);

			//set the rotation of the tip
			tipPos = vec2(tipPos.x - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), tipPos.y - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
			float angle = 0;
			if (tipPos.x <= 0.f && tipPos.y >= 0.f) {
				angle = abs(atan(tipPos.x / tipPos.y) * (180.f / PI)) + 90.f;
			}
			else if (tipPos.x > 0.f && tipPos.y >= 0.f) {
				angle = atan(tipPos.y / tipPos.x) * (180.f / PI);
			}
			else if (tipPos.x >= 0.f && tipPos.y < 0.f) {
				angle = abs(atan(tipPos.x / tipPos.y) * (180.f / PI)) + 270.f;
			}
			else {
				angle = atan(tipPos.y / tipPos.x) * (180.f / PI) + 180.f;
			}
			ECS::GetComponent<Transform>(m_offscreenEnemyPos[i]).SetRotationAngleZ(angle * (PI / 180.f));
		}

		//make enemies spawn
		if (m_enemyNum > 0 && m_waveNum > 0 && m_spawnTimer <= 0) {
			if (m_waveNum % 5 != 0) {
				if (!m_tutorial && ((m_enemy.size() < 8 && rand() % 200 == 0) || (m_enemy.size() == 0))) {
					//find an open spawn point
					int spawn = rand() % 8;
					for (int i = 0; i < 20 && m_spawnPoint[spawn] != 0; i++) {
						spawn = rand() % 8;
					}
					if (m_spawnPoint[spawn] != 0) {
						for (int i = 0; i < 8; i++) {
							if (m_spawnPoint[i] == 0) {
								spawn = i;
								break;
							}
						}
					}
					//create an enemy at the spawn point
					{
						//create the entity
						auto enemy = ECS::CreateEntity();

						//attach the components
						ECS::AttachComponent<Sprite>(enemy);
						ECS::AttachComponent<Transform>(enemy);
						ECS::AttachComponent<Enemy>(enemy);

						//set files
						std::string fileName = "temp3.png";

						//set components
						ECS::GetComponent<Sprite>(enemy).LoadSprite(fileName, 10, 10);
						ECS::GetComponent<Transform>(enemy).SetPosition((ECS::GetComponent<Transform>(2).GetScale().x / 2.f - 25.f) * cos((spawn * 45) * (PI / 180.f)), (ECS::GetComponent<Transform>(2).GetScale().x / 2.f - 25.f) * sin((spawn * 45) * (PI / 180.f)), 50.f);
						ECS::GetComponent<Enemy>(enemy).SetType(1);

						//set up identifier
						unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit() | EntityIdentifier::EnemyBit();
						ECS::SetUpIdentifier(enemy, bitHolder, "enemy");

						//add to enemy vector
						m_enemy.push_back(enemy);
						m_spawnPoint[spawn] = enemy;
					}
					m_enemyNum--;
					m_spawnTimer = 100;

					//set the camera to focus on the main player
					ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
					ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				}
			}
			else {
				//create a boss
				{
					//create the entity
					auto boss = ECS::CreateEntity();

					//attach the components
					ECS::AttachComponent<Sprite>(boss);
					ECS::AttachComponent<Transform>(boss);
					ECS::AttachComponent<Enemy>(boss);

					//set files
					std::string fileName = "temp3.png";

					//set components
					ECS::GetComponent<Sprite>(boss).LoadSprite(fileName, 20, 20);
					ECS::GetComponent<Transform>(boss).SetPosition(0.f, 0.f, 50.f);
					ECS::GetComponent<Enemy>(boss).SetType((m_waveNum - 5) / 5 % 6 + 2);

					//set up identifier
					unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit() | EntityIdentifier::EnemyBit();
					ECS::SetUpIdentifier(boss, bitHolder, "Boss");

					//add to enemy vector
					m_enemy.push_back(boss);
				}
				//create boss health bar
				{
					//create the entity
					auto bossHealth = ECS::CreateEntity();

					//attach the components
					ECS::AttachComponent<Sprite>(bossHealth);
					ECS::AttachComponent<Transform>(bossHealth);

					//set files
					std::string fileName = "Health.png";

					//set components
					ECS::GetComponent<Sprite>(bossHealth).LoadSprite(fileName, 190 * BackEnd::GetAspectRatio(), 10);
					ECS::GetComponent<Transform>(bossHealth).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 90.f, 100.5f);

					//set up identifier
					unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit() | EntityIdentifier::EnemyBit();
					ECS::SetUpIdentifier(bossHealth, bitHolder, "Boss Health");

					m_enemyHealth = bossHealth;
				}
				{
					//create the entity
					auto bossHealthBorder = ECS::CreateEntity();

					//attach the components
					ECS::AttachComponent<Sprite>(bossHealthBorder);
					ECS::AttachComponent<Transform>(bossHealthBorder);

					//set files
					std::string fileName = "HealthBorder.png";

					//set components
					ECS::GetComponent<Sprite>(bossHealthBorder).LoadSprite(fileName, 191 * BackEnd::GetAspectRatio(), 11);
					ECS::GetComponent<Transform>(bossHealthBorder).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 90.f, 100.5f);

					//set up identifier
					unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit() | EntityIdentifier::EnemyBit();
					ECS::SetUpIdentifier(bossHealthBorder, bitHolder, "Boss Health Border");
				}
				m_enemyNum--;
				m_spawnTimer = 100;

				//set the camera to focus on the main player
				ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
				ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
			}
		}

		//count down the invunerability
		if (m_invunerability > 0) {
			m_invunerability -= 0.02;
		}

		//count down spawn timer
		if (m_spawnTimer > 0) {
			m_spawnTimer--;
		}

		if (m_tutorial && m_helpTooltip > 0) {
			std::string fileName = "";
			if (m_tutorialBullet == 1) {
				fileName = "red_bullet_tooltip.png";
			}
			else if (m_tutorialBullet == 2) {
				fileName = "orange_bullet_tooltip.png";
			}
			else {
				fileName = "green_bullet_tooltip.png";
			}
			ECS::GetComponent<Sprite>(m_helpTooltip).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 20);
		}
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
		//set the ammount to increase the stamina by
		float staminaIncrease = 0.2f;

		//if a key is actually being pressed and you are not dodging
		if ((Input::GetKey(Key::W) || Input::GetKey(Key::A) || Input::GetKey(Key::S) || Input::GetKey(Key::D)) && m_dodgeTimer <= 0.f) {
			//move the player...
			vec2 position = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());
			//up and to the left
			if (Input::GetKey(Key::W) && Input::GetKey(Key::A)) {
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + sqrt(0.5f));
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionY(ECS::GetComponent<Transform>(11).GetPositionY() + sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionX(ECS::GetComponent<Transform>(11).GetPositionX() - sqrt(0.5f));
			}
			//up and to the right
			else if (Input::GetKey(Key::W) && Input::GetKey(Key::D)) {
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + sqrt(0.5f));
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionY(ECS::GetComponent<Transform>(11).GetPositionY() + sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionX(ECS::GetComponent<Transform>(11).GetPositionX() + sqrt(0.5f));
			}
			//down and to the left
			else if (Input::GetKey(Key::S) && Input::GetKey(Key::A)) {
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - sqrt(0.5f));
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionY(ECS::GetComponent<Transform>(11).GetPositionY() - sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionX(ECS::GetComponent<Transform>(11).GetPositionX() - sqrt(0.5f));
			}
			//down and to the right
			else if (Input::GetKey(Key::S) && Input::GetKey(Key::D)) {
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - sqrt(0.5f));
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionY(ECS::GetComponent<Transform>(11).GetPositionY() - sqrt(0.5f));
				ECS::GetComponent<Transform>(11).SetPositionX(ECS::GetComponent<Transform>(11).GetPositionX() + sqrt(0.5f));
			}
			else {
				//straight up
				if (Input::GetKey(Key::W)) {
					ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + 1);
					ECS::GetComponent<Transform>(11).SetPositionY(ECS::GetComponent<Transform>(11).GetPositionY() + 1);
				}
				//straight left
				else if (Input::GetKey(Key::A)) {
					ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - 1);
					ECS::GetComponent<Transform>(11).SetPositionX(ECS::GetComponent<Transform>(11).GetPositionX() - 1);
				}
				//straight down
				else if (Input::GetKey(Key::S)) {
					ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionY(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 1);
					ECS::GetComponent<Transform>(11).SetPositionY(ECS::GetComponent<Transform>(11).GetPositionY() - 1);
				}
				//straight right
				else if (Input::GetKey(Key::D)) {
					ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + 1);
					ECS::GetComponent<Transform>(11).SetPositionX(ECS::GetComponent<Transform>(11).GetPositionX() + 1);
				}
			}
			vec2 newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());

			//change the animation
			if (Input::GetKey(Key::A) && (ECS::GetComponent<AnimationController>(11).GetActiveAnim() == 1 || ECS::GetComponent<AnimationController>(11).GetActiveAnim() % 4 == 0)) {
				ECS::GetComponent<AnimationController>(11).SetActiveAnim(5);
			}
			else if (Input::GetKey(Key::D) && (ECS::GetComponent<AnimationController>(11).GetActiveAnim() == 5 || ECS::GetComponent<AnimationController>(11).GetActiveAnim() % 4 == 0)) {
				ECS::GetComponent<AnimationController>(11).SetActiveAnim(1);
			}
			else if ((Input::GetKey(Key::W) || Input::GetKey(Key::S)) && ECS::GetComponent<AnimationController>(11).GetActiveAnim() % 4 == 0) {
				ECS::GetComponent<AnimationController>(11).SetActiveAnim(ECS::GetComponent<AnimationController>(11).GetActiveAnim() + 1);
			}

			//move player along the edge of the arena
			float distToEdge = (sqrt((ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x) * (ECS::GetComponent<Transform>(2).GetPosition().x - newPosition.x) + (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y) * (ECS::GetComponent<Transform>(2).GetPosition().y - newPosition.y)) + ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x) - (ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetScale().x / 2.f + ECS::GetComponent<Transform>(2).GetScale().x / 2.f);
			if (distToEdge > 0) {
				float angle = 0;
				if (newPosition.x <= 0.f && newPosition.y >= 0.f) {
					angle = abs(atan(newPosition.x / newPosition.y) * (180.f / PI)) + 270.f;
				}
				else if (newPosition.x > 0.f && newPosition.y >= 0.f) {
					angle = atan(newPosition.y / newPosition.x) * (180.f / PI) + 180.f;
				}
				else if (newPosition.x >= 0.f && newPosition.y < 0.f) {
					angle = abs(atan(newPosition.x / newPosition.y) * (180.f / PI)) + 90.f;
				}
				else {
					angle = atan(newPosition.y / newPosition.x) * (180.f / PI);
				}
				angle *= (PI / 180.f);
				ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + distToEdge * cos(angle), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + distToEdge * sin(angle), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionZ());
				ECS::GetComponent<Transform>(11).SetPosition(ECS::GetComponent<Transform>(11).GetPositionX() + distToEdge * cos(angle), ECS::GetComponent<Transform>(11).GetPositionY() + distToEdge * sin(angle), ECS::GetComponent<Transform>(11).GetPositionZ());
			}

			//reset new position
			newPosition = vec2(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY());

			//move attack hitbox
			ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(3).GetPositionX() + (newPosition.x - position.x), ECS::GetComponent<Transform>(3).GetPositionY() + (newPosition.y - position.y), ECS::GetComponent<Transform>(3).GetPositionZ());

			if (m_tutorial && m_tooltip != 0) {
				ECS::GetComponent<Transform>(m_tooltip).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 50, 100.5f));
				if (m_helpTooltip > 0) ECS::GetComponent<Transform>(m_helpTooltip).SetPosition(vec3(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + 80, 100.5f));
			}

			//get less stamina
			staminaIncrease = 0.1f;
		}
		
		if (!(Input::GetKey(Key::W) || Input::GetKey(Key::A) || Input::GetKey(Key::S) || Input::GetKey(Key::D)) && ECS::GetComponent<AnimationController>(11).GetActiveAnim() % 4 == 1) {
			ECS::GetComponent<AnimationController>(11).SetActiveAnim(ECS::GetComponent<AnimationController>(11).GetActiveAnim() - 1);
		}

		//dont get stamina if you are dodging
		staminaIncrease = (ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() < 50.f) ? ((m_dodgeTimer <= 0) ? staminaIncrease : 0.f) : 0.f;
		//increase the stamina
		ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetStamina(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() + staminaIncrease);
		ECS::GetComponent<Transform>(9).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - float(int(10 - (ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() / 5))) / 2.f, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 9.f, (ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() < 50.f) ? 100.f : 0.f);
		std::string fileName = "Stamina.png";
		ECS::GetComponent<Sprite>(9).LoadSprite(fileName, ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() / 5, 2);

		//output boss health
		if (m_enemyHealth > 0) {
			ECS::GetComponent<Transform>(m_enemyHealth).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() - float(int(190 * BackEnd::GetAspectRatio() - ((ECS::GetComponent<Enemy>(m_enemy[0]).GetHealth() * 19) * BackEnd::GetAspectRatio()))) / 2.f, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 94.f, 100.5f);
			fileName = "Health.png";
			ECS::GetComponent<Sprite>(m_enemyHealth).LoadSprite(fileName, (ECS::GetComponent<Enemy>(m_enemy[0]).GetHealth() * 19) * BackEnd::GetAspectRatio(), 10);
			ECS::GetComponent<Transform>(m_enemyHealth + 1).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - 94.f, 100.1f);
		}

		//rotate enemies
		for (int i : m_enemy) {
			if (ECS::GetComponent<Enemy>(i).GetType() == 1 || ECS::GetComponent<Enemy>(i).GetType() == 4 || ECS::GetComponent<Enemy>(i).GetType() == 5 || ECS::GetComponent<Enemy>(i).GetType() == 6) {
				float angle = 0;
				vec2 playerPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX() - ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() - ECS::GetComponent<Transform>(i).GetPositionY());

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

		//move the UI
		for (int i = 6; i < 9; i++) {
			ECS::GetComponent<Transform>(i).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + (66.f * BackEnd::GetAspectRatio() + ((BackEnd::GetAspectRatio() - 1) * 35) + (12.f * (i - 6))), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + 90.f, 100.5f);
		}
	}
}

void Game::KeyboardDown()
{
	if (m_activeScene == m_scenes[1]) {
		if (Input::GetKey(Key::Escape)) {
			pause = !pause;
			ECS::GetComponent<Transform>(5).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX(), ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY(), 101.f);
			std::string fileName = "pause.png";
			ECS::GetComponent<Sprite>(5).LoadSprite(fileName, 200.f * BackEnd::GetAspectRatio(), 200.f);
		}

		//make the player dodge
		if (Input::GetKeyDown(Key::Space) && m_dodgeTimer <= 0.f && ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() >= 10.f) {
			if (Input::GetKey(Key::W) && Input::GetKey(Key::A)) {
				m_dodgeTimer = 20.f;
				m_dodgeDirection = vec2(-sqrt(0.5f), sqrt(0.5f));
			}
			//up and to the right
			else if (Input::GetKey(Key::W) && Input::GetKey(Key::D)) {
				m_dodgeTimer = 20.f;
				m_dodgeDirection = vec2(sqrt(0.5f), sqrt(0.5f));
			}
			//down and to the left
			else if (Input::GetKey(Key::S) && Input::GetKey(Key::A)) {
				m_dodgeTimer = 20.f;
				m_dodgeDirection = vec2(-sqrt(0.5f), -sqrt(0.5f));
			}
			//down and to the right
			else if (Input::GetKey(Key::S) && Input::GetKey(Key::D)) {
				m_dodgeTimer = 20.f;
				m_dodgeDirection = vec2(sqrt(0.5f), -sqrt(0.5f));
			}
			else {
				//straight up
				if (Input::GetKey(Key::W)) {
					m_dodgeTimer = 20.f;
					m_dodgeDirection = vec2(0, 1);
				}
				//straight left
				if (Input::GetKey(Key::A)) {
					m_dodgeTimer = 20.f;
					m_dodgeDirection = vec2(-1, 0);
				}
				//straight down
				if (Input::GetKey(Key::S)) {
					m_dodgeTimer = 20.f;
					m_dodgeDirection = vec2(0, -1);
				}
				//straight right
				if (Input::GetKey(Key::D)) {
					m_dodgeTimer = 20.f;
					m_dodgeDirection = vec2(1, 0);
				}
			}
			m_dodgeDirection.MultScalar(2.f);
			ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetStamina(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() - 10);
		}
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
		ECS::GetComponent<Transform>(3).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionX() + xOffset, ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPositionY() + yOffset, ECS::GetComponent<Transform>(3).GetPositionZ());
		ECS::GetComponent<Transform>(3).SetRotationAngleZ(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetRotationAngleZ());
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
			if (m_dodgeTimer <= 0 && m_invunerability <= 0 && ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() >= 10.f) {
				std::cout << "cut" << std::endl;
				//if you left click
				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
					//show the animation
					ECS::GetComponent<AnimationController>(3).Reset();
					ECS::GetComponent<Transform>(3).SetPositionZ(45.f);
					ECS::GetComponent<AnimationController>(11).SetActiveAnim((ECS::GetComponent<AnimationController>(11).GetActiveAnim() / 4 == 0) ? 2 : 6);

					//check each bullet
					for (int i : m_bullet) {
						//if the bullet can be destroyed
						if (ECS::GetComponent<Bullet>(i).GetType() == 2) {
							//check if the attack hitbox is colliding with the bullet
							vec2 bulletPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY()), attackPos = vec2(ECS::GetComponent<Transform>(3).GetPositionX(), ECS::GetComponent<Transform>(3).GetPositionY());
							if (sqrt((bulletPos.x - attackPos.x) * (bulletPos.x - attackPos.x) + (bulletPos.y - attackPos.y) * (bulletPos.y - attackPos.y)) <= ECS::GetComponent<Transform>(3).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
								m_removeEntity.push_back(i);

								if (m_tutorialBullet == 2) {
									m_tutorialBullet++;
									std::string fileName = "green_bullet_tooltip.png";
									ECS::GetComponent<Sprite>(m_helpTooltip).LoadSprite(fileName, 200 * BackEnd::GetAspectRatio(), 20);
								}

								for (int i = 0; i < 5; i++) {
									particle.push_back(Particle());
									particle[particle.size() - 1].CreateParticle(1, bulletPos);
								}
							}
						}
						//if the bullet can be reflected
						else if (ECS::GetComponent<Bullet>(i).GetType() == 3) {
							//check if the attack hitbox is colliding with the bullet
							vec2 bulletPos = vec2(ECS::GetComponent<Transform>(i).GetPositionX(), ECS::GetComponent<Transform>(i).GetPositionY()), attackPos = vec2(ECS::GetComponent<Transform>(3).GetPositionX(), ECS::GetComponent<Transform>(3).GetPositionY());
							if (sqrt((bulletPos.x - attackPos.x) * (bulletPos.x - attackPos.x) + (bulletPos.y - attackPos.y) * (bulletPos.y - attackPos.y)) <= ECS::GetComponent<Transform>(3).GetScale().x / 2.f + ECS::GetComponent<Transform>(i).GetScale().x / 2.f) {
								ECS::GetComponent<Bullet>(i).SetVelocity(vec2(cos(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetRotationAngleZ()), sin(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetRotationAngleZ())));
								ECS::GetComponent<Sprite>(i).LoadSprite(GreenBulletSpriteRef, 3, 3);
								ECS::GetComponent<Bullet>(i).SetReflected(true);

								for (int i = 0; i < 5; i++) {
									particle.push_back(Particle());
									particle[particle.size() - 1].CreateParticle(1, bulletPos);
								}
							}
						}
					}
					ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).SetStamina(ECS::GetComponent<Stats>(EntityIdentifier::MainPlayer()).GetStamina() - 10);
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