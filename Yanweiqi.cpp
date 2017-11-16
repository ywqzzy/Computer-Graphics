#include <iostream>
#include <math.h>
#include <vector>

class YWQVector3 {
private:
	float x,y,z;
public:
	YWQVector3():x(0),y(0),z(0){};
	YWQVector3(float x,float y,float z);
	~YWQVector3();
	YWQVector3(const YWQVector3 &v);
	void operator=(const YWQVector3 &v);  
	YWQVector3 operator+(const YWQVector3 &v);
	YWQVector3 operator-(const YWQVector3 &v);
	YWQVector3 operator/(const YWQVector3 &v);
	YWQVector3 operator*(const YWQVector3 &v);
	YWQVector3 operator+(float f);
	YWQVector3 operator-(float f);
	YWQVector3 operator*(float f);
	YWQVector3 operator/(float f);
	float dot(const YWQVector3 &v);
	float length();
	void normalize();
	YWQVector3 crossProduct(const YWQVector3 &v);
	void printYWQVector3();
};


YWQVector3::YWQVector3(float x,float y,float z) 
	:x(x)
	,y(y)
	,z(z)
{

}

YWQVector3::~YWQVector3(){}

void YWQVector3::operator=(const YWQVector3 &v) {
	x=v.x;
	y=v.y;
	z=v.z;
}
YWQVector3::YWQVector3(const YWQVector3 &v)
	:x(v.x)
	,y(v.y)
	,z(v.z)
{
}

YWQVector3 YWQVector3::operator+(const YWQVector3 &v) {
	return YWQVector3(x + v.x, y + v.y, z + v.z);
}

YWQVector3 YWQVector3::operator-(const YWQVector3 &v) {
	return YWQVector3(x - v.x, y - v.y, z - v.z);
}


const double Zero = 1e-6;  

YWQVector3 YWQVector3::operator*(const YWQVector3 &v) {
	return YWQVector3(x * v.x, y * v.y, z * v.z);
}

YWQVector3 YWQVector3::operator/(const YWQVector3 &v) {
	if (fabsf(v.x) <= Zero || fabsf(v.y) <= Zero || fabsf(v.z) <= Zero)  {  
		std::cerr<<"Over flow!\n";  
		return *this;  
	}  
	return YWQVector3(x / v.x,y / v.y,z / v.z);
}



YWQVector3 YWQVector3::operator+(float f) {
	return YWQVector3(x + f, y + f, z + f);
}

YWQVector3 YWQVector3::operator-(float f) {
	return YWQVector3(x - f, y - f, z - f);
}

YWQVector3 YWQVector3::operator*(float f) {
	return YWQVector3(x * f, y * f, z * f);
}

YWQVector3 YWQVector3::operator/(float f) {
	if(fabs(f) < Zero) {
		std::cerr<<"Over flow!\n";  
		return *this;  
	}
	return YWQVector3(x / f, y / f, z / f);
}


float YWQVector3::dot(const YWQVector3 &v) {
	return x * v.x + y * v.y + z* v.z;
}

YWQVector3 YWQVector3::crossProduct(const YWQVector3 &v)  
{  
	return YWQVector3(y * v.z - z * v.y,  
		z * v.x - x * v.z,  
		x * v.y - y * v.x);  
}  

float YWQVector3::length() {
	return sqrtf(dot(*this));
}

void YWQVector3::normalize() {
	float len = length();
	if(len < Zero) len = 1;
	len = 1/len;
	x *= len;
	y *= len;
	z *= len;
}

void YWQVector3::printYWQVector3() {
	if(z==0) z=0;
	if(y==0) y=0;
	if(x==0) x=0;
	std::cout<<"("<<x<<","<<y<<","<<z<<")"<<std::endl;
}




class YWQEdge {
private:	
	YWQVector3 start;
	YWQVector3 end;
public:
	YWQEdge():start(YWQVector3(0,0,0)),end(YWQVector3(0,0,0)){}
	YWQEdge(YWQVector3 start,YWQVector3 end) {
		this->start = start;
		this->end = end;
	}
	~YWQEdge();
};
YWQEdge::~YWQEdge() {

}

class YWQPlane {
private:
	std::vector<YWQEdge> edges;
	std::vector<YWQVector3> points;
public:
	YWQPlane();
	~YWQPlane();
	void addEdge(YWQEdge &e);
	void addPoint(YWQVector3 &p);
	YWQVector3 getNormalVector();
};//此对象必须手动构造哈哈哈

YWQPlane::YWQPlane() {
	edges.clear();
	points.clear();
}
YWQPlane::~YWQPlane() { }

void YWQPlane::addEdge(YWQEdge &e) {
	edges.push_back(e);
}

void YWQPlane::addPoint(YWQVector3 &p) {
	points.push_back(p);
}

YWQVector3 YWQPlane::getNormalVector() {
	
	YWQVector3 temp =  (points[1]-points[0]);
	YWQVector3 ans =  temp.crossProduct(points[2]-points[0]);
	ans.normalize();
	return ans;
}

class YWQPolyhedron {
private:
	std::vector<YWQPlane> planes;
public:
	YWQPolyhedron();
	~YWQPolyhedron();
	void addPlane(YWQPlane &pl);

};

YWQPolyhedron::YWQPolyhedron() {
	planes.clear();
}

YWQPolyhedron::~YWQPolyhedron() { }

void YWQPolyhedron::addPlane(YWQPlane &pl) {
	planes.push_back(pl);
}



int main() {
	//test  getnormalvector
	YWQPlane p1 = YWQPlane();
	p1.addPoint(YWQVector3(0,0,0));
	p1.addPoint(YWQVector3(1,0,0));
	p1.addPoint(YWQVector3(0,0,1));

	YWQPlane p2 = YWQPlane();
	p2.addPoint(YWQVector3(0,0,0));
	p2.addPoint(YWQVector3(0,-1,0));
	p2.addPoint(YWQVector3(1,0,0));

	YWQPlane p3 = YWQPlane();
	p3.addPoint(YWQVector3(0,0,0));
	p3.addPoint(YWQVector3(0,0,1));
	p3.addPoint(YWQVector3(0,-1,0));

	YWQPlane p4 = YWQPlane();
	p4.addPoint(YWQVector3(0,0,1));
	p4.addPoint(YWQVector3(0,1,0));
	p4.addPoint(YWQVector3(1,0,0));


	YWQVector3 normal[4];

	normal[0] = p1.getNormalVector();
	normal[0].printYWQVector3();

	normal[1] = p2.getNormalVector();
	normal[1].printYWQVector3();
	normal[2] = p3.getNormalVector();
	normal[2].printYWQVector3();
	normal[3] = p4.getNormalVector();
	normal[3].printYWQVector3();

	YWQPolyhedron poly = YWQPolyhedron();
	poly.addPlane(p1);
	poly.addPlane(p2);
	poly.addPlane(p3);
	poly.addPlane(p4);

	YWQVector3 pp = YWQVector3(0.5,0,0.5);
	bool flag =true;
	for(int i = 0; i < 4; i++) {
		int ans = pp.dot(normal[i]);
		if(ans)
			flag = false;
	}
	if(flag)
		std::cout<<"IN"<<std::endl;
	else 
		std::cout<<"OUT"<<std::endl;

	system("pause");
}