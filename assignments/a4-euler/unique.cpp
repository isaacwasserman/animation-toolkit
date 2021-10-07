#include <iostream>
#include <deque>
#include<cmath>
#include "atkui/framework.h"
using namespace glm;

// class Cube {
//    vec3 pos = vec3(0);
//    vec3 vel = vec3(0);
//    Cube(const vec3 initPos, const vec3 initVel){
//       pos = initPos;
//       vel = initVel;
//    }
//    ~Cube(){}
//    void update(const float dt){
//       pos += vel * dt;
//    }
// };

class Unique : public atkui::Framework {
   public:
      Unique() : atkui::Framework(atkui::Orthographic) {
      }
      float stepSize = 100;
      float yOffset = -310;
      float xOffset = -150;
      std::deque<std::vector<vec3>> cubes = {};
      vec3 stairVelocity = vec3(-100,-100,0);
      float t = 0;
      float ballFloor = 200;
      vec3 lightColor = vec3(agl::random(),agl::random(),agl::random());
      float colorLastSetTime = 0;
      virtual void setup(){
         while(stepSize * cubes.size() <= height()){
            vec3 pos;
            if(cubes.size() == 0){
               pos = vec3(width() - (stepSize / 2) + xOffset, height() - (stepSize / 2) + yOffset, 0);
            }
            else {
               pos = cubes[cubes.size() - 1][0] + vec3(stepSize, stepSize, 0);
            }
            cubes.push_back({pos, vec3(stepSize)});
         }
      }
      virtual void scene() {
         setColor(lightColor);
         drawCube(vec3(width()/2, height()/2, -200), vec3(width(), height(), 1));
         if(elapsedTime() - colorLastSetTime > 0.5 && t >= 0.5){
            lightColor = vec3(agl::random(),agl::random(),agl::random());
            colorLastSetTime = elapsedTime();
         }

         renderer.beginShader("phong");
         renderer.setUniform("Gamma", 1.6f);
         renderer.setUniform("Light.position", width()/0.75f, height(), 1000.0f, 1.0f);
         // renderer.setUniform("Light.color", lightColor[0], lightColor[1], lightColor[2]);

         setColor(vec3(34,46,80) * (1.0f/255.0f));
         rotate(0.5, vec3(0.16,0.2,0.03));
         for(std::vector<vec3>& cube : cubes){
            drawCube(cube[0],cube[1]);
            cube[0] += (dt() * stairVelocity);
         }
         if(cubes[0][0][0] <= -1 * stepSize || cubes[0][0][1] <= -1 * stepSize){
               cubes.pop_front();
               vec3 pos = cubes[cubes.size() - 1][0] + vec3(stepSize, stepSize, 0);
               cubes.push_back({pos, vec3(stepSize)});
         }
         t = fmod(t+dt(),1);
         int dropHeight = 335;
         int floorHeight = 160;
         int ballSize = 50;
         setColor(vec3(255,51,99) * (1.0f/255.0f));
         renderer.setUniform("Material.specular", 0.1f, 0.1f, 0.1f);
         if(t <= 0.5){
            drawSphere(vec3(width()/2,-9.8*71.43*t*t+dropHeight,0), ballSize);
         } else {
            drawSphere(vec3(width()/2,-9.8*71.43*pow(t-1,2)+dropHeight,0), ballSize);
         }
      }
};

int main(int argc, char **argv) {
  Unique viewer;
  viewer.run();
}