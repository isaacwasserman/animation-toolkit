#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class MotionViewer : public atkui::Framework
{
public:
   std::string BVHPath = "../motions/Beta/jump.bvh";
   MotionViewer() : atkui::Framework(atkui::Perspective) {
   }
   MotionViewer(std::string path) : atkui::Framework(atkui::Perspective) {
      BVHPath = path;
   }

   void setup() {
      BVHReader reader;
      reader.load(BVHPath, skeleton, motion);
      motion.update(skeleton, 0);
   }

   void scene() {
      if(!paused){
         time += dt() * timeScale;
      }
      motion.update(skeleton, time);

      setColor(vec3(0,0,0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++) {
         Joint* joint = skeleton.getByID(i);
         if (joint->getParent() == 0) continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      currentFrame = motion.getKeyID(time);

      drawText(paused? "Paused" : "Playing", 10, 15);
      drawText("Current frame: "+std::to_string(currentFrame), 10, 35);
      drawText("Time scale: "+std::to_string(timeScale), 10, 55);
   }

   virtual void keyUp(int key, int mods) {
      if(key == GLFW_KEY_P){
         paused = !paused;
      }
      else if(key == GLFW_KEY_0){
         time = 0.0f;
      }
      else if(key == GLFW_KEY_PERIOD){
         time += 1.0f/motion.getFramerate();
      }
      else if(key == GLFW_KEY_COMMA){
         time -= 1.0f/motion.getFramerate();
      }
      else if(key == GLFW_KEY_LEFT_BRACKET){
         timeScale -= 0.1f;
      }
      else if(key == GLFW_KEY_RIGHT_BRACKET){
         timeScale += 0.1f;
      }
   }

private:
   Skeleton skeleton;
   Motion motion;

   float timeScale = 1.0f;
   int currentFrame = 0; 
   bool paused = false;
   float time = 0;
};


int main(int argc, char** argv) {
   if(argc > 1){
      MotionViewer viewer = MotionViewer(argv[1]);
      viewer.run();
   }
   else {
      MotionViewer viewer;
      viewer.run();
   }
}
