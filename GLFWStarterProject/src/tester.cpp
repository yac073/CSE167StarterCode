////////////////////////////////////////
// Tester.cpp
////////////////////////////////////////

#include "Tester.h"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER=0;

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	TESTER=new Tester("Spinning Cube",argc,argv);
	glutMainLoop();
	delete TESTER;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void specialInput(int key, int x, int y)			{TESTER->SpecialInput(key, x, y); }
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(const char *windowTitle,int argc,char **argv) {
	WinX=1280;
	WinY=720;
	DEBUG = true;
	numOfParticlesPerRow = 50;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;
	windDir = vec3(0.0f, 0.0f, -5.0f);

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
	glDisable(GL_CULL_FACE);
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 100, 100 );
	WindowHandle = glutCreateWindow( windowTitle );
	glutSetWindowTitle( windowTitle );
	glutSetWindow( WindowHandle );
	

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );
	glutSpecialFunc(specialInput);

	// Initialize GLEW
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Initialize components
	DefaultShader=new ShaderProgram("Model.glsl",ShaderProgram::eRender);
	PointShader = new ShaderProgram("Point.glsl", ShaderProgram::eRender);
	CurveShader = new ShaderProgram("curveshader.glsl", ShaderProgram::eRender);
	Cube=new SpinningCube;
	Skel = new Skeleton;
	Anim = new Animation;
	particleSystem = new ParticleSystem(numOfParticlesPerRow *numOfParticlesPerRow);
	for (int i = 0; i < numOfParticlesPerRow; i++) {
		for (int j = 0; j < numOfParticlesPerRow; j++) {
			particleSystem->AddParticle(new Particle(.1f, vec3(i * .5f, -10.0,-50.f + j * .5f)));
		}
	}
	particleSystem->BuildStructure();
	//load anim
	auto fileName = argv[3];
	Anim->Load(fileName);

	//load skel
	fileName = argv[1];
	Skel->anim = Anim;
	Skel->Load(fileName);
	//load skin
	skin = new Skin(Skel, false);	
	fileName = argv[2];
	skin->Load(fileName);
	startT = glutGet(GLUT_ELAPSED_TIME);
	Cam=new Camera;
	Cam->SetAspect(float(WinX)/float(WinY));	
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	delete DefaultShader;
	delete CurveShader;	
	delete PointShader;
	delete particleSystem;
	delete Cube;
	delete Skel;
	delete Cam;
	delete Anim;

	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	// += (1.0f / 30.0f)
	nowT = glutGet(GLUT_ELAPSED_TIME);
	//Anim->Evaluate(startT / 1000.0f);
	particleSystem->Update((nowT - startT) / 1000.0f, windDir);
	//Skel->Update();
	//skin->Update();
	Cam->Update();
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
	startT = glutGet(GLUT_ELAPSED_TIME);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam->Reset();
	Cam->SetAspect(float(WinX)/float(WinY));

	Cube->Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0., 0., 0., 1.);

	// Draw components
	//Cube->Draw(Cam->GetViewProjectMtx(),Program->GetProgramID());
	//skin->Draw(Cam->GetViewProjectMtx(), DefaultShader->GetProgramID());
	//Skel->DrawCurve(Cam->GetViewProjectMtx(), CurveShader->GetProgramID());
	particleSystem->Draw(DEBUG, Cam->GetViewProjectMtx(), DefaultShader->GetProgramID());

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		windDir = glm::rotate(glm::mat4(1.0f), -1.0f / 18.0f * glm::pi<float>(), vec3(1.0f, .0f, .0f))*vec4(windDir, .0f);
		cout << "windDir" << windDir.x << " " << windDir.y << " " << windDir.z << endl;
		break;
	case GLUT_KEY_DOWN:
		windDir = glm::rotate(glm::mat4(1.0f), 1.0f / 18.0f * glm::pi<float>(), vec3(1.0f, .0f, .0f))*vec4(windDir, .0f);
		cout << "windDir" << windDir.x << " " << windDir.y << " " << windDir.z << endl;
		break;
	case GLUT_KEY_LEFT:
		windDir = glm::rotate(glm::mat4(1.0f), -1.0f / 18.0f * glm::pi<float>(), vec3(.0f, 1.0f, .0f))*vec4(windDir, .0f);
		cout << "windDir" << windDir.x << " " << windDir.y << " " << windDir.z << endl;
		break;
	case GLUT_KEY_RIGHT:
		windDir = glm::rotate(glm::mat4(1.0f), 1.0f / 18.0f * glm::pi<float>(), vec3(.0f, 1.0f, .0f))*vec4(windDir, .0f);
		cout << "windDir" << windDir.x << " " << windDir.y << " " << windDir.z << endl;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int maxDelta=100;
	int dx = glm::clamp(nx - MouseX,-maxDelta,maxDelta);
	int dy = glm::clamp(-(ny - MouseY),-maxDelta,maxDelta);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth()+dx*rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline()-dy*rate,-90.0f,90.0f));
	}
	if(RightDown) {
		const float rate=0.005f;
		float dist=glm::clamp(Cam->GetDistance()*(1.0f-dx*rate),0.01f,1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
