#include "atkui/framework.h"

using namespace glm;

class Tree {
  public:
    vec3 rootPosition;
    vec3 tipPosition;
    float length;
    float width;
    vec3 rotation;
    Tree(const vec3& rootPos, const vec3& relativeTipPos, const float& wid) {
      rootPosition = rootPos;
      tipPosition = rootPos + relativeTipPos;
      rotation = vec3(0,0,atan2(relativeTipPos[0], relativeTipPos[1]));
      width = wid;
      length = glm::length(relativeTipPos);
    }
};

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    // your code here!
  }

  virtual void scene() {
    vec3 center = vec3(width()*0.5, height()*0.5, 0);
    vec3 rootPos = vec3(width()*0.5, height()*0.25, 0);
    vec3 tipPos = rootPos+vec3(-20,50,0);
    Tree tree = Tree(rootPos, tipPos, 20);
    setColor(vec3(1,1,1));
    renderer.translate(vec3((rootPos[0]+tipPos[0])/2, (rootPos[1]+tipPos[1])/2, 0));
    renderer.scale(vec3(tree.width, tree.length, 1));
    renderer.rotate(1,vec3(20,0,0));
    renderer.cube();
  }
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}

