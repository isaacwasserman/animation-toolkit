#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>
#include <time.h>
#include <deque>

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
      BVHReader reader;
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = _upper;
   }

   class Particle {
      public:
      vec3 pos;
      float size = 20.0f;
      float timeCreated;;
      Particle(vec3 p, float t){
         pos = p;
         timeCreated = t;
      }
      void update(float t){
         float age = t - timeCreated;
         float maxAge = 3.0f;
         if(age > maxAge){
            size = 0.0f;
         }
         else {
            size = 20.0f * (1.0f - (age / maxAge)) * (1.0f - (age / maxAge));
         }
         // std::cout << size << std::endl;
      }
   };

   std::deque<Particle> particles = {};

   void scene()
   {  
      _splice.update(_skeleton, elapsedTime());

      Joint* leftHand = _skeleton.getByName("Beta:LeftHand");
      Joint* rightHand = _skeleton.getByName("Beta:RightHand");
      vec3 leftHandPos = leftHand->getGlobalTranslation();
      vec3 rightHandPos = rightHand->getGlobalTranslation();

      for(Particle& particle : particles){
         particle.update(elapsedTime());
      }

      if(particles.size() > 0){
         while(particles[0].size == 0){
            particles.pop_front();
         }
      }

      particles.push_back(Particle(leftHandPos, elapsedTime()));
      particles.push_back(Particle(rightHandPos, elapsedTime()));
      
      setColor(vec3(1,0,0));
      for(Particle& particle : particles){
         drawSphere(particle.pos, particle.size);
      }
      // std::cout << particles.size() << std::endl;

      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
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

