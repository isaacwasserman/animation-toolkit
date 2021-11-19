#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include "atk/toolkit.h"
#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;
using namespace atk;

class WASDSimple : public atkui::Framework
{
public:
  WASDSimple() : atkui::Framework(atkui::Perspective) {}

  virtual void setup()
  {
    BVHReader reader;
    reader.load("../motions/Beta/walking.bvh", _skeleton, _walk);
    for(int i=0; i<_walk.getNumKeys(); i++){
      Pose newPose = _walk.getKey(i);
      newPose.rootPos = vec3(_walk.getKey(0).rootPos[0],_walk.getKey(i).rootPos[1],_walk.getKey(0).rootPos[2]);
      _walk.editKey(i, newPose);
    }
    _drawer.showAxes = true;
  }

  virtual void scene()
  {
   renderer.setUniform("Light.position", cos(elapsedTime()) * 900, 900, sin(elapsedTime()) * 900, 1.0f);
   vec3 color = (vec3(0,0,1) * (1 - sin(elapsedTime()))) + (vec3(1,0.5,0) * (sin(elapsedTime())));
   renderer.setUniform("Light.color", color[0],color[1],color[2]);
    update();
    push();
      translate(_pos);
      rotate(_heading, vec3(0,1,0));
      _drawer.draw(_skeleton, *this);
    pop();

    push();
     setColor(vec3(0.3,0.3,1));
     drawCube(vec3(0,1000,2000),vec3(2000));
     drawCube(vec3(0,1000,-2000),vec3(2000));
     drawCube(vec3(2000,1000,0),vec3(2000));
     drawCube(vec3(-2000,1000,0),vec3(2000));
     setColor(vec3(0.3,1,0.3));
     drawCube(vec3(0,-999,0),vec3(2000));
   pop();
  }

  virtual void update()
  {
    _walk.update(_skeleton, elapsedTime());

    // TODO: Your code here
    _pos += vec3(2 * sin(_heading), 0, 2 * cos(_heading));

    if(abs(_pos[0]) >= 1000 || abs(_pos[2]) >= 1000){
       _heading *= -1;
    }
    std::cout << _pos << std::endl;

    // TODO: Override the default camera to follow the character
    vec3 headPos = _skeleton.getByName("Beta:Head")->getGlobalTranslation() + _pos;
    Transform headTranslation = Transform(quat(1,0,0,0),_skeleton.getByName("Beta:Hips")->getGlobalTranslation(),vec3(1));

    Transform headTransform = Transform(quat(cos(_heading/2),0,sin(_heading/2),0),_skeleton.getByName("Beta:Hips")->getGlobalTranslation() + _pos,vec3(100));
    Transform cameraOffset = Transform(quat(1,0,0,0),vec3(0,1,-2),vec3(1));

    vec3 cameraPos = headTransform.transformPoint(cameraOffset.transformPoint(vec3(0,0,0)));
    lookAt(cameraPos, headPos, vec3(0, 1, 0));

    // update heading when key is down
    if (keyIsDown('D')) _heading -= 0.05;
    if (keyIsDown('A')) _heading += 0.05;
  }

protected:
  float _heading;
  vec3 _pos = vec3(0);

  Motion _walk;
  Skeleton _skeleton;
  atkui::SkeletonDrawer _drawer;
};

int main(int argc, char **argv)
{
  WASDSimple viewer;
  viewer.run();
}
