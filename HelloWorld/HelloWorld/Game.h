#ifndef __GAME_H__
#define __GAME_H__

#include "BackEnd.h"
#include "HelloWorld.h"
#include "MainMenu.h"
#include "Xinput.h"

//Our main class for running our game
class Game
{
public:
	//Empty constructor
	Game() { };
	//Deconstructor for game
	//*Unloads window
	~Game();

	//Initiaiizes game
	//*Seeds random
	//*Initializes SDL
	//*Creates Window
	//*Initializes GLEW
	//*Create Main Camera Entity
	//*Creates all other entities and adds them to register
	void InitGame();

	//Runs the game
	//*While window is open
	//*Clear window
	//*Update 
	//*Draw
	//*Poll events
	//*Flip window
	//*Accept input
	bool Run();
	
	//Updates the game
	//*Update timer
	//*Update the rendering system
	//*Update the animation system
	void Update();

	//Runs the GUI
	//*Uses ImGUI for this
	void GUI();

	//Check events
	//*Checks the results of the events that have been polled
	void CheckEvents();

	/*Input Functions*/
	void AcceptInput();
	void GamepadInput();

	void GamepadStroke(XInputController* con);
	void GamepadUp(XInputController* con);
	void GamepadDown(XInputController* con);
	void GamepadStick(XInputController* con);
	void GamepadTrigger(XInputController* con);
	void KeyboardHold();
	void KeyboardDown();
	void KeyboardUp();

	//Mouse input
	void MouseMotion(SDL_MouseMotionEvent evnt);
	void MouseClick(SDL_MouseButtonEvent evnt);
	void MouseWheel(SDL_MouseWheelEvent evnt);
private:
	//The window
	Window *m_window = nullptr;

	//Scene name
	std::string m_name;
	//Clear color for when we clear the window
	vec4 m_clearColor;
	
	//The main register for our ECS
	entt::registry* m_register;

	//Scenes
	Scene* m_activeScene;
	std::vector<Scene*> m_scenes;
	
	//Imgui stuff
	bool m_guiActive = false;

	//Hooks for events
	bool m_close = false;
	bool m_motion = false;
	bool m_click = false;
	bool m_wheel = false;
	
	//Game variables
	std::vector<int> m_bullet; //all bullet entities
	std::vector<int> m_enemy; //all enemy entities
	std::vector<int> m_removeEntity; //all entities that need to be removed
	std::vector<Bullet> m_offscreenBullet; //all the information needed for offscreen bullets
	std::vector<unsigned int> m_offscreenEnemy; //all the offscreen enemies
	std::vector<unsigned int> m_offscreenEnemyPos; //all the offscreen enemy tips
	float m_invunerability = 0.f; //player invunerability timer (after being hit)
	float m_dodgeTimer = 0.f; //the time dodging
	vec2 m_dodgeDirection = vec2(0.f, 0.f); //the direction of motion when dodging
	float m_speedCap = 0.f; //caps the speed of the game so it doesnt run too fast
	float m_bossBulletOffsetSpiral = 0.f, m_bossBulletOffsetPulse = 0.f, m_bossBulletOffsetBig = 0.f; //adds a little bit of offset to the bullets to get some of the patterns
	int m_trackingBulletCount = 0; //how many bullets can track you
	bool m_initialStartup = true; // allows the game to load on the initial startup
	bool m_tutorial = true; //runs the tutorial
	int m_tutorialBullet = 1; //sets the type of the bullet in the tutorial
	int m_tooltip = 0;
	int m_helpTooltip = 0;
	bool startgame = false;
	bool pause = false;
};


#endif // !__GAME_H__

