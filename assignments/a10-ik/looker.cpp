#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ALooker : public atkui::Framework {
public:
   ALooker() : atkui::Framework(atkui::Perspective) {}
   virtual ~ALooker() {}

   void setup() {
      Motion motion;
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, motion);
      motion.update(_skeleton, 0);

      _drawer.color = vec3(1,0,0);
      _head = _skeleton.getByName("Beta:Head");
   }

   void reset(Joint* head) {
      head->setLocalRotation(IdentityQ);
      head->fk();
   }

   void lookAtTarget(Joint* head, const vec3& target) {
      vec3 delta = target - head->getGlobalTranslation();
      float deltaX = delta[0];
      float deltaY = delta[1];
      float deltaZ = delta[2];

      float thetaX = -1 * atan2(deltaY,deltaZ);
      float thetaY = (-1 * atan2(deltaZ,deltaX)) + pi<float>()/2;

      mat4 Rx = glm::rotate(mat4(1.0f), thetaX, vec3(1,0,0));
      mat4 Ry = glm::rotate(mat4(1.0f), thetaY, vec3(0,1,0));
      mat4 R = Ry*Rx;

      quat Rq = quat_cast(R);

      mat4 a = glm::rotate(mat4(1.0f), pi<float>()/2, vec3(1,0,0));
      quat aq = quat_cast(a);

      head->setLocalRotation(Rq);

      head->fk();
   }

   void scene() {  
      float r = 100;
      float angle = elapsedTime();
      _target = vec3(r * cos(angle), r * sin(angle) + r, 150);

      reset(_head);
      lookAtTarget(_head, _target);
      _drawer.draw(_skeleton, *this);

      vec3 globalHeadPos = _head->getGlobalTranslation();
      vec3 globalForward = _head->getGlobalRotation() * vec3(0,0,1);

      setColor(vec3(0,0,1));
      drawLine(globalHeadPos, globalHeadPos + 200.0f*globalForward);
      drawSphere(_target, 5);
   }

   Cyclops _drawer;
   Skeleton _skeleton;
   Joint* _head;
   vec3 _target;
};

int main(int argc, char** argv) {
   ALooker viewer;
   viewer.run();
   return 0;
}

