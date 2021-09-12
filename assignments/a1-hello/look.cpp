#include "atkui/framework.h"

using namespace glm;

class Look : public atkui::Framework {
 public:
  Look() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;
  }

  virtual void scene() {
    _mouseX = mousePosition()[0];
    _mouseY = (mousePosition()[1] * -1) + height();
    vec3 target = vec3(_mouseX, _mouseY, 0);
    setColor(vec3(1,0,0));
    drawSphere(target, 10);

    vec3 leftEyePos = vec3(width()*0.33,height()*0.5,0);
    vec3 rightEyePos = vec3(width()*0.66,height()*0.5,0);

    float leftPupilRelativeAngle = (relativeAngle(leftEyePos, target) * -1)-pi<float>()/2;
    float rightPupilRelativeAngle = (relativeAngle(rightEyePos, target) * -1)-pi<float>()/2;

    vec2 leftPupilOffsets = getOffsets(leftEyePos, target);
    vec2 rightPupilOffsets = getOffsets(rightEyePos, target);

    setColor(vec3(1,1,1));
    drawSphere(leftEyePos, 130);
    drawSphere(rightEyePos, 130);

    setColor(vec3(1,0,0));
    drawSphere(vec3(leftEyePos[0] + leftPupilOffsets[0],leftEyePos[1] + leftPupilOffsets[1],leftEyePos[2]+130), 35);
    drawSphere(vec3(rightEyePos[0] + rightPupilOffsets[0],rightEyePos[1] + rightPupilOffsets[1],rightEyePos[2]+130), 35);
  }

  vec2 getOffsets(const vec3& eyepos, const vec3& target){
    vec2 offsets;
    if(length(eyepos - target) < 35){
      offsets = (target - eyepos);
    }
    else {
      float relAngle = (relativeAngle(eyepos, target) * -1)-pi<float>()/2;
      offsets = vec2(cos(relAngle)*35,sin(relAngle)*35);
    }
    return offsets;
  }

  float relativeAngle(const vec3& eyePos, const vec3& target){
    float deltaX = eyePos[0] - target[0];
    float deltaY = eyePos[1] - target[1];

    return atan2(deltaX, deltaY);
  }

 private:
  int _mouseX;
  int _mouseY;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
