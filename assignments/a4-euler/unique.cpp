#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class Unique : public atkui::Framework {
   public:
      Unique() : atkui::Framework(atkui::Orthographic) {
      }
      virtual void setup(){
      }
      virtual void scene() {
         setColor(vec3(0.5,0.5,0.5));
         drawCube(vec3(50,50,0),vec3(50,50,50));

         // rotate(pi<float>()/4.0f, vec3(0,1,0));
         rotate(elapsedTime(), vec3(200,200,0));
         drawCube(vec3(200,200,0),vec3(50,50,50));
         
         setColor(vec3(1,0,0));
         drawCube(vec3(200,195,0),vec3(52,52,52));
      }
};

int main(int argc, char **argv) {
  Unique viewer;
  viewer.run();
}