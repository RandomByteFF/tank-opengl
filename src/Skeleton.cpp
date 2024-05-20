#include "framework.h"
#include "shader.h"
#include "geometry.h"
#include "paramSurface.h"
#include "phong.h"
#include "gameObject.h"
#include "plane.h"
#include "scene.h"
#include "tank.h"


Shader GPU;
Shader groundShader;
float deltaTime = 0;
float lastFrame = 0;

// class Pyramid : public Drawable {
// 	std::vector<Triangle*> triangles;
// 	public:
// 	Pyramid(vec3 pos, vec3 scale = vec3(1,1,1), float rot = 0) {
// 		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot)));
// 		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot + M_PI/2)));
// 		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot + M_PI/2*2)));
// 		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot + M_PI/2*3)));
// 	}
// 	void Draw() {
// 		for (Triangle* t : triangles) {
// 			t->Draw();
// 		}
// 	}
// };

Scene scene;
Tank* player;
void onInitialization(){
	glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
	glViewport(0,0, windowWidth, windowHeight); 
	
	GPU.createShader("./shaders/phong.vert", "./shaders/phong.frag", "outColor");
	groundShader.createShader("./shaders/phongText.vert", "./shaders/phongText.frag", "outColor");
	Material* mat = new Phong(&GPU);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	player = new Tank(mat);
	scene.AddObject(player);

	lastFrame = glutGet(GLUT_ELAPSED_TIME) / 1000;
}

void onDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	scene.Render(deltaTime);
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
	glutPostRedisplay();
}
