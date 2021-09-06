#include "atkui/framework.h"

using glm::vec3;

class Sphere3D : public atkui::Framework {
 public:
  vec3 pos;
  vec3 vel;
  vec3 dest;
  vec3 disp;

  Sphere3D() : atkui::Framework(atkui::Perspective) {
    pos = vec3(0,0,0);
    vel = vec3(0,0,0);
    dest = vec3(0,0,0);
    disp = vec3(0,0,0);
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    // draw a sphere at center of the world
    float radius = 50.0;

    update_pos();
    drawSphere(pos, radius);
    dest_check();
  }

  void keyUp(int key, int mods){
    // Space pressed
    if(key == GLFW_KEY_SPACE){
      move_random();
    }
    // R pressed
    if(key == GLFW_KEY_R){
      reset();
    }
  }

  void move_random(){
    vec3 direction = agl::randomUnitVector();
    float magnitude = agl::random() * 100;
    int duration = 2;
    disp = direction * magnitude;
    dest = pos + disp;
    vel = disp * (1.0f / duration);
    std::cout << magnitude << std::endl;
  }

  void update_pos(){
    vec3 step = (vel * dt());
    pos = pos + step;
    disp -= step;
  }

  void dest_check(){
    vec3 diff = dest - pos;
    if(glm::dot(vel, diff) <= 0){
      vel = vec3(0);
    }
    std::cout << disp << std::endl;
  }

  void reset(){
    pos = vec3(0,0,0);
    vel = vec3(0,0,0);
    dest = vec3(0,0,0);
    disp = vec3(0,0,0);
  }
};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
