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

class circle{
	unsigned int vbo;
	float radius;
	vec2 center;
	vec3 color;

public:
	circle() {};

	circle(float r, vec2 c,vec3 col) {
		radius = r;
		center = c; 
		color = col;
		glGenBuffers(1, &vbo); //generat a buffer 
	}

	void setColor(vec3 col) {
		color = col;
	}

	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };


	void Update() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		float radian = 2 * M_PI;
		float vertices[600];
		for (int i = 0; i < 600;  i += 5) {
			vertices[i] = center.x + cosf(i * radian / 120) * radius;
			vertices[i + 1] = center.y + sinf(i * radian / 120) * radius;
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


		glDrawArrays(GGL_STRING, 0 /*startIdx*/, 120/*# Elements*/);
	}
};

class cycle {

	circle circle;
	vec3 color;

	float rotation; 
	float speed;

public: 

	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };

	cycle() {};

	
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

circle ccc;
LineStrip path;

// Initialization, create an OpenGLcontext
void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);

	glGenVertexArrays(1, &vao);	// get 1 vao id
	glBindVertexArray(vao);		// make it active

	/*unsigned int vbo;		// vertex buffer object
	glGenBuffers(1, &vbo);	// Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Geometry with 24 bytes (6 floats or 3 x 2 coordinates)
	float vertices[] = { -0.8f, -0.8f, -0.6f, 1.0f, 0.8f, -0.2f };
	glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
		sizeof(vertices),  // # bytes
		vertices,	      	// address
		GL_STATIC_DRAW);	// we do not change later

	glEnableVertexAttribArray(0);  // AttribArray 0
	glVertexAttribPointer(0,       // vbo -> AttribArray 0
		2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
		0, NULL); 		     // stride, offset: tightly packed             */

	ccc = circle(0.08f, vec2(0, 0), vec3(1.0f, 0.0f, 0.0f));
	


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
	glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);	// Load a 4x4 row-major float matrix to the specified location
	
	ccc.Update();
	ccc.Draw();

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
}
