//=============================================================================================
// Mintaprogram: Zold haromszog. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!! 
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak 
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Banati Benedek
// Neptun : BSA3ZV
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"

const char* vertSource = R"(
	#version 330
	precision highp float;

	uniform mat4 M, Minv, MVP;
	uniform vec3 wEye;
	uniform vec3 lDir;

	layout(location = 0) in vec3 vtxPos; // pos in model sp
	layout(location = 1) in vec3 vtxNorm;// normal in model sp
	layout(location = 2) in vec2 textPos;

	out vec3 wNormal;
	out vec3 wView;
	out vec3 wLight;

	void main() {
		gl_Position = vec4(vtxPos, 1) * MVP; // to NDC
		vec4 wPos = vec4(vtxPos, 1) * M;
		wLight = vec3(1, 2, 3);
		wView = wEye - wPos.xyz/wPos.w;
		wNormal = (Minv * vec4(vtxNorm, 0)).xyz;
	}
)";

const char* fragSource = R"(
	#version 330
	precision highp float;

	uniform vec3 kd, ks, ka;
	uniform float shine;
	uniform vec3 La, Le;

	in vec3 wNormal;
	in vec3 wView;
	in vec3 wLight;
	out vec4 outColor;

	void main() {
		vec3 N = normalize(wNormal);
		vec3 V = normalize(wView);
		vec3 L = normalize(wLight);
		vec3 H = normalize(L + V);
		float cost = max(dot(N, L), 0);
		float cosd = max(dot(N, H), 0);
		vec3 color = ka * La + (kd * cost + ks * pow(cosd, shine)) * Le;
		outColor = vec4(color, 1);
	}
)";

vec3 operator / (vec3 a, vec3 b) {
	return vec3((float)a.x / b.x, (float)a.y / b.y, (float)a.z / b.z);
}

class Camera {
	public:
	vec3 wEye, wLookat, wVup;
	float fov, asp, fp, bp;
	Camera() {
		fov = M_PI/4;
		asp = 1;
		wEye = vec3(0, -4, 1.5);
		wLookat = vec3(0,0,0);
		wVup = vec3(0, 0, 1);
		fp = 0.1;
		bp = 30;
	}

	mat4 V() {
		vec3 w = normalize(wEye - wLookat);
		vec3 u = normalize(cross(wVup, w));
		vec3 v = cross(w, u);
		return TranslateMatrix(-wEye) * mat4 (u.x, v.x, w.x, 0,
												u.y, v.y, w.y, 0,
												u.z, v.z, w.z, 0,
												0,0,0,1);
	}
	mat4 P() {
		float sy = 1/tanf(fov/2);
		return mat4(sy/asp, 0, 0, 0,
					0, sy, 0, 0,
					0, 0, -(fp+bp)/(bp-fp), -1,
					0, 0, -2*fp*bp/(bp-fp), 0);
	}

	vec3 GetEye() {
		return wEye;
	}
};
Camera camera;
GPUProgram GPU;
class Geometry {
	protected:
	unsigned int vao, vbo;
	public:
	Geometry() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
	}

	virtual void Draw() = 0;
	~Geometry() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
};

class ParamSurface : public Geometry {
	unsigned int nVtxStrip, nStrips;
	protected:
	vec3 scale, rot, pos;
	struct VertexData {
		vec3 pos, norm;
		vec2 tex;
	};
	virtual VertexData GenVertexData(float u, float v) = 0;

	public:
	void Create(size_t N, size_t M) {
		nVtxStrip = (M + 1) * 2;
		nStrips = N;
		std::vector<VertexData> vtxData;
		for (size_t i = 0; i < N; i++)
		{
			for (size_t j = 0; j <= M; j++)
			{
				vtxData.push_back(GenVertexData((float) j/M, (float) i/N));
				vtxData.push_back(GenVertexData((float) j/M, (float) (i+1)/N));
			}
		}
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vtxData.size() * sizeof(VertexData), &vtxData[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, norm));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tex));
	}

	void Draw() {
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

		GPU.setUniform(M, "M");
		GPU.setUniform(Minv, "Minv");
		GPU.setUniform(MVP, "MVP");
		vec3 eye = camera.GetEye();
		GPU.setUniform(eye, "wEye");
		GPU.setUniform(vec3(0.4, 0.4, 0.4), "kd");
		GPU.setUniform(vec3(1, 1, 1), "ks");
		GPU.setUniform(vec3(0.2, 0.2, 0.2), "ka");
		GPU.setUniform(5.f, "shine");
		GPU.setUniform(vec3(1, 1, 1), "La");
		GPU.setUniform(vec3(1, 1, 1), "Le");
		//GPU.setUniform(vec3(1, 2, 3), "lDir");

		glBindVertexArray(vao);
		for (size_t i = 0; i < nStrips; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i*nVtxStrip, nVtxStrip);
		}
	}
};

class Plane : public ParamSurface {
	public:
	Plane(vec3 pos, vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0)) {
		this->pos = pos;
		this->scale = scale;
		this->rot = rot;
		this->Create(3,3);
	}
	VertexData GenVertexData(float u, float v) {
		VertexData vd;
		vd.tex = vec2(u,v);
		vd.pos = vec3(u-0.5, v-0.5, 0);
		vd.norm = vec3(0, 0, 1);
		return vd;
	}
};
class Cylinder : public ParamSurface {
	public:
	Cylinder(vec3 pos, vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0)) {
		this->pos = pos;
		this->scale = scale;
		this->rot = rot;
		this->Create(2, 30);
	}
	VertexData GenVertexData(float u, float v) {
		VertexData vd;
		vd.tex = vec2(u,v);
		vd.pos = vec3(cosf(2*M_PI*u), sinf(2*M_PI*u), v-0.5);
		vd.norm = vec3(vd.pos.x, vd.pos.y, 0);
		return vd;
	}
};


std::vector<Geometry*> objects;
void onInitialization(){
	glViewport(0,0, windowWidth, windowHeight); 
	objects.push_back(new Plane(vec3(0,0,0), vec3(1,1,1)));
	objects.push_back(new Cylinder(vec3(0, 0, 0)));
	GPU.create(vertSource, fragSource, "outColor");
	glEnable(GL_DEPTH_TEST);
	// glDisable(GL_CULL_FACE);
}

void onDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	for(Geometry* g : objects) {
		g->Draw();
	}
	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int pX, int pY){
	if (key == 'w') {
		camera.wEye = camera.wEye + vec3(0, 1, 0);
		camera.wLookat = camera.wLookat + vec3(0, 1, 0);
		glutPostRedisplay();
	}
	if (key == 's') {
		camera.wEye = camera.wEye + vec3(0, -1, 0);
		camera.wLookat = camera.wLookat + vec3(0, -1, 0);
		glutPostRedisplay();
	}
	if (key == 'a') {
		camera.wEye = camera.wEye + vec3(-1, 0, 0);
		camera.wLookat = camera.wLookat + vec3(-1, 0, 0);
		glutPostRedisplay();
	}
	if (key == 'd') {
		camera.wEye = camera.wEye + vec3(1, 0, 0);
		camera.wLookat = camera.wLookat + vec3(1, 0, 0);
		glutPostRedisplay();
	}
	if (key == 'q') {
		camera.wEye = camera.wEye + vec3(0, 0, -1);
		camera.wLookat = camera.wLookat + vec3(0, 0, -1);
		glutPostRedisplay();
	}
	if (key == 'e') {
		camera.wEye = camera.wEye + vec3(0, 0, 1);
		camera.wLookat = camera.wLookat + vec3(0, 0, 1);
		glutPostRedisplay();
	}
}

void onKeyboardUp(unsigned char key, int pX, int pY){}

void onMouseMotion(int pX, int pY){

}

void onMouse(int button, int state, int pX, int pY) {
}

void onIdle() {}