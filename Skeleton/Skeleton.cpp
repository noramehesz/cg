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

	void Animate(float rot, float transl) {

			if (state == false ) {
				rotation = rot;
				speed.x -= transl;
				if (speed.x < -1.0f) {
					state = true;
				}
			}

			if(state == true){
				rotation = -rot;
				speed.x += transl;
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

	void Animate(float transl) {

		if (state == false) {
			speed.x -= transl;
			if (speed.x < -1.0f) {
				state = true;
			}
		}

		if (state == true) {
			speed.x += transl;
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

	void Animate(float transl) {

		if (state == false) {
			speed.x -= transl;
			if (speed.x < -1.0f) {
				state = true;
			}
		}

		if (state == true) {
			speed.x += transl;
			if (speed.x > 1.0f) {
				state = false;
			}
		}

	}

	void Update() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		MVPtransf[3][0] = speed.x;

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
	float speed;
	float rotation;
	float leglong;

	Circle circle;
	CycleUser body;

	Leg() {}

	Leg(Circle c, CycleUser cu){
		circle = c;
		body = cu;
		attachetobody = body.seet.seettop;
		pedalradius = c.radius / 2.0f;
		leglong = 2.0f * c.radius / 3;
		color = body.color;
	}

	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };

	void Update() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		float vertices[25];
/*
		vertices[0] = attachetobody.x;
		vertices[1] = attachetobody.x;
		vertices[2] = color.x;
		vertices[3] = color.y;
		vertices[4] = color.z;

			vertices[5] = 
			vertices[6]
			vertices[7]
			vertices[8]
			vertices[9]


			vertices[10]
			vertices[11]
			vertices[12]
			vertices[13]
			vertices[14]


			vertices[15]
			vertices[16]
			vertices[17]
			vertices[18]
			vertices[19]

*/
	}

};

	class LineStrip {
		unsigned int vao;
		unsigned int vbo;
		std::vector<float> vertexData;
		float tension;

	public:
		void AddPoint(float cX, float cY) {
			vertexData.push_back(cX);
			vertexData.push_back(cY);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_DYNAMIC_DRAW);
		}
	};

	GPUProgram gpuProgram; // vertex and fragment shaders
	unsigned int vao;	   // virtual world on the GPU

	Circle ccc;
	LineStrip path;
	Seet seet;
	CycleUser bela;

	// Initialization, create an OpenGLcontext
	void onInitialization() {
		glViewport(0, 0, windowWidth, windowHeight);

		glGenVertexArrays(1, &vao);	// get 1 vao id
		glBindVertexArray(vao);		// make it active

	
		//kerék
		ccc = Circle(0.14f, vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f));

		//ülés
		seet = Seet(ccc);

		//ember
		bela = CycleUser(seet);
		


		// create program for the GPU
		gpuProgram.Create(vertexSource, fragmentSource, "outColor");
	}

	// Window has become invalid: Redraw
	void onDisplay() {
		glClearColor(0, 0, 0, 0);     // background color
		glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer


		 // Set color to (0, 1, 0) = green
		int location = glGetUniformLocation(gpuProgram.getId(), "color");
		//glUniform3f(location, 0.0f, 1.0f, 0.0f); // 3 floats

		float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
								  0, 1, 0, 0,    // row-major!
								  0, 0, 1, 0,
							  0, 0, 0, 1 };

		//location = glGetUniformLocation(gpuProgram.getId(), "MVP");	// Get the GPU location of uniform variable MVP
		//glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);	// Load a 4x4 row-major float matrix to the specified location

		//glBindVertexArray(vao);  // Draw call
		//glDrawArrays(GL_TRIANGLES, 0 /*startIdx*/, 3 /*# Elements*/);

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
		float sec = time / 350.0f;  //ez lesz a forgás sebessége? 

		//ide a dolgok
		ccc.Animate(sec, 0.0003f);
		seet.Animate(0.0003f);
		bela.Animate(0.0003f);

		glutPostRedisplay();
	}
