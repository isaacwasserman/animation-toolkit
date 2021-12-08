#include "ik_controller.h"
#include <cmath>
#include <iostream>

using namespace atk;
using namespace glm;
using namespace std;

bool IKController::solveIKAnalytic(Skeleton& skeleton, 
    int jointid, const vec3& goalPos, float epsilon) {
  if (jointid == -1) return true;

  Joint* ankle = skeleton.getByID(jointid);
  if (!ankle->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* knee = ankle->getParent();
  if (!knee->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* hip = knee->getParent();

  float goalLength = length(goalPos - hip->getGlobalTranslation());

  // vec3 r = ankle->getGlobalTranslation() - hip->getGlobalTranslation();
  // vec3 e = goalPos - ankle->getGlobalTranslation();
  // float deltaPhi = nudgeFactor * atan2(length(cross(r,e)), dot(r,r)+dot(r,e));
  // vec3 axis = cross(r,e)/length(cross(r,e));
  // quat nudge = angleAxis(deltaPhi,axis);
  
  return true;
}

bool IKController::solveIKCCD(Skeleton& skeleton, int jointid, 
    vec3 goalPos, const std::vector<Joint*>& chain, 
    float threshold, int maxIters, float nudgeFactor) {
  // There are no joints in the IK chain for manipulation
  if (chain.size() == 0) return true;

  Joint* endEffector = skeleton.getByID(jointid);

  // for(int i=0; i<chain.size(); i++){
  //   Joint* j = chain[i];
  //   std::cout << j->getName() << std::endl;
  // }

  int nIters = 0;
  while(length(goalPos - endEffector->getGlobalTranslation()) > threshold && nIters < maxIters){
    for(int i=0; i<chain.size(); i++){
      Joint* j = chain[i];
      vec3 r = endEffector->getGlobalTranslation() - j->getGlobalTranslation();
      vec3 e = goalPos - endEffector->getGlobalTranslation();
      float deltaPhi = nudgeFactor * atan2(length(cross(r,e)), dot(r,r)+dot(r,e));
      vec3 axis = cross(r,e)/length(cross(r,e));
      quat nudge = angleAxis(deltaPhi,axis);
      j->setLocalRotation(j->getLocalRotation() * nudge);
    }
    skeleton.fk();
    nIters++;
  }
  return (length(goalPos - endEffector->getGlobalTranslation()) <= threshold);
}
