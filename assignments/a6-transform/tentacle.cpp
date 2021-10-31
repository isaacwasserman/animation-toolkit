#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;

class Tentacle : public atkui::Framework {
public:
   Tentacle() : atkui::Framework(atkui::Perspective) {}
   virtual ~Tentacle() {}

   virtual void setup() {
      lookAt(vec3(200), vec3(0));

      Joint* root  = new Joint("root");
      root->setLocalTranslation(vec3(0,0,0));
      _tentacle.addJoint(root);

      Joint* joint1  = new Joint("joint1");
      joint1->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint1, root);

      Joint* joint2  = new Joint("joint2");
      joint2->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint2, joint1);

      Joint* joint3  = new Joint("joint3");
      joint3->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint3, joint2);

      Joint* joint4  = new Joint("joint4");
      joint4->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint4, joint3);

      Joint* joint5  = new Joint("joint5");
      joint5->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint5, joint4);

      _tentacle.fk(); // compute local2global transforms
   }

   virtual void scene()
   {
      int nJoints = _tentacle.getNumJoints();
      for(int i=0;i<nJoints;i++){
         _tentacle.getByID(i)->setLocalRotation(glm::angleAxis((float) sin(elapsedTime() + i)/1.5f, vec3(0,0,1)));
      }
      
      _tentacle.fk(); // computes local2global transforms
      setColor(vec3(0,1,0));

      for(int i=0;i<nJoints;i++){
         Joint* child = _tentacle.getByID(i);
         Joint* parent = child->getParent();
         if(parent == NULL){
            continue;
         }
         vec3 globalParentPos = parent->getGlobalTranslation();
         vec3 globalPos = child->getGlobalTranslation();
         drawEllipsoid(globalParentPos, globalPos, 5);
      }
   }

protected:
   Skeleton _tentacle;
};

int main(int argc, char** argv)
{
   Tentacle viewer;
   viewer.run();
} 

