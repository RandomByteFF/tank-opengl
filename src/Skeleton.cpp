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
#define RES 600
#define AMBIENT vec3(0.4,0.4,0.4)
#define FOV 45
#define MAXDEPTH 4

const char* vertSource = R"(
	#version 330
	precision highp float;

	layout(location = 0) in vec2 pos;
	layout(location = 1) in vec2 textPos;

	out vec2 textCord;
	void main() {
		gl_Position = vec4(pos.x, pos.y, 0, 1);
		textCord = textPos;
	}
)";

const char* fragSource = R"(
	#version 330
	precision highp float;

	uniform vec4 color;
	uniform sampler2D text;
	in vec2 textCord;
	out vec4 outColor;

	void main() {
		outColor = texture(text, textCord);
	}
)";

float clamp(float a) {
	if(a < 0) return 0;
	if (a > 1) return 1;
	return a;
}
GPUProgram GPU;
struct Ray {
	vec3 start;
	vec3 dir;
	bool out;
};
vec3 trace(Ray r, int d = 0);

class Camera {
	vec3 eye;
	vec3 lookat = vec3(0,0,0);
	vec3 up;
	vec3 right;

	public:
		Ray getRay(float X, float Y) {
			Ray r;
			r.start = eye;
			vec3 p = lookat + (2*(X + 0.5)/RES-1)*right + (2*(Y + 0.5)/RES-1)*up;
			r.dir = normalize(p-eye);
			return r;
		}
		void setPos(vec3 eyePos) {
			eye = eyePos;
			vec3 idir = eye-lookat;

			right = normalize(cross(vec3(0,1,0), idir)) * tanf(0.25 * M_PI / 2) * length(idir);
			up = normalize(cross(idir, right)) * tanf(0.25 * M_PI / 2) * length(idir);
		}
};
Camera camera;

class Renderer {
	unsigned int vao, vbo;
	unsigned int textureId;
	size_t s = 40;
	float points[16] = {-1, -1, 0, 0,
						1, -1, 1, 0,
						-1, 1, 0, 1,
						1, 1, 1, 1};
	std::vector<vec4> texture;
	public:
		void Init() {
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(0*sizeof(float)));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
			glEnableVertexAttribArray(1);
			glGenTextures(1, &textureId);
			texture = std::vector<vec4>(RES*RES);
			UpdateGPU();
			RenderToTexture();
		}
		void UpdateGPU() {
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &points[0], GL_DYNAMIC_DRAW);
		}
		void Draw() {
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		void RenderToTexture() {
			for (size_t y = 0; y < 600; y++)
			{
				for (size_t x = 0; x < 600; x++)
				{
					size_t index = x + y*RES;
					Ray r = camera.getRay(x, y);
					vec3 t = trace(r);
					//texture[index] = vec4(1, (float)j/600, 0, 1);
					texture[index] = vec4(t.x, t.y, t.z, 1);
				}
			}
			glBindTexture(GL_TEXTURE_2D, textureId);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RES, RES, 0, GL_RGBA, GL_FLOAT, &texture[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
};

class Material {
	public:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 n; //tores
	vec3 k; //kioltas
	float shiny;
	bool rough = false;
	bool reflective = false;
	bool refractive = false;
	Material(vec3 diffuse, vec3 specular, float shiny) {
		rough = true;
		this->diffuse = diffuse;
		this->specular = specular;
		this->ambient = 3 * diffuse;
		this->shiny = shiny;
	}
	Material(bool reflective, bool refractive, vec3 n, vec3 k = vec3(0,0,0)) { 
		this->reflective = reflective;
		this->refractive = refractive;
		this->n = n;
		this->k = k;
	}
};

struct Hit {
	float t;
	vec3 position;
	vec3 normal;
	Material* material;
	Hit() { t = -1; }
};

class Intersectable {
	public:
	Material* material;
	virtual Hit intersect(const Ray& ray) = 0;
	~Intersectable() {
		delete material;
	}
};

class Plane : public Intersectable {
	vec3 center = vec3(0, -1, 0);
	Material* material2;
	public:
	Plane() {
		material = new Material(vec3(0, 0.1, 0.3), vec3(0,0,0), 0);
		material2 = new Material(vec3(0.3, 0.3, 0.3), vec3(0,0,0), 0);
	}
	Hit intersect(const Ray& ray) {
		Hit t;
		if (ray.dir.y == 0) return t;
		float dist = (float)(center.y - ray.start.y) / ray.dir.y;
		vec3 pos = dist * ray.dir + ray.start;
		if (!(pos.x < 10 && pos.z < 10 && pos.x > -10 && pos.z > -10)) return t;
		t.t = dist;
		t.position = pos;
		t.normal = vec3(0, 1, 0);
		int x = -pos.z+20;
		int y = -pos.x+20;
		bool blue = (x % 2 == 1 && y %2  == 0) || (x%2==0 && y%2==1);
		t.material = blue ? material : material2;

		return t;
	}
	~Plane() {
		delete material2;
	}
};

class Cone : public Intersectable {
	vec3 p;
	vec3 n;
	float alpha = 0.2;

	public:
	Cone(Material *material, vec3 p, vec3 n, float alpha) {
		this->material = material;
		this->p = p;
		this->n = normalize(n);
		this->alpha = alpha;
	}
	Hit intersect(const Ray& ray) {
		//source: https://www.youtube.com/watch?v=x7U_GHGXhJg
		vec3 i = ray.start-p;
		float acs = powf(cosf(alpha),2);

		float a = powf(dot(ray.dir, n), 2) - dot(ray.dir, ray.dir) * acs;
		float b = 2 * dot(ray.dir, n) * dot(i, n) - 2 * dot(ray.dir, i) * acs;
		float c = powf(dot(i, n), 2) - dot(i,i) * acs;

		float disc = pow(b,2) - 4 * a * c;
		Hit h;
		if (disc < 0) return h;
		float sdisc = sqrtf(disc);
		float t1 = (float)(-b + sdisc)/(2*a);
		float t2 = (float)(-b - sdisc)/(2*a);
		float t = t1 < t2 ? t1 : t2;
		h.position = ray.start + ray.dir * t;
		float dist = dot((h.position - p), n);
		if (dist < 0 || dist > 2) return Hit();
		h.material = material;
		h.t = t;
		h.normal = normalize(2 * dot(h.position - p, n) * n - 2 * (h.position - p)*acs);
		return h;
	}
};

class Cylinder : public Intersectable {
	vec3 p;
	vec3 v;
	float r;
	float height;
	public:
	Cylinder(Material* material, vec3 p, vec3 v, float r, float height) {
		this->material = material;
		this->p = p;
		this->v = normalize(v);
		this->r = r;
		this->height = height;
	}
	Hit intersect(const Ray& ray) {
		//source: https://www.youtube.com/watch?v=x7U_GHGXhJg
		float a = dot(ray.dir, ray.dir) - 2 * dot(ray.dir, v*dot(ray.dir, v)) + dot(v * dot(ray.dir, v), v * dot(ray.dir, v));
		float b = dot(2*(ray.dir - v * dot(ray.dir, v)), ray.start - p - v*(dot(ray.start,v)-dot(p,v)));
		float c = dot(ray.start - p - v * (dot(ray.start, v) - dot(p,v)), ray.start - p - v * dot(ray.start - p, v)) - powf(r,2);

		float disc = pow(b,2) - 4 * a * c;
		Hit h;
		if (disc < 0) return h;
		float sdisc = sqrtf(disc);
		float t1 = (float)(-b + sdisc)/(2*a);
		float t2 = (float)(-b - sdisc)/(2*a);
		float t = t1 < t2 ? t1 : t2;
		h.position = ray.start + ray.dir * t;
		float height_ = dot(h.position - p, v);
		if (height_ < 0 || height_ > height) return Hit();
		h.material = material;
		h.t = t;
		h.normal = normalize((h.position-p)-(v*dot(h.position - p, v)));
		return h;
	}
};

struct LightSource {
	vec3 dir;
	vec3 color;
};

std::vector<Intersectable*> objects;
std::vector<LightSource> lightSources;

Hit firstIntersect(Ray ray) {
	Hit bestHit;
	
	for(Intersectable *obj : objects) {
		Hit hit = obj->intersect(ray);
		if (hit.t > 0 && (bestHit.t < 0 || hit.t < bestHit.t)) bestHit = hit;
	}
	if (dot(ray.dir, bestHit.normal) > 0) bestHit.normal = bestHit.normal * -1;
	return bestHit;
}

vec3 refract(vec3 v, vec3 n, float ns) {
	float cosa = -dot(v, n);
	float disc = 1- (1- cosa * cosa)/ns/ns;
	if(disc < 0) return vec3(0,0,0);
	return v/ns + n * (cosa/ns - sqrt(disc));
}

vec3 reflect(vec3 v, vec3 n) {
	return v - n * dot(n, v) * 2;
}

vec3 operator / (vec3 a, vec3 b) {
	return vec3((float)a.x / b.x, (float)a.y / b.y, (float)a.z / b.z);
}

vec3 Fresnel(vec3 V, vec3 N, vec3 n, vec3 kappa) {
	float cosa = -dot(V,N);
	vec3 one(1,1,1);
	vec3 f0 = ((n-one) * (n-one) + kappa*kappa) /
			((n+one) * (n+one) + kappa*kappa);
	return f0 + (one-f0)* powf(1-cosa, 5);
}

//La = ambient
//L^in_L = l. lámpa
//k_a = ambient coefficent
//k_d = diffúz
//k_s = spekuláris

vec3 trace(Ray ray, int d) {
	if (d > MAXDEPTH) return AMBIENT;

	Hit hit = firstIntersect(ray);
	if(hit.t < 0) return AMBIENT;
	vec3 outRad(0,0,0);

	if (hit.material->rough) {
		outRad = AMBIENT * hit.material->ambient;

		for(LightSource l : lightSources) {
			Ray shadowRay;
			shadowRay.start = hit.position + (hit.normal *0.001);
			shadowRay.dir = l.dir;
			Hit shadowHit = firstIntersect(shadowRay);
			if (shadowHit.t < 0) {
				float dIntensity = clamp(dot(l.dir, hit.normal));
				vec3 diffuse = dIntensity * hit.material->diffuse;

				vec3 h = normalize(l.dir + -ray.dir);
				float sIntensity = powf(clamp(dot(hit.normal, h)), hit.material->shiny);
				vec3 specular = sIntensity * hit.material->specular;
				outRad = outRad + (specular + diffuse)*2;
			}
		}
	}

	if (hit.material->reflective) {
		Ray reflectRay;
		reflectRay.start = hit.position + (hit.normal *0.001);
		reflectRay.dir = reflect(ray.dir, hit.normal); //FIXME: might have to normalize these values
		reflectRay.out = ray.out;
		outRad = outRad + trace(reflectRay, d+1) * Fresnel(ray.dir, hit.normal, hit.material->n, hit.material->k);
	}

	if (hit.material->refractive) {
		float ior = ray.out ? hit.material->n.x : (float) 1/hit.material->n.x;
		vec3 refractionDir = refract(ray.dir, hit.normal, ior);
		if (length(refractionDir) > 0) {
			Ray refractRay;
			refractRay.start = hit.position - hit.normal*0.001;
			refractRay.dir = refractionDir;
			refractRay.out = !ray.out;
			outRad = outRad + trace(refractRay, d+1) * (vec3(1,1,1)-Fresnel(ray.dir, hit.normal, hit.material->n, hit.material->k));
		}
	}

	return outRad;
}

Renderer renderer;

void onInitialization(){
	objects.push_back(new Plane());
	Material* cone1 = new Material(vec3(0.1, 0.2, 0.3), vec3(2,2,2), 100);
	objects.push_back(new Cone(cone1, vec3(0,1,0), vec3(-0.1, -1, -0.05), 0.2)); //cyan cone
	Material* cone2 = new Material(vec3(0.3, 0, 0.2), vec3(2,2,2), 20);
	objects.push_back(new Cone(cone2, vec3(0,1,0.8), vec3(0.2, -1, 0), 0.2)); //magenta cone
	Material* cylinder1 = new Material(true, false, vec3(0.17, 0.35, 1.5), vec3(3.1, 2.7, 1.9)); 
	objects.push_back(new Cylinder(cylinder1, vec3(1, -1, 0), vec3(0.1, 1, 0), 0.3, 2)); //gold cylinder
	Material* cylinder2 = new Material(true, true, vec3(1.3, 1.3, 1.3)); //FIXME: idk if i need to set reflective to true
	objects.push_back(new Cylinder(cylinder2, vec3(0, -1, -0.8), vec3(-0.2, 1, -0.1), 0.3, 2)); //water cylinder
	Material* cylinder3 = new Material(vec3(0.3, 0.2, 0.1), vec3(2,2,2), 50);
	objects.push_back(new Cylinder(cylinder3, vec3(-1, -1, 0), vec3(0, 1, 0.1), 0.3, 2)); //orange cylinder
	LightSource s;
	s.color = vec3(1,1,1);
	s.dir = normalize(vec3(1,1,1));
	lightSources.push_back(s);
	glViewport(0,0, windowWidth, windowHeight);
	GPU.create(vertSource, fragSource, "outColor");
	camera.setPos(vec3(0, 1, 4));
	renderer.Init();
}

void onDisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.Draw();
	glutSwapBuffers();
}

float animStartTime;
int presses = 0;
void onKeyboard(unsigned char key, int pX, int pY){
	switch (key)
	{
		case 'a':
			presses++;
			float rad = (float)(2 * M_PI / 8) * presses;
			vec3 pos = vec3(sinf(rad)*4, 1, cosf(rad)*4);
			camera.setPos(pos);
			renderer.RenderToTexture();
			glutPostRedisplay();
			break;
	}
}

void onKeyboardUp(unsigned char key, int pX, int pY){}

void onMouseMotion(int pX, int pY){
}

void onMouse(int button, int state, int pX, int pY) {
}

void onIdle() {}