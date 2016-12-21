#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int sun_rotation;
static int earth_rotation, earth_revolution, moon_rotation, moon_revolution;
static int venus_rotation, venus_revolution;

static GLfloat eyex = 8.0, eyey, eyez; 
//specify the position of the eye point and are mapped to the origin
static GLfloat atx, aty, atz; 
//specify a point being looked at, which will be rendered in the centre 
//of the view port. It is mapped to -z axis
static GLfloat upx = 0.0, upy =0.25, upz = 1.0; 
//specify components of the camera up vector
static GLfloat camera_step = 0.25; 
//定义每次camera的delta变化(包括前进后退) 都为0.5 

void Init(void);
void Display(void);
void SolarDisplay(void);
void Reshape(GLsizei w, GLsizei h);
void Keyboard(unsigned char key, int x, int y);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); //初始化GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //设置初始显示模式
	glutInitWindowSize(640, 480); //设置窗口大小
	glutInitWindowPosition(100, 100); //设置初始窗口位置
	glutCreateWindow("Solar System"); //命名顶层窗口名字为Solar System
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(SolarDisplay);
	glutMainLoop();
	return 0;
}

void Init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT); //设置为GL_FLAT模式
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //BUFFER设置为颜色可写
	glColor3f(1.0, 0.5, 0.2); //setting the color to red

	//Drawing the Sun
	glPushMatrix(); //将当前矩阵压栈
	glRotatef((GLfloat)sun_rotation, 0.0, -1.0, 2.0); //.....
	glRotatef(30.0, 1.0, 0.0, 0.0); // angle of the sun's axis  
	glutWireSphere(1.0, 20, 16); //drawing a solid sphere as the sun
	glPopMatrix(); //恢复先前压栈的矩阵

	//Drawing the Earth
	glPushMatrix(); //将当前矩阵压栈
	glColor3f(0.0, 0.0, 1.0); //setting color to blue
	glRotatef((GLfloat)earth_revolution, 0.0, 0.0, 1.0); //.....
	glTranslatef(4.0, 0.0, 0.0); //distance between earth and sun  
	glPushMatrix(); //将当前矩阵压栈
	glRotatef((GLfloat)earth_rotation, 0.0, -1.0, 2.0); //.....
	glRotatef(30, 1.0, 0.0, 0.0); //angle of the earth's axis  
	glutSolidSphere(0.2, 10, 8); //drawing a solid sphere as the earth
	glPopMatrix(); //恢复先前压栈的矩阵

	//Drawing the Moon
	glColor3f(1.0, 1.0, 1.0); //setting color to white
	glRotatef((GLfloat)moon_revolution, 0.0, 1.0, 1.0); //.....
	glTranslatef(0.5, 0.0, 0.0); //distance between earth and moon  
	glutSolidSphere(0.05, 5, 4); //drawing a solid sphere as the moon
	glPopMatrix(); //恢复先前压栈的矩阵
	glPopMatrix(); //恢复先前压栈的矩阵

	//Drawing the Venus
	glPushMatrix(); //将当前矩阵压栈
	glColor3f(1.0, 1.0, 0.2); //setting the color to yellow
	glRotatef((GLfloat)venus_revolution, 0.0, 0.0, 1.0); //.....
	glTranslatef(3.0, 0, 0); //distnace between venus and sun
	glRotatef((GLfloat)venus_rotation, 0.0, 0.0, 1.0); //.....
	glutSolidSphere(0.2, 9, 7); //drawing a solid sphere as the venus
	glPopMatrix(); //恢复先前压栈的矩阵
	glutSwapBuffers();
}

void SolarDisplay(void)
{
	//All rotation & revolution equations/calculations
	sun_rotation = (sun_rotation + 1) % 360;
	earth_rotation = (earth_rotation + 8) % 360;
	earth_revolution = (earth_revolution + 4) % 360;
	moon_revolution = (moon_revolution + 9) % 360;
	venus_revolution = (venus_revolution - 4) % 360;
	venus_rotation = (venus_rotation + 8) % 360;
	glutPostRedisplay(); //标记当前窗口需要重新绘制
	Sleep(30); //控制整体星系的旋转速度
}

void Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h); //设置视口
	glMatrixMode(GL_PROJECTION); //指明当前矩阵为投影矩阵
	glLoadIdentity(); //将当前矩阵置换为单位矩阵
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); //生成对称透视型视景体
	glMatrixMode(GL_MODELVIEW); //指明当前矩阵为模型视图矩阵
	glLoadIdentity(); //将当前矩阵置换为单位矩阵

	gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz); 
	//specify positions of the eye point & view port & up vector
}

//透过键盘keyboard来实现用户user交互设计
void Keyboard(unsigned char key, int x, int y)
{
	//以下提供4种不同按键交互模式
	switch (key)
	{
	case 'w':
		eyex -= camera_step; //将当前eyex值减去0.25；实际视觉效果是将视角拉近，距离拉近物体自然就变大
		glLoadIdentity(); //将当前矩阵置换为单位矩阵
		gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz); //eye point & view port & up vector adjust
		glutPostRedisplay(); //标记当前窗口需要重新绘制
		break; //跳出switch case语句
	case 's':
		eyex += camera_step; //将当前eyex值加上0.25；实际视觉效果是将视角拉远，距离拉远物体自然就变小
		glLoadIdentity(); //将当前矩阵置换为单位矩阵
		gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz); //eye point & view port & up vector adjust
		glutPostRedisplay(); //标记当前窗口需要重新绘制
		break; //跳出switch case语句
	case 'a':
		aty -= camera_step; //将当前aty值减去0.25；实际视觉效果是物体绕着轴向右旋转，整个星系由视窗中心逐渐向右移动最后移动到视窗外
		glLoadIdentity(); //将当前矩阵置换为单位矩阵
		gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz); //eye point & view port & up vector adjust
		glutPostRedisplay(); //标记当前窗口需要重新绘制
		break; //跳出switch case语句
	case 'd':
		aty += camera_step; //将当前aty值加上0.25；实际视觉效果是物体绕着轴向左旋转，整个星系由视窗中心逐渐向左移动最后移动到视窗外
		glLoadIdentity(); //将当前矩阵置换为单位矩阵
		gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz); //eye point & view port & up vector adjust
		glutPostRedisplay(); //标记当前窗口需要重新绘制
		break; //跳出switch case语句
	case 27:
		exit(0); //当user按下Esc键后，程序结束
		break;
	default:
		break;
	}
}

