#include <iostream>
#include <windows.h>

#include <GL/GLU.h>
#include <GLFW/glfw3.h>

#include "Application.h"

#include <set>

using namespace std;

const int RESOLUTION_X = 800;
const int RESOLUTION_Y = 600;

Application& app = Application::getInstance();
set <int> gDownKeys;
set <int> gDownMouseButtons;

/*
Assignment 3 : Top - Down Shooting Game
Implement a simple network multiplayer top-down shooting demo / game using C++ / Photon.
Submission due date : before the end of 14 December 2020, Monday
- Movement with inertia environment setup (same as the demo provided)
- Position prediction using velocity & acceleration + interpolation (refer to the demo)
- Using mouse to control shooting direction (turret direction), you should apply interpolation to smooth the other player's turret movement.
- At least 2 weapons : machine gun and homing missile (triggered using right and left mouse buttons)
- Indication (or effect) of getting hit by projectile.
- Max 8 players to join one arena.
- (Optional) Static obstacles(destructible or non-destructible)
- (Optional) Player health
- (Optional) Score / Death count
- (Optional) Pickups or powerup
- (Optional) Sounds + music
- (Optional) Respawn after death
- (Optional) Arena / game world that is larger than view space(refer to the diep.io)
*/

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//! Use ortho2D view
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		app.onKeyPressed (key);
		gDownKeys.insert (key);		
	}
	else if (action == GLFW_RELEASE)
	{
		app.onKeyReleased (key);
		gDownKeys.erase (key);		
	}	
}
static void keyHoldCallback ()
{
	for(set<int>::iterator iter = gDownKeys.begin(); iter != gDownKeys.end(); ++iter)
	{
		app.onKeyHold (*iter);
	}
}

static void mouse_button_callback (GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS)
	{		
		app.onMousePressed (window, button);
		gDownMouseButtons.insert (button);		
	}
	else if(action == GLFW_RELEASE)
	{
		app.onMouseReleased (button);
		gDownMouseButtons.erase (button);		
	}	
}
//! Called in main function
static void mouseButtonHoldCallback ()
{
	for(set <int>::iterator iter = gDownMouseButtons.begin (); iter != gDownMouseButtons.end (); ++iter)
	{
		app.onMouseHold(*iter);
	}
}

static void cursor_position_callback (GLFWwindow* window, double xPos, double yPos)
{
	app.onMouseMoved (xPos, yPos);
}

int main (void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create m_a windowed mode window and its OpenGL context */
	window = glfwCreateWindow (RESOLUTION_X, RESOLUTION_Y, "", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback (window, onWindowResized);
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	onWindowResized (window, RESOLUTION_X, RESOLUTION_Y);

	glfwSetKeyCallback (window, key_callback);
	glfwSetMouseButtonCallback (window, mouse_button_callback);
	glfwSetCursorPosCallback (window, cursor_position_callback);

	double prevTime = glfwGetTime();
	app.start ();

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		double elapsedTime = currentTime - prevTime;

		app.update(elapsedTime);
		app.draw ();

		keyHoldCallback ();
		mouseButtonHoldCallback ();

		glfwSwapBuffers (window);
		glfwPollEvents ();

		prevTime = currentTime;
	}

	glfwTerminate();
	return 0;
}