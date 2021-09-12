#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }

    virtual void scene() {
      float pupilXOffsetState = cos(elapsedTime());
      float pupilYOffsetState = sin(elapsedTime());

      setColor(vec3(1,1,1));
      drawSphere(vec3(width()*0.33,height()*0.5,0), 130);
      drawSphere(vec3(width()*0.66,height()*0.5,0), 130);

      setColor(vec3(1,0,0));
      drawSphere(vec3(width()*0.33+(pupilXOffsetState*30),height()*0.5+(pupilYOffsetState*30),130), 35);
      drawSphere(vec3(width()*0.66+(pupilXOffsetState*30),height()*0.5+(pupilYOffsetState*30),130), 35);
    }
};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}
