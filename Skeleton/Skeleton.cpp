//=============================================================================================
// Mintaprogram: Zöld háromszög. Ervenyes 2018. osztol.
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
// Nev    : 
// Neptun : 
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

// vertex shader in GLSL: It is a Raw string (C++11) since it contains new line characters
const char * const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	uniform mat4 MVP;			// uniform variable, the Model-View-Projection transformation matrix
	layout(location = 0) in vec2 vp;	// Varying input: vp = vertex position is expected in attrib array 0
	layout(location = 1) in vec3 vertexColor;			// Attrib Array 1

	out vec3 finalcolor;

	void main() {
		finalcolor = vertexColor; 
		gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP;		// transform vp from modeling space to normalized device space
	}
)";

// fragment shader in GLSL
const char * const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers
	
	uniform vec3 color;		// uniform variable, the color of the primitive
	in vec3 finalcolor;
	out vec4 outColor;		// computed color of the current pixel
	

	void main() {
		outColor = vec4(finalcolor, 1);	// computed color is the color of the primitive
	}
)";

vec2 monocyclepos;
vec2 pathpos;

class Circle{

public:
	unsigned int vbo;
	float radius;
	vec2 center;
	vec3 color;

	vec2 speed;
	float rotation;
	bool state;

	Circle() {};

	Circle(float r, vec2 c,vec3 col) {
		glGenBuffers(1, &vbo); //generat a buffer 
		radius = r;
		center = c; 
		color = col;
		
		speed = vec2(c.x, c.y);
		state = false;
	}

	void Animate(float rot, float transl, float pos) {

		

			if (state == false ) {
				rotation = rot;
				speed.x -= transl;
				speed.y = pos;
				if (speed.x < -1.0f) {
					state = true;
				}
			}

			if(state == true){
				rotation = -rot;
				speed.x += transl;
				speed.y = pos;
				if (speed.x > 1.0f) {
					state = false;
				}
			}
			
	}


	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };


	void Update() {

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		MVPtransf[0][0] = cosf(rotation);
		MVPtransf[0][1] = sinf(rotation);
		MVPtransf[1][0] = -sinf(rotation);
		MVPtransf[1][1] = cosf(rotation);

		MVPtransf[3][0] = speed.x;
		MVPtransf[3][1] = speed.y;

		float radian = 2 * M_PI;
		float vertices[655];
		for (int i = 0; i < 596;  i += 5) {
			vertices[i] = center.x + cosf(i * radian / (float)120) * radius;
			vertices[i + 1] = center.y + sinf(i * radian / (float)120) * radius;
			vertices[i + 2] = color.x;
			vertices[i + 3] = color.y;
			vertices[i + 4] = color.z;
		}

		vertices[600] = -vertices[595];
		vertices[601] = -vertices[596];
		vertices[602] = color.x;
		vertices[603] = color.y;
		vertices[604] = color.z;

		vertices[605] = center.x;
		vertices[606] = center.y;
		vertices[607] = color.x;
		vertices[608] = color.y;
		vertices[609] = color.z;

		vertices[610] = vertices[100];
		vertices[611] = vertices[101];
		vertices[612] = color.x;
		vertices[613] = color.y;
		vertices[614] = color.z;
		
		vertices[615] = -vertices[100];
		vertices[616] = -vertices[101];
		vertices[617] = color.x;
		vertices[618] = color.y;
		vertices[619] = color.z;

		vertices[620] = center.x;
		vertices[621] = center.y;
		vertices[622] = color.x;
		vertices[623] = color.y;
		vertices[624] = color.z;

		vertices[625] = vertices[200];
		vertices[626] = vertices[201];
		vertices[627] = color.x;
		vertices[628] = color.y;
		vertices[629] = color.z;

		vertices[630] = -vertices[200];
		vertices[631] = -vertices[201];
		vertices[632] = color.x;
		vertices[633] = color.y;
		vertices[634] = color.z;
	
		

		glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
			sizeof(vertices),           // # bytes
			vertices,	      	        // address
			GL_STATIC_DRAW);	      	// we do not change later
	}

	void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);  // AttribArray 0
		glVertexAttribPointer(0,       // vbo -> AttribArray 0
			2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), NULL);

		glEnableVertexAttribArray(1);  // AttribArray 0
		glVertexAttribPointer(1,       // vbo -> AttribArray 0
			3, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), (void *)(2 * sizeof(float)));


		glDrawArrays(GGL_STRING, 0 /*startIdx*/, 127/*# Elements*/);
	}
};

class Seet {
public: 
	unsigned int vbo;
	vec2 cbase;
	vec3 color;
	vec2 seettop;
	Circle c;
	
	vec2 speed;
	bool state;

	Seet(){}

	Seet(Circle circle) {
		glGenBuffers(1, &vbo); //generat a buffer 
		c = circle; 
		cbase = c.center;  //printf("%d %d", cbase.x, cbase.y);
		color = circle.color; // printf("%f %f %f ", color.x, color.y, color.z);
		seettop = (cbase.x, cbase.y + circle.radius + 0.03f);
		speed = vec2(0.0f, 0.0f);
		state = false;
	}

	void Animate(float transl, float pos) {
		
		if (state == false) {
			speed.x -= transl;
			speed.y = pos;
			if (speed.x < -1.0f) {
				state = true;
			}
		}

		if (state == true) {
			speed.x += transl;
			speed.y = pos;
			if (speed.x > 1.0f) {
				state = false;
			}
		}

	}

	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };

	void Update() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		MVPtransf[3][0] = speed.x;
		MVPtransf[3][1] = speed.y;

		float vertices[20];

		vertices[0] = cbase.x;
		vertices[1] = cbase.y;
		vertices[2] = color.x;
		vertices[3] = color.y;
		vertices[4] = color.z;

		vertices[5] = cbase.x;
		vertices[6] = cbase.y + c.radius + 0.03f;
		vertices[7] = color.x;
		vertices[8] = color.y;
		vertices[9] = color.z;

		vertices[10] = cbase.x + c.radius / 4.0f;
		vertices[11] = cbase.y + c.radius + 0.03f;
		vertices[12] = color.x;
		vertices[13] = color.y;
		vertices[14] = color.z;

		vertices[15] = cbase.x - c.radius / 4.0f;
		vertices[16] = cbase.y + c.radius + 0.03f;
		vertices[17] = color.x;
		vertices[18] = color.y;
		vertices[19] = color.z;

		glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
			sizeof(vertices),           // # bytes
			vertices,	      	        // address
			GL_STATIC_DRAW);	      	// we do not change later

	}

	void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);  // AttribArray 0
		glVertexAttribPointer(0,       // vbo -> AttribArray 0
			2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), NULL);

		glEnableVertexAttribArray(1);  // AttribArray 0
		glVertexAttribPointer(1,       // vbo -> AttribArray 0
			3, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), (void *)(2 * sizeof(float)));


		glDrawArrays(GGL_STRING, 0 /*startIdx*/, 4/*# Elements*/);
	}

};

class CycleUser {
public:

	unsigned int vbo;
	Seet seet;
	vec3 color;
	float radiusofhead;
	vec2 centerofhead;
	vec2 speed;
	bool state;

	CycleUser(){}

	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };

	CycleUser(Seet s) {
		glGenBuffers(1, &vbo); //generat a buffer 
		seet = s;
		color = vec3(255.0f, 174.0f, 201.0f);
		radiusofhead =  s.c.radius / 2.0f;
		centerofhead = vec2(seet.c.center.x, seet.c.center.y + 2.0f * seet.c.radius + 0.03f + radiusofhead);
		speed = vec2(0.0f, 0.0f);
		state = false; //kezdetben jobbra 
	}

	void Animate(float transl, float pos) {
	
		if (state == false) {
			speed.y = pos;
			speed.x -= transl;
			if (speed.x < -1.0f) {
				state = true;
			}
		}

		if (state == true) {
			speed.y = pos;
			speed.x += transl;
			if (speed.x > 1.0f) {
				state = false;
			}
		}

	}

	void Update() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		MVPtransf[3][0] = speed.x;
		MVPtransf[3][1] = speed.y;

		float radian = 2 * M_PI;
		float vertices[610];

			vertices[0] = seet.c.center.x;
			vertices[1] = seet.c.center.y + seet.c.radius + 0.03f;
			vertices[2] = color.x;
			vertices[3] = color.y;
			vertices[4] = color.z;

			vertices[5] = seet.c.center.x;
			vertices[6] = seet.c.center.y + 2.0f * seet.c.radius + 0.03f;
			vertices[7] = color.x;
			vertices[8] = color.y;
			vertices[9] = color.z;

			for (int i = 10; i < 606; i += 5) {
				vertices[i] = centerofhead.x + cosf(((i+190) * radian) / (float)120) * radiusofhead;
				vertices[i + 1] = centerofhead.y + sinf(((i+190) * radian) / (float)120) * radiusofhead;
				vertices[i + 2] = color.x;
				vertices[i + 3] = color.y;
				vertices[i + 4] = color.z;
			}


			glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
				sizeof(vertices),           // # bytes
				vertices,	      	        // address
				GL_STATIC_DRAW);	      	// we do not change later

	}

	void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);  // AttribArray 0
		glVertexAttribPointer(0,       // vbo -> AttribArray 0
			2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), NULL);

		glEnableVertexAttribArray(1);  // AttribArray 0
		glVertexAttribPointer(1,       // vbo -> AttribArray 0
			3, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), (void *)(2 * sizeof(float)));


		glDrawArrays(GGL_STRING, 0 /*startIdx*/, 122/*# Elements*/);
	}


};

class Leg {

public:
	unsigned int vbo;

	vec2 attachetobody;
	float pedalradius;
	vec3 color;

	vec2 attachetopedal;
	vec2 knee;
	vec2 speed;
	float rotation;
	float leglong;
	bool state;
	float rot;

	Circle circle;
	CycleUser body;

	Leg() {}

	Leg(Circle c, CycleUser cu){
		glGenBuffers(1, &vbo); //generat a buffer 
		circle = c;
		body = cu;
		attachetobody = vec2(c.center.x, c.center.y + c.radius + 0.03f);
		pedalradius = c.radius / 3.0f;
		leglong = 2.0f * c.radius / 3;
		color = body.color;
		attachetopedal = vec2(c.center.x, c.center.y-pedalradius);
		state = false; //jobbra
	}

	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };

	void Animation(float r, float transl, float pos) {
		
		if (state == false) {
			rot = r;
			speed.x -= transl;
			speed.y = pos;
			if (speed.x < -1.0f) {
				state = true;
			}
		}

		if (state == true) {
			rot = -r;
			speed.x += transl;
			speed.y = pos;
			if (speed.x > 1.0f) {
				state = false;
			}
		}

	}

	void Update() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		MVPtransf[3][0] = speed.x;
		MVPtransf[3][1] = speed.y;

		float x = attachetopedal.x;
		float y = attachetopedal.y;

		//attachetopedal.x = x * cosf(rot) - y * sinf(rot);
		//attachetopedal.y = x * sinf(rot) + y * cosf(rot);

		float vertices[25];

		if (state == false) {
			attachetopedal.x = circle.center.x - pedalradius;
			knee.x = attachetobody.x - 0.05f;
			knee.y = (attachetobody.y + (x * sinf(rot) + y * cosf(rot))) / 2;
		}
		if (state == true) {
			attachetopedal.x = circle.center.x + pedalradius;
			knee.x = attachetobody.x +0.05f;
			knee.y = (attachetobody.y + (x * sinf(rot) + y * cosf(rot))) / 2;
		}

		vertices[0] = x * cosf(rot) - y * sinf(rot);
		vertices[1] = x * sinf(rot) + y * cosf(rot);
		vertices[2] = color.x;
		vertices[3] = color.y;
		vertices[4] = color.z;

		vertices[5] = knee.x;
		vertices[6] = knee.y; 
		vertices[7] = color.x;
		vertices[8] = color.y;
		vertices[9] = color.z;

		vertices[10] = attachetobody.x;
		vertices[11] = attachetobody.y;
		vertices[12] = color.x;
		vertices[13] = color.y;
		vertices[14] = color.z;


		glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
			sizeof(vertices),           // # bytes
			vertices,	      	        // address
			GL_STATIC_DRAW);	      	// we do not change later
	}

	void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);  // AttribArray 0
		glVertexAttribPointer(0,       // vbo -> AttribArray 0
			2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), NULL);

		glEnableVertexAttribArray(1);  // AttribArray 0
		glVertexAttribPointer(1,       // vbo -> AttribArray 0
			3, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
			5 * sizeof(float), (void *)(2 * sizeof(float)));


		glDrawArrays(GGL_STRING, 0 /*startIdx*/, 3/*# Elements*/);
	}

};

class LineStrip {
public:
	unsigned int vbo;
	std::vector<vec2> controlpoints;
	std::vector<vec2> linepoints;

	float p0, p1, p2, p3;
	float tension;
	float bias;
	float continuity;

	vec3 color;

	std::vector<float> vertices;



	LineStrip() {}

	LineStrip(std::vector<vec2> ctrlps, float _tension, float _bias = 0, float _continuity = 0) {
		tension = _tension;
		bias = _bias;
		continuity = _continuity;
		controlpoints = ctrlps;
		color = vec3(0.0f, 1.0f, 0.0f);

		
		//AddPoint(1.3f, 0.5f);
	}

		vec2 GetLinePoint(float t,
		vec2 p0,
		vec2 p1,
		vec2 p2,
		vec2 p3
	) {
		

		float tt = t * t;
		float ttt = tt * t;

		float q1 = 2.0f * ttt - 3.0f*tt + 1.0f ;
		float q2 = ttt - 2.0f*tt + t;
		float q3 = -2.0f*ttt + 3.0f*tt;
		float q4 = ttt - tt;

		
		vec2 d0 = (p1 - p0) * ((1.0f - tension)*(1.0f + bias)*(1.0f + continuity)) + (p2 - p1) * ((1.0f - tension)*(1.0f - bias)*(1.0f - continuity));
		vec2 d1 = (p2 - p1) * ((1.0f - tension)*(1.0f + bias)*(1.0f + continuity)) + (p3 - p2) * ((1.0f - tension)*(1.0f - bias)*(1.0f - continuity));

		vec2  res = p1 * q1 + d0 * q2 + p2 * q3 + d1 * q4;
		
		return(res);
	}

		void GetPointsOfLine() {
			linepoints.clear();
			for (int i = 1; i < controlpoints.size()-2; i++) {
				float s =1.0f / 10000.0f ;
				for (float j = 0; j < 1.0f ; j += s) {
					vec2 p = GetLinePoint(j, controlpoints[i - 1], controlpoints[i], controlpoints[i + 1], controlpoints[i + 2]);
					linepoints.push_back(p);
				}
			}
		}

		std::vector<vec2> makeline() {
			GetPointsOfLine();
			return linepoints;
		}

		bool greaterthan(vec2 first, vec2 second) {
			if (first.x > second.x) {
				return true;
			}
			else {
				return false;
			}
		}

		int numberofpoints() {
			return linepoints.size();
		}

		void sort() {
			for (int i = controlpoints.size()-1; i > 0 ; --i) {
				for (int j = 0; j < i; j++) {
					if (greaterthan(controlpoints[j], controlpoints[j + 1])) {
						vec2 temp = controlpoints[j];
						controlpoints[j] = controlpoints[j + 1];
						controlpoints[j + 1] = temp;
					}
				}
			}
		}

		void AddPoint(float cX, float cY) {
			controlpoints.push_back(vec2(cX, cY));
			sort();
		}

		void Update() {
			
			std::vector<vec2> tmp;
			GetPointsOfLine();
			tmp = linepoints;
			
			vertices.clear();

			for (int i = 0; i < tmp.size(); i++) {
				vertices.push_back(tmp[i].x);
				vertices.push_back(tmp[i].y);
				vertices.push_back(color.x);
				vertices.push_back(color.y);
				vertices.push_back(color.z);
				//printf("\n %f   %f", tmp[i].x, tmp[i].y);
			}

			
		}
			

		void Draw() {

			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
				vertices.size() * sizeof(float),           // # bytes
				&vertices[0],	      	        // address
				GL_STATIC_DRAW);	      	// we do not change later
			
			glEnableVertexAttribArray(0);  // AttribArray 0
			glVertexAttribPointer(0,       // vbo -> AttribArray 0
				2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
				5 * sizeof(float), NULL);

			glEnableVertexAttribArray(1);  // AttribArray 0
			glVertexAttribPointer(1,       // vbo -> AttribArray 0
				3, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
				5 * sizeof(float), (void *)(2 * sizeof(float)));

			glDrawArrays(GGL_STRING, 0 /*startIdx*/, linepoints.size()  /*# Elements*/);

		}
	};

	GPUProgram gpuProgram; // vertex and fragment shaders
	unsigned int vao;	   // virtual world on the GPU

	Circle ccc;
	LineStrip path;
	LineStrip mountain;
	Seet seet;
	CycleUser bela;
	Leg belalegs;

	float diff;


	// Initialization, create an OpenGLcontext
	void onInitialization() {
		glViewport(0, 0, windowWidth, windowHeight);
		glLineWidth(1.5);

		glGenVertexArrays(1, &vao);	// get 1 vao id
		glBindVertexArray(vao);		// make it active

		
	
		//kerék
		ccc = Circle(0.08f, vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f));

		//ülés
		seet = Seet(ccc);

		//ember
		bela = CycleUser(seet);

		//laba
		belalegs = Leg(ccc, bela);
		
		path = LineStrip({ vec2(-2.0f, 0.0f), vec2(-1.5f, -0.8f), vec2(1.7f, -0.6f), vec2(2.0f, 0.0f) }, -0.5f);
		path.Update();

		mountain = LineStrip({ vec2(-2.0f, 0.32f), vec2(-1.5f, -0.48f), vec2(1.7f, -0.28f), vec2(2.0f, 0.32f) }, 0.5f);
		mountain.Update();

		// create program for the GPU
		gpuProgram.Create(vertexSource, fragmentSource, "outColor");
	}

	// Window has become invalid: Redraw
	void onDisplay() {
		glClearColor(0, 0, 0, 0);     // background color
		glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer

		 // Set color to (0, 1, 0) = green
		int location = glGetUniformLocation(gpuProgram.getId(), "color");
		
		float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
								  0, 1, 0, 0,    // row-major!
								  0, 0, 1, 0,
								  0, 0, 0, 1 };

		location = glGetUniformLocation(gpuProgram.getId(), "MVP");
		glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);

		mountain.Draw();

		location = glGetUniformLocation(gpuProgram.getId(), "MVP");
		glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);

		path.Draw();
		

		location = glGetUniformLocation(gpuProgram.getId(), "MVP"); // Get the GPU location of uniform variable MVP
		glUniformMatrix4fv(location, 1, GL_TRUE, &ccc.MVPtransf[0][0]);	// Load a 4x4 row-major float matrix to the specified location

		ccc.Update();
		ccc.Draw();
		

		location = glGetUniformLocation(gpuProgram.getId(), "MVP"); 
		glUniformMatrix4fv(location, 1, GL_TRUE, &seet.MVPtransf[0][0]);

		seet.Update();
		seet.Draw();


		location = glGetUniformLocation(gpuProgram.getId(), "MVP");
		glUniformMatrix4fv(location, 1, GL_TRUE, &bela.MVPtransf[0][0]);

		bela.Update();
		bela.Draw();

		location = glGetUniformLocation(gpuProgram.getId(), "MVP");
		glUniformMatrix4fv(location, 1, GL_TRUE, &belalegs.MVPtransf[0][0]);

		belalegs.Update();
		belalegs.Draw();

		

		glutSwapBuffers(); // exchange buffers for double buffering
	}

	// Key of ASCII code pressed
	void onKeyboard(unsigned char key, int pX, int pY) {
		if (key == 'd') glutPostRedisplay();         // if d, invalidate display, i.e. redraw
	}

	// Key of ASCII code released
	void onKeyboardUp(unsigned char key, int pX, int pY) {
	}

	// Move mouse with key pressed
	void onMouseMotion(int pX, int pY) {	// pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
		// Convert to normalized device space
		float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
		float cY = 1.0f - 2.0f * pY / windowHeight;
		printf("Mouse moved to (%3.2f, %3.2f)\n", cX, cY);
	}

	// Mouse click event
	void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
		// Convert to normalized device space
		float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
		float cY = 1.0f - 2.0f * pY / windowHeight;

		char * buttonStat;
		switch (state) {
		case GLUT_DOWN: buttonStat = "pressed"; break;
		case GLUT_UP:   buttonStat = "released"; break;
		}

		switch (button) {
		case GLUT_LEFT_BUTTON:

			//printf("Left button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
			if (buttonStat == "pressed") {
				path.AddPoint(cX, cY);
				path.Update();

				mountain.AddPoint(cX, cY + 0.32f);
				mountain.Update();
				glutPostRedisplay();
			}
			break;

		case GLUT_MIDDLE_BUTTON: printf("Middle button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY); break;
		case GLUT_RIGHT_BUTTON:  printf("Right button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);  break;
		}
	}

	// Idle event indicating that some time elapsed: do animation here
	void onIdle() {
		long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
		float sec = time / 250.0f;  

		monocyclepos = vec2( ccc.center.x + ccc.speed.x,  ccc.center.y + ccc.speed.y - ccc.radius );
		for (int i = 0; i < path.linepoints.size(); i++) {
			if (monocyclepos.x < path.linepoints[i].x + 0.01f && monocyclepos.x > path.linepoints[i].x - 0.01f) {
				diff = (-1.0f * (monocyclepos.y - path.linepoints[i].y)) ;
			}
		}
		
		//printf("\n %f ", diff);


		//ide a dolgok
		ccc.Animate(sec, 0.0002f, diff);
		seet.Animate(0.0002f, diff);
		bela.Animate(0.0002f, diff);
		belalegs.Animation(sec, 0.0002f, diff);

		glutPostRedisplay();
	}
