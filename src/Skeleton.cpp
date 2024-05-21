#include "framework.h"
#include "shader.h"
#include "geometry.h"
#include "paramSurface.h"
#include "phong.h"
#include "gameObject.h"
#include "plane.h"
#include "scene.h"
#include "tank.h"
#include "phongText.h"
#include "pyramid.h"
#include "bullet.h"
#include "timeManager.h"


Shader GPU;
Shader groundShader;
float deltaTime = 0;
float lastFrame = 0;


Scene scene;
Tank* player;
Tank* enemy;
void onInitialization(){
	glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
	glViewport(0,0, windowWidth, windowHeight); 
	
	GPU.createShader("./shaders/phong.vert", "./shaders/phong.frag", "outColor");
	groundShader.createShader("./shaders/phongText.vert", "./shaders/phongText.frag", "outColor");
	Material* mat = new Phong(&GPU);
	Material* groundMat = new PhongText(&groundShader);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	GameObject* ground = new GameObject();
	ground->AddPrimitive(new Plane(groundMat, vec3(0,0,-0.01), vec3(200, 200, 1)));
	scene.AddObject(ground);
	player = new Tank(mat);
	enemy = new Tank(mat);
	enemy->pos = vec3(0, 5, 0);
	scene.AddTank(player);
	scene.AddTank(enemy);
	scene.AddObject(new Pyramid(mat, vec3(5, 10, 2)));

	lastFrame = glutGet(GLUT_ELAPSED_TIME) / 1000;
}

void onDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	scene.Render(deltaTime);
	Camera::GetInstance()->setTarget(vec3(0, 0, 1.1) + player->pos, player->GetLookDirection());
	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int pX, int pY){
	if (key == 'o') {
		player->ChangeSpeedRight(0.1);
	}
	if (key == 'l') {
		player->ChangeSpeedRight(-0.1);
	}
	if (key == 'q') {
		player->ChangeSpeedLeft(0.1);
	}
	if (key == 'a') {
		player->ChangeSpeedLeft(-0.1);
	}
	if (key == 'b') {
		player->RotateTurret(M_PI/12);
	}
	if (key == 'n') {
		player->RotateTurret(-M_PI/12);
	}
	if (key == 'w') {
		player->LiftCanon(M_PI/36);
	}
	if (key == 's') {
		player->LiftCanon(-M_PI/36);
	}
	if (key == ' ') {
		player->Shoot();
	}
}

void onKeyboardUp(unsigned char key, int pX, int pY){}

void onMouseMotion(int pX, int pY){

}

void onMouse(int button, int state, int pX, int pY) {
}

void onIdle() {
	//TODO: Time class
	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentFrame - lastFrame) / 1000;
	lastFrame = currentFrame;
	Time::Update();
	glutPostRedisplay();
}
