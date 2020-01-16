#include "shape.hpp"
using namespace std;


//Base class
//Please implement Shape's member functions
//constructor, getName()
//
//Base class' constructor should be called in derived classes'
//constructor to initizlize Shape's private variable

Shape::Shape(string name){ //define Shape's constructor, since it is called in all derived class' constructors
  name_ = name;
}

string Shape::getName(){ //defiine member function of shape, getName()
  return name_;
}

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here


Rectangle::Rectangle(double width, double length) : Shape("Rectangle"){
  //constructor for Rectangle. takes params width, length and sets object's variables to those values, then calls shape's constructor with name = "Rectangle"
  width_ = width;
  length_ = length;
}
double Rectangle::getArea()const{ //returns object's area by multiplying length and width
  return length_ * width_;
}
double Rectangle::getVolume()const{ //volume of a rectangle is 0
  return 0;
}
Rectangle Rectangle::operator + (const Rectangle& rec){ //define operator +, which adds the lengths and widths, respectively
  double newLength = length_ + rec.getLength(); //define new length and width
  double newWidth = width_ + rec.getWidth();
  return *(new Rectangle(newWidth, newLength)); //create new object as sum and return it
}
Rectangle Rectangle::operator - (const Rectangle& rec){ //define operator -, which is max of l1 or w1 mins l2 or w2, respectively, and 0
  double newWidth = (width_ - rec.getWidth() > 0) ? width_ - rec.getWidth() : 0;
  double newLength = (length_ - rec.getLength() > 0) ? length_ - rec.getLength() : 0;
  return *(new Rectangle(newWidth, newLength)); //create new object and return it
}
double Rectangle::getWidth()const{return width_;}
double Rectangle::getLength()const{return length_;}


//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

Circle::Circle(double radius) : Shape("Circle"){ //constructor for circle, which sets radius_ and calls shape's constructor with name = "Circle"
  radius_ = radius;
}
double Circle::getArea()const{
  return M_PI * radius_ * radius_; //return pi r^2 == area of a circle
}
double Circle::getVolume()const{
  return 0; //volume of circle == 0
}
Circle Circle::operator + (const Circle& cir){ //return new circle with r = r1 + r2
  double newRad = radius_ + cir.getRadius();
  return *(new Circle(newRad));
}
Circle Circle::operator - (const Circle& cir){ //return new circle with r = max(0, r1 - r2)
  double newRad = (0 > radius_ - cir.getRadius()) ? 0 : radius_ - cir.getRadius();
  return *(new Circle(newRad));
}
double Circle::getRadius()const{return radius_;}

//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
Sphere::Sphere(double radius) : Shape("Sphere"){ //constructor of sphere, same algorithm as circle - pass parammeter radius to radius_
  //also calls shape's constructor with name = "Sphere"
  radius_ = radius;
}
double Sphere::getArea()const{ //returns value equal to the area of a sphere (4 * pi * r * r)
  return 4.0 * M_PI * radius_ * radius_;
}
double Sphere::getVolume()const{ //returns volume of a sphere (4/3 * pi * r * r * r)
  return (4.0/3.0)*M_PI*radius_*radius_*radius_;
}
Sphere Sphere::operator + (const Sphere& sph){ //return new sphere with r = r1 + r2
  double newRad = radius_ + sph.getRadius();
  return *(new Sphere(newRad));
}
Sphere Sphere::operator - (const Sphere& sph){ //return new sphere with r = max(r1-r2, 0)
  double newRad = (radius_ - sph.getRadius() > 0) ? radius_ - sph.getRadius() : 0;
  return *(new Sphere(newRad));
}

double Sphere::getRadius()const{return radius_;}

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
RectPrism::RectPrism(double width, double length, double height) : Shape("RectPrism"){
  //constructor for RectPrism. passes width, length, and height, and calls Shape's constructor with name = "RectPrism"
  width_ = width;
  length_ = length;
  height_ = height;
}
double RectPrism::getArea()const{ //returns area which equals (2 * (length * width + length * height + width * height))
  return 2*(length_ * width_+length_ * height_+width_ * height_);
}
double RectPrism::getVolume()const{ //returns volume, which equals length * width * height
  return length_*width_*height_;
}
RectPrism RectPrism::operator + (const RectPrism& rectp){ //returns new rectangle with l = l1 +  l2, w = w1 + w2, h = h1 + h2
  double newLength = length_ + rectp.getLength();
  double newWidth = width_ + rectp.getWidth();
  double newHeight = height_ + rectp.getHeight();
  return *(new RectPrism(newWidth, newLength, newHeight));
}
RectPrism RectPrism::operator - (const RectPrism& rectp){ //new rectangle with l = max(l1 - l2 ,0), etc.
  double newWidth = (width_ - rectp.getWidth() > 0) ? width_ - rectp.getWidth() : 0;
  double newLength = (length_ - rectp.getLength() > 0) ? length_ - rectp.getLength() : 0;
  double newHeight = (height_ - rectp.getHeight() > 0) ? height_ - rectp.getHeight() : 0;
  return *(new RectPrism(newWidth, newLength, newHeight));
}

double RectPrism::getWidth()const{return width_;}
double RectPrism::getHeight()const{return height_;}
double RectPrism::getLength()const{return length_;}



// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects
vector<Shape*> CreateShapes(char* file_name){
	//@@Insert your code here
  vector<Shape*> vec; //creates vector to hold all the shapes
  ifstream in; //ifstream object that will stram file_name
  double w, l, h, r;//various parammeters that will be used in creation of shapes
  int n, i=0; //n holds number of shapes in file, i used as counter
  string str; //holder for any string inputs
  Shape *shape; //pointer to a shape that will be defined as the object to add to vector
  in.open(file_name); //open file, read first item (# of shapes)
  in >> n;
  while(i < n){ //loop through all shapes,
    in >> str; //read name, define shape accordingly according to name
    if(str == "Rectangle"){
      in >> w >> l;
      shape = new Rectangle (w, l);
    }
    else if(str == "Circle"){
      in >> r;
      shape = new Circle(r);
    }
    else if (str == "RectPrism"){
      in >> w >> l >> h;
      shape = new RectPrism(w, l, h);
    }
    else if (str == "Sphere"){
      in >> r;
      shape = new Sphere(r);
    }
    vec.push_back(shape); //add shape to vector
    i++; //increment i
  }
	return vec; //return final vector
}

// call getArea() of each object
// return the max area
double MaxArea(vector<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
  for(int i=0; i<shapes.size(); i++){ //loop through elements of shapes, call getArea on them, update max_area accordingly
    if (shapes[i]->getArea() > max_area){
      max_area = shapes[i]->getArea();
    }
  }
	return max_area; //return final max area
}


// call getVolume() of each object
// return the max volume
double MaxVolume(vector<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
  for(int i=0; i<shapes.size(); i++){ //loop through vector, call getVolume on each element, update max_volume if > current max_volume
    if(shapes[i]->getVolume() > max_volume){
      max_volume = shapes[i]->getVolume();
    }
  }
	return max_volume; //return final max volume
}
