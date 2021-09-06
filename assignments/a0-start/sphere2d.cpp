#include "atkui/framework.h"

using glm::vec3;

class Sphere2D : public atkui::Framework {
 public:
  // Constructor
  Sphere2D() : atkui::Framework(atkui::Orthographic) /* extends atkui::Framework, uses Orthographic mode */ {
  }

  // virtual function: can be overwritten by classes which inherit
  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    // draw a sphere at center of screen
    double x = width() * 0.5;
    double y = height() * 0.5;
    double radius = 100; 
    drawSphere(vec3(x,y,0), radius);

    // Draw the new red sphere
    setColor(vec3(1,0,0));
    radius = 140; 
    drawSphere(vec3(x,y,-100), radius);
  }
};

int main(int argc, char** argv)
{
  // Instantiates the viewer
  Sphere2D viewer;
  // .run() automatically calls .scene()
  viewer.run();
  return 0;
}
