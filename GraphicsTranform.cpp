#include <GL/glut.h>  
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <cmath>
using namespace std;
const GLdouble pi = 3.14159265253;
//color 类无用 属于过度设计
//class color {
//public:
//	int r,g,b;
//	GLfloat alpha;
//	color(){};
//	color(int r, int g, int b,int alpha):r(r),g(g),b(b),alpha(alpha) {}
//}colors[255];

//opengl 窗口属性
class  propertys {
public :
	GLsizei winWidth,winHeight;
	GLfloat xwcMin,xwcMax,ywcMin,ywcMax; 
	propertys(){};
	propertys(GLsizei x, GLsizei y, GLfloat z, GLfloat u, GLfloat v, GLfloat w):winWidth(x),winHeight(y),xwcMin(z),xwcMax(u),ywcMin(v),ywcMax(w) { }
}p;

//点
class point2D {
public:
	GLfloat x,y;
	point2D(){};
	point2D(GLfloat x,GLfloat y):x(x),y(y){}
};
//矩阵
class Matrix3x3 {
public:
	GLfloat data[3][3];
	//构造单位矩阵
	Matrix3x3() {
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				if(i==j) data[i][j] = 1;
				else data[i][j] = 0;
			}
		}
	}
	void Multiply(Matrix3x3 a) {
		Matrix3x3 temp;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				if(data[i][j]) {
					for(int k = 0;k < 3; k++) {
						temp.data[i][k]+=data[i][j]*a.data[j][k];
					}
				}
			}
		}
	}
};
//变换类
class TransFrom{
public:
	//平移
	Matrix3x3 translate2D(Matrix3x3 t,GLfloat tx,GLfloat ty) {
		Matrix3x3 matTransl;
		matTransl.data[0][2] = tx;
		matTransl.data[1][2] = ty;
		matTransl.Multiply(t);
		return matTransl;
	}
	Matrix3x3 rotate2D(Matrix3x3 t,point2D pivotPt,GLfloat theta) {
		Matrix3x3 matRot;
		matRot.data[0][0] = cos(theta);
		matRot.data[0][1] = -sin(theta);
		matRot.data[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);
		matRot.data[1][0] = sin(theta);
		matRot.data[1][1] = cos(theta);
		matRot.data[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);
		matRot.Multiply(t);
		return matRot;
	}
	Matrix3x3 scale2D(Matrix3x3 t,GLfloat sy,GLfloat sx,point2D fixedPt) {
		Matrix3x3 matScale;
		matScale.data[0][0] = sx;
		matScale.data[1][1] = sy;
		matScale.data[0][2] = (1-sx) * fixedPt.x;
		matScale.data[1][2] = (1-sy) * fixedPt.y;
		matScale.Multiply(t);
		return matScale;
	}
	//计算坐标
	void calCoord(point2D *verts,GLint num,Matrix3x3 t){
		GLfloat temp;
		for(int i = 0; i < num; i++) {
			temp = t.data[0][0] * verts[i].x+ t.data[0][1] * verts[i].y + t.data[0][2];
			verts[i].y = t.data[1][0] * verts[i].x + t.data[1][1] * verts[i].y + t.data[1][2];
			verts[i].x = temp;
		}
	}
};
//三角形图形
class Triangle {
public:
	point2D centriodPt;
	point2D verts[3];
	Triangle() {
		for(int i=0;i<3;i++) {
			verts[i].x = 0;
			verts[i].y = 0;
		}
	}
	Triangle(point2D *v) {
		for(int i = 0; i < 3; i++) {
			verts[i]=point2D(v[i].x,v[i].y);
		}
	}
	point2D getCentroidPt() {
		GLint xSum = 0,ySum = 0;
		for(int i = 0; i < 3; i++) {
			xSum += verts[i].x;
			ySum += verts[i].y;
		}
		GLfloat tempX = (GLfloat)xSum / (GLfloat) 3;
		GLfloat tempY = (GLfloat)ySum / (GLfloat) 3;
		return point2D(tempX,tempY);
	}
	void DrawTriangle() {
		GLint k;
		glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++) {
			glVertex3f(verts[k].x,verts[k].y,0);
		/*cout<<verts[k].x<<" "<<verts[k].y<<endl;
		cout<<"done"<<endl;*/
		}
		glEnd();
	}
};


//void display() {
//	//定义三角形初始位置
//	point2D v[3]={point2D(0,0),point2D(50,0),point2D(0,100)};
//	Triangle t1 = Triangle(v);
	//point2D centroidPt = t1.getCentroidPt();
	//point2D pivPt = point2D(0,0);
	//point2D fixedPt = centroidPt;
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(0.0,0.0,1.0);
	//t1.DrawTriangle();
	////旋转参数
	//GLdouble theta = pi / 6;
	////平移参数
	//GLfloat tx = 7.0, ty = 0.0;
	//TransFrom todo;
	//Matrix3x3 t;
	//t = todo.rotate2D(t,fixedPt,theta);
	//t = todo.translate2D(t,tx,ty);
	//todo.calCoord(v,3,t);
	//glColor3f(1.0,0.0,0.0);
	//Triangle t2 = Triangle(v);
	//t2.DrawTriangle();
	//glFlush();
//}
void winReshapeFun(GLint newWidth,GLint newHeight) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(p.xwcMin,p.xwcMax,p.ywcMin,p.ywcMax);
	glClear(GL_COLOR_BUFFER_BIT);
}

//方案1
void displayFun1() {
	GLint nVerts = 3;
	point2D v[3]={point2D(50.0,25.0),point2D(150.0,25.0),point2D(100.0,100.0)};
	Triangle origin = Triangle(v);
	/*for(int i=0;i<3;i++) {
		cout<<origin.verts[i].x<<" "<<origin.verts[i].y<<endl;
	}*/
	//三角形中心位置 没用	
	point2D centroidPt = origin.getCentroidPt();
	point2D pivotPt = point2D(0,0),fixedPt = centroidPt;
	GLfloat tx = 47.0,ty = 0.0;
	GLdouble theta = pi/6;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,1.0);
	origin.DrawTriangle();
	//变换
	Matrix3x3 t1;
	Matrix3x3 t2;
	TransFrom todo;
	t1 = todo.rotate2D(t1,fixedPt,theta);
	todo.calCoord(v,3,t1);
	t2 = todo.translate2D(t1,tx,ty);
	todo.calCoord(v,3,t2);
	Triangle second = Triangle(v);
	glColor3f(1.0,0.0,0.0);
	second.DrawTriangle();
	glFlush();
}
//方案2（在主函数中修改即可调用 两个方案在当前参数下差别不大）
void displayFun2() {
	GLint nVerts = 3;
	point2D v[3]={point2D(50.0,25.0),point2D(150.0,25.0),point2D(100.0,100.0)};
	Triangle origin = Triangle(v);
	/*for(int i=0;i<3;i++) {
		cout<<origin.verts[i].x<<" "<<origin.verts[i].y<<endl;
	}*/
	//三角形中心位置 没用	
	point2D centroidPt = origin.getCentroidPt();
	point2D pivotPt = point2D(0,0),fixedPt = centroidPt;
	GLfloat tx = 47.0,ty = 0.0;
	GLdouble theta = pi/6;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,1.0);
	origin.DrawTriangle();
	//变换
	Matrix3x3 t1;
	Matrix3x3 t2;
	TransFrom todo;
	t1 = todo.translate2D(t1,tx,ty);
	todo.calCoord(v,3,t1);
	t2 = todo.rotate2D(t1,fixedPt,theta);
	todo.calCoord(v,3,t2);
	Triangle second = Triangle(v);
	glColor3f(1.0,0.0,0.0);
	second.DrawTriangle();
	glFlush();
}

void init() {
	glClearColor(1,1,1,0);
}
int main(int argc,char ** argv) {
	glutInit(&argc,argv);
	int n;
	cout<<"请选择变换种类(只可以选一次)"<<endl;
	cout<<"1(旋转--平移)"<<endl;
	cout<<"2(平移--旋转)"<<endl;
	cout<<"0(退出)"<<endl;
	while(cin>>n&&n) {
		glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
		glutInitWindowPosition(150,150);
		glutCreateWindow("固定坐标系模式的变换");
		init();
		p = propertys(600,600,0.0,225.0,0.0,225.0);
		glutInitWindowSize(p.winWidth,p.winHeight);
		if(n==1) {
			glutDisplayFunc(displayFun1);
		} else if(n==2) {
			glutDisplayFunc(displayFun2);
		} else {
			return 0;
		}
		glutReshapeFunc(winReshapeFun);
		glutMainLoop();
		if(getchar()) continue; 
	}
	return 0;
}