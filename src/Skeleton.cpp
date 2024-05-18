#include "framework.h"
#include "shader.h"
#include "geometry.h"
#include "paramSurface.h"


Shader GPU;
Shader groundShader;
float deltaTime = 0;
float lastFrame = 0;

Camera camera;








class Track : public Plane {
	bool right = true;
	size_t num;
	public:

	Track(bool right, size_t num) : Plane(vec3(0,0,0), vec3(0.4, 0.1, 1)){
		this->right = right;
		this->num = num;
		Animate(vec3(0, 1, 0), vec3(0,0,0), 0);
	}

	void Animate(vec3 facing, vec3 tPos, float t, float angle = 0) {
		//0-1.8
		//1.8-2.7
		//2.7-4.5
		//4.5-5.4
		t += num*0.15;
		if (t < 0) t = 5.4+t;
		float n = fmod(t, 5.4);
		vec3 d;
		if (n < 1.8) {
			rot = vec3(0,0,0);
			d = vec3(0, n, 0);
		}
		else if (n < 2.7) {
			float dl = n - 1.8;
			rot = vec3(-dl/0.3, 0, 0);
			d = vec3(0, 1.8 + 0.3* sinf(dl/0.3), -0.3*(1-cosf(dl/0.3)));
		}
		else if (n < 4.5) {
			rot = vec3(M_PI, 0, 0);
			d = vec3(0, 4.5-n, -0.6);
		}
		else {
			float dl = 5.4-n;
			rot = vec3(dl/0.3, 0, 0);
			d = vec3(0, - 0.3* sinf(dl/0.3), -0.3*(1-cosf(dl/0.3)));
		}

		vec3 toRot = d + vec3((right ?  1: -1)*0.75, -0.9, 0.6);
		vec4 r = vec4(toRot.x, toRot.y, toRot.z, 1) * RotationMatrix(angle, vec3(0,0,1));
		pos = vec3(r.x, r.y, r.z) + tPos;

		rot = rot + vec3(0, 0, angle);
	}
};

class Ground : public Plane {
	Texture t;
	std::vector<vec4> texture;
	public:
	Ground() : Plane(vec3(0,0,-0.001), vec3(1000, 1000, 1)){
		groundShader.Use();
		texture = std::vector<vec4>(100*100);
		for (size_t i = 0; i < 100*100; i++)
		{
			texture[i] = vec4((float)rand()/RAND_MAX/4, (float)rand()/RAND_MAX/2, (float)rand()/RAND_MAX/5, 1);
		}
		
		t.create(100, 100, texture, GL_LINEAR);
		GPU.Use();
	}
	void Draw() {
		groundShader.Use();
		mat4 M = ScaleMatrix(scale) *
		RotationMatrix(rot.x, vec3(1, 0, 0))*
		RotationMatrix(rot.y, vec3(0, 1, 0)) *
		RotationMatrix(rot.z, vec3(0, 0, 1))*
		TranslateMatrix(pos);
		mat4 Minv = TranslateMatrix(-pos)*	
		RotationMatrix(-rot.z, vec3(0, 0, 1))*	
		RotationMatrix(-rot.y, vec3(0, 1, 0)) *
		RotationMatrix(-rot.x, vec3(1, 0, 0))*
		ScaleMatrix(vec3(1,1,1)/scale);
		mat4 MVP = M*camera.V()*camera.P();

		groundShader.setUniform(M, "M");
		groundShader.setUniform(Minv, "Minv");
		groundShader.setUniform(MVP, "MVP");
		groundShader.setUniform(vec3(0.1, 0.1, 0.1), "ks");
		groundShader.setUniform(vec3(0.2, 0.2, 0.2), "ka");
		groundShader.setUniform(100.f, "shine");
		groundShader.setUniform(vec3(1, 1, 1), "La");
		groundShader.setUniform(vec3(1, 1, 1), "Le");
		
		for (size_t i = 0; i < nStrips; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i*nVtxStrip, nVtxStrip);
		}
		GPU.Use();
	}
};

class Tank {
	vec3 pos;
	vec3 facing = vec3(0, 1, 0);
	float rot;
	float sRight = 0;
	float sLeft = 0;
	float animRight = 0;
	float animLeft = 0;
	float baseRot = 0;
	float canonLift = 0;

	Cylinder* base;
	Cylinder* canon;
	Triangle* backPlate;
	Triangle* frontPlate;
	Circle* topPlate;
	std::vector<Track*> rightTracks;
	std::vector<Track*> leftTracks;

	bool feq(float a, float b) {
		return abs(a-b) < 0.0001;
	}
	public:
	void Init() {
		base = new Cylinder(vec3(0, 0, 0.3), vec3(1.1, 1.1, 0.8));
		canon = new Cylinder(vec3(0, 0, 0.9), vec3(0.2, 0.2, 1.5), vec3(M_PI/2*3, 0, 0));
		backPlate = new Triangle(vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, 0));
		frontPlate = new Triangle(vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, M_PI));
		topPlate = new Circle(vec3(0, 0, 1.1), vec3(1.1, 1.1, 1));
		for (size_t i = 0; i < 36; i++)
		{
			rightTracks.push_back(new Track(true, i));
		}
		for (size_t i = 0; i < 36; i++)
		{
			leftTracks.push_back(new Track(false, i));
		}
	}

	void Move(vec3 pos) {
		this->pos = pos;
		base->offset = pos;
		canon->offset = pos;
		backPlate->offset = pos;
		frontPlate->offset = pos;
		topPlate->offset = pos;
		
		animRight += sRight * deltaTime;
		animLeft += sLeft * deltaTime;
		animRight = fmod(animRight, 5.4);
		animLeft = fmod(animLeft, 5.4);
		for (Track* t : rightTracks) {
			t->Animate(facing, pos, animRight, rot);
		}
		for (Track* t : leftTracks) {
			t->Animate(facing, pos, animLeft, rot);
		}
	}

	void Move(float angle) {
		rot = angle;
		facing = normalize(vec3(sinf(-angle), cosf(-angle), 0));
		base->rOffset = vec3(0, 0, angle + baseRot);
		canon->rOffset = vec3(canonLift, 0, angle + baseRot);
		canon->offset = vec3(pos.x + 0.3*sinf(-angle), pos.y + 0.3*cosf(-angle), 0);
		backPlate->rOffset = vec3(0, 0, angle);
		frontPlate->rOffset = vec3(0, 0, angle);
		topPlate->rOffset = vec3(0, 0, angle);
	}

	void MoveY(float pos) {
		Move(this->pos + facing*pos);
	}
	void RotateLeft(float rot) {
		Move(this->rot + rot);
		vec3 c = pos + normalize(vec3(-facing.y, facing.x, 0)) * 0.75;
		vec3 p_ = pos - c;
		vec4 p = vec4(p_.x, p_.y, p_.z, 1) * RotationMatrix(rot, vec3(0, 0, 1));
		Move(vec3(p.x, p.y, p.z) + c);
	}
	void RotateRight(float rot) {
		Move(this->rot - rot);
		vec3 c = pos + normalize(vec3(facing.y, -facing.x, 0)) * 0.75;
		vec3 p_ = pos - c;
		vec4 p = vec4(p_.x, p_.y, p_.z, 1) * RotationMatrix(-rot, vec3(0, 0, 1));
		Move(vec3(p.x, p.y, p.z) + c);
	}

	void Animate() {
		RotateLeft(1.5*sRight * deltaTime);
		RotateRight(1.5*sLeft * deltaTime);
		if (feq(sRight+sLeft, 0)) {
			MoveY(0);
		}
		else if (sRight > 0 && sLeft > 0) {
			MoveY((sRight < sLeft ? sRight : sLeft) * deltaTime);
		}
		else if (sRight < 0 && sLeft < 0) {
			MoveY((sRight > sLeft ? sRight : sLeft) * deltaTime);
		}
		else {
			MoveY((abs(sRight) < abs(sLeft) ? sRight : sLeft) * deltaTime);
		}
		camera.setTarget(pos + vec3(0, 0, 1.2), facing);
	}

	void Draw() {
		Animate();
		base->Draw();
		canon->Draw();
		backPlate->Draw();
		frontPlate->Draw();
		topPlate->Draw();
		for (Track* t : rightTracks) {
			t->Draw();
		}
		for (Track* t : leftTracks) {
			t->Draw();
		}
	}

	void changeSpeedLeft(float s) {
		sLeft += s;
	}
	void changeSpeedRight(float s) {
		sRight += s;
	}
	void rotateBase(float angle) {
		baseRot += angle;
	}
	void liftCanon(float angle) {
		canonLift += angle;
	}
	~Tank() {
		delete base;
		delete canon;
		delete backPlate;
		delete frontPlate;
		delete topPlate;
		for (Track* t : rightTracks) {
			delete t;
		}
		for (Track* t : leftTracks) {
			delete t;
		}
	}
};

class Pyramid : public Drawable {
	std::vector<Triangle*> triangles;
	public:
	Pyramid(vec3 pos, vec3 scale = vec3(1,1,1), float rot = 0) {
		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot)));
		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot + M_PI/2)));
		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot + M_PI/2*2)));
		triangles.push_back(new Triangle(pos, scale, vec3(M_PI/3, 0, rot + M_PI/2*3)));
	}
	void Draw() {
		for (Triangle* t : triangles) {
			t->Draw();
		}
	}
};

std::vector<Drawable*> objects;
Tank tank;
void onInitialization(){
	glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
	glViewport(0,0, windowWidth, windowHeight); 
	
	GPU.createShader("./shaders/phong.vert", "./shaders/phong.frag", "outColor");
	groundShader.createShader("./shaders/phongText.vert", "./shaders/phongText.frag", "outColor");
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	objects.push_back(new Ground());
	objects.push_back(new Pyramid(vec3(40, 0, 6), vec3(7, 7, 7), 10));
	objects.push_back(new Pyramid(vec3(35, 20, 5), vec3(6, 6, 6), 30));
	objects.push_back(new Pyramid(vec3(35, -5, 3), vec3(4, 4, 4), 5));
	tank.Init();
	tank.RotateRight(M_PI/2);
	tank.Move(vec3(0, 0, 0));
	lastFrame = glutGet(GLUT_ELAPSED_TIME) / 1000;
}

void onDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	for(Drawable* g : objects) {
		g->Draw();
	}
	tank.Draw();
	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int pX, int pY){
	if (key == 'o') {
		tank.changeSpeedRight(0.1);
	}
	if (key == 'l') {
		tank.changeSpeedRight(-0.1);
	}
	if (key == 'q') {
		tank.changeSpeedLeft(0.1);
	}
	if (key == 'a') {
		tank.changeSpeedLeft(-0.1);
	}
	if (key == 'b') {
		tank.rotateBase(M_PI/12);
	}
	if (key == 'n') {
		tank.rotateBase(-M_PI/12);
	}
	if (key == 'w') {
		tank.liftCanon(M_PI/36);
	}
	if (key == 's') {
		tank.liftCanon(-M_PI/36);
	}
	// if (key == 'a') {
	// 	tank.MoveY(1);
	// }
}

void onKeyboardUp(unsigned char key, int pX, int pY){}

void onMouseMotion(int pX, int pY){

}

void onMouse(int button, int state, int pX, int pY) {
}

void onIdle() {
	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentFrame - lastFrame) / 1000;
	lastFrame = currentFrame;
	glutPostRedisplay();
}
