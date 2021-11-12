#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class ASplice : public atkui::Framework
{
public:
   ASplice() : atkui::Framework(atkui::Perspective) {}
   virtual ~ASplice() {}

   void setup()
   {
      _alpha = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   std::vector<Joint*> getDeepChildren(Joint* root){
      std::vector<Joint*> immediateChildren;
      std::vector<Joint*> allChildren;
      int numImmediateChildren = root->getNumChildren();
      for(int i=0; i<numImmediateChildren; i++){
         immediateChildren.push_back(root->getChildAt(i));
      }
      std::vector<std::vector<Joint*>> childrensChildren;
      for(Joint* child : immediateChildren){
         childrensChildren.push_back(getDeepChildren(child));
      }
      allChildren.insert( allChildren.end(), immediateChildren.begin(), immediateChildren.end() );
      for(std::vector<Joint*> childList : childrensChildren){
         allChildren.insert( allChildren.end(), childList.begin(), childList.end() );
      }
      return allChildren;
   }

   glm::quat getJointRot(Pose pose, Joint* joint){
      int jointID = joint->getID();
      return pose.jointRots[jointID];
   }

   Motion spliceUpperBody(const Motion& lower, const Motion& upper, float alpha)
   {
      Motion result;
      result.setFramerate(lower.getFramerate());
      Joint* spine = _skeleton.getByName("Beta:Spine1");
      int spineID = spine->getID();
      std::vector<int> upperBodyIDs;
      std::vector<Joint*> upperBodyJoints = getDeepChildren(spine);
      for(Joint* joint : upperBodyJoints){
         upperBodyIDs.push_back(joint->getID());
      }
      sort(upperBodyIDs.begin(), upperBodyIDs.end());

      for(int k=0; k<upper.getNumKeys(); k++){
         Pose basePose = lower.getKey(k % lower.getNumKeys());
         Pose gangnamPose = upper.getKey(k);
         for(Joint* upperBodyJoint : upperBodyJoints){
            glm::quat baseRot = getJointRot(basePose, upperBodyJoint);
            glm::quat gangnamRot = getJointRot(gangnamPose, upperBodyJoint);
            glm::quat newRot = glm::slerp(baseRot, gangnamRot, alpha);
            basePose.jointRots[upperBodyJoint->getID()] = newRot;
         }
         result.appendKey(basePose);
      }
      return result;
   }

   void scene()
   {  
      _splice.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: "+std::to_string(_alpha), 10, 15);
   }

   void keyUp(int key, int mods) 
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha+0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha-0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
};

int main(int argc, char** argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}

