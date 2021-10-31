#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;

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

class Unique : public atkui::Framework {
public:
   Unique() : atkui::Framework(atkui::Orthographic) {}
   virtual ~Unique() {}
    std::vector<Particle> particles;
    virtual void setup() {
        lookAt(vec3(200), vec3(0));
        int gridWidth = 5;
        int spacing = 50;
        for(int i=0;i<gridWidth;i++){
            for(int k=0;k<gridWidth;k++){
                Skeleton _tentacle;
                Joint* root  = new Joint("root");
                root->setLocalTranslation(vec3(i*spacing-100,0,k*spacing-450));
                _tentacle.addJoint(root);

                Joint* joint1  = new Joint("joint1");
                joint1->setLocalTranslation(vec3(0,50,0));
                _tentacle.addJoint(joint1, root);

                Joint* joint2  = new Joint("joint2");
                joint2->setLocalTranslation(vec3(0,50,0));
                _tentacle.addJoint(joint2, joint1);

                Joint* joint3  = new Joint("joint3");
                joint3->setLocalTranslation(vec3(0,50,0));
                _tentacle.addJoint(joint3, joint2);

                Joint* joint4  = new Joint("joint4");
                joint4->setLocalTranslation(vec3(0,50,0));
                _tentacle.addJoint(joint4, joint3);

                Joint* joint5  = new Joint("joint5");
                joint5->setLocalTranslation(vec3(0,50,0));
                _tentacle.addJoint(joint5, joint4);

                _tentacle.fk(); // compute local2global transforms
                blades.push_back(_tentacle);
            }
        }
        int nParticles = 100;
        vec3 baseVel = vectorFilter(agl::randomUnitVector(), vec3(50,-100,0));
        vec3 baseCol = vec3(1);
        for(int i=0; i<nParticles; i++){
            float x = agl::random() * width();
            float y = agl::random() * height();
            vec3 pos = vec3(x-300,y+100,-350);
            vec3 vel = baseVel * agl::random();
            vec3 col = baseCol + (agl::randomUnitVector()/2.0f);
            particles.push_back(Particle(pos,vel,col));
        }
    }

    virtual void scene()
    {
        rotate(0, vec3(0,0,1));
        for(Skeleton& _tentacle : blades){
            int nJoints = _tentacle.getNumJoints();
            for(int i=0;i<nJoints;i++){
                _tentacle.getByID(i)->setLocalRotation(glm::angleAxis((float) sin(elapsedTime() + i)/1.5f,vec3(0,0,1)));
            }

            _tentacle.fk(); // computes local2global transforms
            setColor(vec3(0,1,0));

            for(int i=0;i<nJoints;i++){
                Joint* child = _tentacle.getByID(i);
                Joint* parent = child->getParent();
                if(parent == NULL){
                    continue;
                }
                vec3 globalParentPos = parent->getGlobalTranslation();
                vec3 globalPos = child->getGlobalTranslation();
                drawEllipsoid(globalParentPos, globalPos, 5);
            }
        }
        setColor(vec3(0,0,0.5));
        rotate(glm::pi<float>()/10, vec3(0.5,0.5,0));
        drawCube(vec3(0,-100,0), vec3(6000,1,6000));
        for(Particle& particle : particles){
            setColor(particle.color);
            drawSphere(particle.position, 10);
            vec3 delta = particle.velocity * dt();
            particle.incrementPosition(delta);
            if(outOfBounds(particle)){
                // if(particle.position[0] > (width() + 10)){
                // particle.position[0] = 0;
                // particle.position[1] = (height() - particle.position[1]);
                // }
                // else if(particle.position[0] < -10){
                // particle.position[0] = width();
                // particle.position[1] = (height() - particle.position[1]);
                // }
                // else if(particle.position[1] > (height() + 10)){
                // particle.position[0] = (width() - particle.position[0]);
                // particle.position[1] = 0;
                // }
                // else if(particle.position[1] < -10){
                // particle.position[0] = (width() - particle.position[0]);
                // particle.position[1] = height();
                // }
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

protected:
    std::vector<Skeleton> blades;
};

int main(int argc, char** argv)
{
   Unique viewer;
   viewer.run();
} 

