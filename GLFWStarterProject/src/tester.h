////////////////////////////////////////
// Tester.h
////////////////////////////////////////

#pragma once

#include "Shader.h"
#include "Camera.h"
#include "SpinningCube.h"
#include "Skeleton.h"
#include "skin.h"
#include "Animation.h"
#include "ParticleSystem.h"

////////////////////////////////////////////////////////////////////////////////

// The 'Tester' is a simple top level application class. It creates and manages a
// window with the GLUT extension to OpenGL and it maintains a simple 3D scene
// including a camera and some other components.

class Tester {	
public:
	Tester(const char *windowTitle,int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void SpecialInput(int key, int x, int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);

	bool DEBUG;
	int numOfParticlesPerRow;
private:
	// Window management
	int WindowHandle;
	int WinX,WinY;
	vec3 windDir;
	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Components
	ShaderProgram *DefaultShader, *CurveShader, *PointShader;
	SpinningCube *Cube;
	Skeleton * Skel;
	Camera *Cam;
	Skin * skin;
	Animation* Anim;
	ParticleSystem * particleSystem;
	double startT, nowT;	
};

////////////////////////////////////////////////////////////////////////////////
