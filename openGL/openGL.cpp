#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <gl/GLAUX.H>
//#include "GL/bmp.h"
#pragma comment (lib, "glaux.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")

//각종 변수 선언
double vcs_L = -1, vcs_R = 1, vcs_B = -1, vcs_T = 1, vcs_N = 1, vcs_F = 30;
double dis_x, dis_z;
int theta;
float angle = 0.0;
float move = 0.1;
float lx = 0.0f, lz = -1.0f, ly = 0.0;
float x = 0.0f, z = 2.0f;
GLuint img[1];
float msize = 0.6f;

//이미지 파일을 불러오기위한 함수입니다. 
AUX_RGBImageRec* LoadBMP(char* Filename)    
{
	FILE* File = NULL;        
	if (!Filename)          
	{
		return NULL;
	}
	File = fopen(Filename, "r");      
	if (File)          
	{
		printf("apple을 불러옴");
		fclose(File);        
		return auxDIBImageLoadA((const char*)Filename);    
	}
	return NULL;          
}

//면을 생성 해주는 함수입니다.
GLfloat vertices[8][3] = {
	{ -0.5, -0.5,  0.5 },
	{ -0.5,  0.5, 0.5 },
	{ 0.5,  0.5,  0.5 },
	{ 0.5, -0.5,  0.5 },
	{ -0.5, -0.5, -0.5 },
	{ -0.5,  0.5, -0.5 },
	{ 0.5,  0.5, -0.5 },
	{ 0.5, -0.5, -0.5 }
};

//맵핑을 해주는 함수입니다. 텍스쳐 맵핑도 함께 해줍니다.
void mapping(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glColor3f(255.0, 255.0, 255.0);

	glBindTexture(GL_TEXTURE_2D, img[0]);
	glBegin(GL_QUADS);

	//윗면
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.6f, 0.6f, -0.6f); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.6f, 0.6f, 0.6f);  
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.6f, 0.6f, 0.6f);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.6f, 0.6f, -0.6f);  

	//아랫면
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.6f, -0.6f, -0.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.6f, -0.6f, -0.6f); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.6f, -0.6f, 0.6f);  
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.6f, -0.6f, 0.6f); 

	//정면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.6f, -0.6f, 0.6f); 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.6f, -0.6f, 0.6f);  
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.6f, 0.6f, 0.6f);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.6f, 0.6f, 0.6f);  

	//오른면
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.6f, -0.6f, -0.6f); 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.6f, 0.6f, -0.6f);  
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.6f, 0.6f, 0.6f);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.6f, -0.6f, 0.6f);  

	//뒷면
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.6f, -0.6f, -0.6f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.6f, 0.6f, -0.6f); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.6f, 0.6f, -0.6f);  
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.6f, -0.6f, -0.6f); 

	//왼면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.6f, -0.6f, -0.6f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.6f, -0.6f, 0.6f); 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.6f, 0.6f, 0.6f);  
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.6f, 0.6f, -0.6f); 
									 
	glEnd();
}

//건물의 형태를 위해 면을 배치 해줍니다.
void building()
{
	mapping(0, 3, 2, 1);
	mapping(2, 3, 7, 6);
	mapping(3, 0, 4, 7);
	mapping(4, 5, 6, 7);
	mapping(1, 2, 6, 5);
	mapping(5, 4, 0, 1);
}

//명수당 그려주는 함수입니다.
void Lake(float cx, float cz, float r)
{
	float x1, z1, x2, z2;
	float angle;
	double radius = r;
	x1 = cx, z1 = cz;
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x1, 0.0, z1);
	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		z2 = z1 + cos(angle) * radius;
		glVertex3f(x2, 0.0, z2);
	}
	glEnd();
} 

//조명을 생성해주는 함수입니다.
void Light()
{
	GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 45.0 };
	GLfloat light_specular[] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.0, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.0, 0.3, 1.0 };
	GLfloat light_position[] = { -30, 600, 30.0, 1.0 };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

//이미지를 불러와 텍스쳐 맵핑을 해주는 함수입니다.
void Texture()
{
	glShadeModel(GL_SMOOTH);       			   
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(2, img);

	AUX_RGBImageRec* TextureImage[2];    

	memset(TextureImage, 0, sizeof(void*) * 2);           

	TextureImage[0] = LoadBMP((char*)"apple.bmp");
	glGenTextures(2, img);    							
	glBindTexture(GL_TEXTURE_2D, img[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
}

//도로와 건물 그리기는 함수입니다.
void Draw()
{
	//도로1
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.7f, 0.5f);
	glVertex3f(-1.0, 0.0, 2.5);
	glVertex3f(1.0, 0.0, 2.5);
	glVertex3f(1.0, 0.0, -12.0);
	glVertex3f(-1.0, 0.0, -12.0);
	glEnd();

	//도로2
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.7f, 0.5f);
	glVertex3f(0.5, 0.0, 1.5);
	glVertex3f(3.0, 0.0, 1.5);
	glVertex3f(3.0, 0.0, 2.0);
	glVertex3f(0.5, 0.0, 2.0);
	glEnd();

	//도로3
	glBegin(GL_POLYGON);
	glColor3f(0.3f, 0.7f, 0.5f);
	glVertex3f(-0.5, 0.0, -1.5);
	glVertex3f(-3.0, 0.0, -1.5);
	glVertex3f(-3.0, 0.0, -2.0);
	glVertex3f(-0.5, 0.0, -2.0);
	glEnd();

	//명수당
	Lake(-3.0, -9.0, 1.0);

	//공대
	glTranslated(-2.0, 0.0, -5.5);
	building();

	//자대
	glTranslated(-3.0, 0.0, 2.0);
	building();

	//도서관
	glTranslated(6.8, 0.0, 0);
	building();

	//기숙사
	glTranslated(0.8, 0.0, 3.5);
	building();

	//기숙사
	glTranslated(0.0, 0.3, -3.0);
	building();

	glEnd();
	glFlush();
}

void MyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(vcs_L, vcs_R, vcs_B, vcs_T, vcs_N, vcs_F);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, 3.0, z, x + lx, 3.0 + ly, z + lz, 0.0, 1.0, 0.0);
	glTranslatef(0, 0, -2);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);

	Draw();

	glEnd();

	glFlush();
}

//움직임 및 시선을 구현한 함수입니다.
void movement(unsigned char KeyPressed, int X, int Y)
{
	switch (KeyPressed)
	{

		//앞으로 이동
	case 'S':
	case 's':
		x += lx * move;
		z += lz * move;
		break;
		//뒤로 이동
	case 'D':
	case 'd':
		x -= lx * move;
		z -= lz * move;
		break;
		//오른쪽으로 회전
	case 'R':
	case 'r':
		angle += 0.1;
		lx = sin(angle);
		lz = -cos(angle);
		break;
		//왼쪽으로 회전
	case 'L':
	case 'l':
		angle -= 0.1;
		lx = sin(angle);
		lz = -cos(angle);
		break;
		//시야 상승
	case 'U':
	case 'u':
		angle += 0.01;
		ly = sin(angle);
		lz = -cos(angle);
		break;
		//시야 하강
	case 'B':
	case 'b':
		angle -= 0.01;
		ly = sin(angle);
		lz = -cos(angle);
		break;
	}
	gluLookAt(x, 3.0, z, x + lx, 3.0 + ly, z + lz, 0.0, 1.0, 0.0);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	printf("main");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("navigation");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	Texture();
	Light();
	glutKeyboardFunc(movement);
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}
