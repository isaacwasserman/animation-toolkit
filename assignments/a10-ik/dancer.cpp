#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ADancer : public atkui::Framework
{
public:
   ADancer() : atkui::Framework(atkui::Perspective) {}
   virtual ~ADancer() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);
      _drawer.color = vec3(1,0,0);
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());

      IKController ik;
      int leftHand = _skeleton.getByName("Beta:LeftHand")->getID();
      int rightHand = _skeleton.getByName("Beta:RightHand")->getID();
      int leftFoot = _skeleton.getByName("Beta:LeftFoot")->getID();
      int rightFoot = _skeleton.getByName("Beta:RightFoot")->getID();

      std::vector<Joint*> leftArmChain;
      Joint* j = _skeleton.getByName("Beta:LeftHand")->getParent();
      while(j->getName() != "Beta:LeftShoulder"){
         leftArmChain.push_back(j);
         j = j->getParent();
      }
      leftArmChain.push_back(j);


      std::vector<Joint*> rightArmChain;
      j = _skeleton.getByName("Beta:RightHand")->getParent();
      while(j->getName() != "Beta:RightShoulder"){
         rightArmChain.push_back(j);
         j = j->getParent();
      }
      rightArmChain.push_back(j);

      std::vector<Joint*> leftLegChain;
      j = _skeleton.getByName("Beta:LeftFoot")->getParent();
      while(j->getName() != "Beta:Hips"){
         leftLegChain.push_back(j);
         j = j->getParent();
      }
      leftLegChain.push_back(j);


      std::vector<Joint*> rightLegChain;
      j = _skeleton.getByName("Beta:RightFoot")->getParent();
      while(j->getName() != "Beta:Hips"){
         rightLegChain.push_back(j);
         j = j->getParent();
      }
      rightLegChain.push_back(j);

      vec3 hipPos = _skeleton.getByName("Beta:Hips")->getLocalTranslation() + vec3(0, 10*sin(5*elapsedTime()), 0);
      _skeleton.getByName("Beta:Hips")->setLocalTranslation(hipPos);

      vec3 leftHandPos = _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation() + vec3(30, 100*sin(5*elapsedTime()), 40);
      vec3 rightHandPos = _skeleton.getByName("Beta:RightHand")->getGlobalTranslation() + vec3(-30, 100*sin(5*(elapsedTime()+pi<float>())), 40);

      IKController::solveIKCCD(_skeleton, leftHand, leftHandPos, leftArmChain, 0.1f, 50, 0.1);
      IKController::solveIKCCD(_skeleton, rightHand, rightHandPos, rightArmChain, 0.1f, 50, 0.1);

      IKController::solveIKCCD(_skeleton, leftFoot, vec3(20, 30, 0), leftLegChain, 0.1f, 50, 0.1);
      IKController::solveIKCCD(_skeleton, rightFoot, vec3(-20, 30, 0), rightLegChain, 0.1f, 50, 0.1);
      _skeleton.fk();
   }

   void scene()
   {  
      update();
      _drawer.draw(_skeleton, *this);
      setColor(vec3(0,0,1));
      drawSphere(_lhandTarget, 10);
      drawSphere(_rhandTarget, 10);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Motion _motion;

   // Hand target positions
   vec3 _lhandTarget;
   vec3 _rhandTarget;
};

int main(int argc, char** argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}

