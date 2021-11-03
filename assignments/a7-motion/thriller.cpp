#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   std::vector<Devil> devils;

   virtual void setup() {
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      vec3 position = vec3(0);
      float size = 1.0f;
      int spacing = 200;
      vec3 color = vec3(1,0,0);
      for(int row=0; row<3; row++){
         for(int column=0; column<4; column++){
            position = vec3((spacing * row) + (agl::random() * 100), 0, (spacing * column) + (agl::random() * 100));
            color = agl::random() * vec3(1,0,0);
            size = agl::random() * 2;
            devils.push_back(Devil(position, color, size));
         }  
      }
      _devil = Devil(position, color, size);
   }

   virtual void scene() {
      if (!_paused) _motion.update(_skeleton, elapsedTime());
      for(Devil& devil : devils){
         devil.draw(_skeleton, *this);
      }
   }

   virtual void keyUp(int key, int mods) {
      if (key == 'P') _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   Devil _devil;
};

int main(int argc, char** argv) {
   Thriller viewer;
   viewer.run();
}

