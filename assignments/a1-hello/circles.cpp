#include "atkui/framework.h"

using namespace glm;

class Particle {
  public:
    float distance;
    float angularVelocity;
    float theta;
    vec3 color;
    Particle(const float& dist, const float& th, const float& vel, const vec3& col) {
      distance = dist;
      theta = th;
      angularVelocity = vel;
      color = col;
    }
};

class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }
   std::vector<Particle> particles;
   virtual void setup() {
      int nRings = 11;
      int particlesPerRing = 36;
      float circleSpacing = 30;
      int rotationDirection = 1;
      float radius = 30;
      std::vector<vec3> palette = {
         vec3(1,0.6824,0.0018),
         vec3(0.996, 0.306, 0),
         vec3(1, 0.059, 0.502),
         vec3(0.945, 0.949, 0.933)
      };
      for(int i=0; i<nRings; i++){
         float theta = 0;
         vec3 ringColor = palette[i % palette.size()];
         for(int k=0; k<particlesPerRing; k++){
            particles.push_back(Particle(radius, theta, pi<float>() / 4000 * rotationDirection, ringColor));
            theta += 2 * pi<float>() / 36;
         }
         rotationDirection *= -1;
         radius += circleSpacing;
      }
   }

   virtual void scene() {
      int particleRadius = 25;
      vec3 center = vec3(width()*0.5,height()*0.5,0);
      for(Particle particle : particles){
         setColor(particle.color);
         vec3 position = (vec3(cos(particle.theta), sin(particle.theta), 0) * particle.distance) + center;
         drawSphere(position, particleRadius);
         updateParticles();
      }
      setColor(vec3(0.263, 0.314, 0.345));
      renderer.push();
      renderer.translate(center - vec3(0,0,100));
      renderer.scale(vec3(width(), height(), 1));
      renderer.cube();
      renderer.pop();
   }

   void keyUp(int key, int mods){
      // Space pressed
      if(key == GLFW_KEY_SPACE){
         updateParticles();
      }
   }

   void updateParticles(){
      for(Particle& particle : particles){
         particle.theta += particle.angularVelocity * dt();
      }
   }

};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
