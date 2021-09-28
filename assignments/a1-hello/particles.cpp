#include "atkui/framework.h"

using namespace glm;

class Particle {
  public:
    vec3 position;
    vec3 velocity;
    vec3 color;
    Particle(const vec3& pos, const vec3& vel, const vec3& col) {
      position = pos;
      velocity = vel;
      color = col;
    }

    void incrementPosition(const vec3& delta){
      position += delta;
    }
};

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }
  std::vector<Particle> particles;
  virtual void setup() {
    int nParticles = 100;
    vec3 baseVel = vectorFilter(agl::randomUnitVector(), vec3(100,100,0));
    vec3 baseCol = vec3(0.2,1,0.2);
    for(int i=0; i<nParticles; i++){
      float x = agl::random() * width();
      float y = agl::random() * height();
      vec3 pos = vec3(x,y,0);
      vec3 vel = baseVel * agl::random();
      vec3 col = baseCol + (agl::randomUnitVector()/2.0f);
      particles.push_back(Particle(pos,vel,col));
    }
  }
  virtual void scene() {
    for(Particle& particle : particles){
      setColor(particle.color);
      drawSphere(particle.position, 10);
      vec3 delta = particle.velocity * dt();
      particle.incrementPosition(delta);
      if(outOfBounds(particle)){
        if(particle.position[0] > (width() + 10)){
          particle.position[0] = 0;
          particle.position[1] = (height() - particle.position[1]);
        }
        else if(particle.position[0] < -10){
          particle.position[0] = width();
          particle.position[1] = (height() - particle.position[1]);
        }
        else if(particle.position[1] > (height() + 10)){
          particle.position[0] = (width() - particle.position[0]);
          particle.position[1] = 0;
        }
        else if(particle.position[1] < -10){
          particle.position[0] = (width() - particle.position[0]);
          particle.position[1] = height();
        }
      }
    }
  }

  bool outOfBounds(const Particle& particle){
    vec3 pos = particle.position;
    if(pos[0] > (width() + 10) || pos[1] > (height() + 10) || pos[0] < -10 || pos[1] < -10){
      return true;
    }
    else {
      return false;
    }
  }

  vec3 vectorFilter(const vec3& v1, const vec3& v2){
    return vec3(v1[0]*v2[0],v1[1]*v2[1],v1[2]*v2[2]);
  }
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}