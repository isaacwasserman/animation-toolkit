#include "atkui/framework.h"

using namespace glm;

class Ball {
  public:
    vec3 position;
    float radius = 30;
    vec3 color = vec3(1,1,1);
    float dynamicsMultiplier = 20;
    vec3 acceleration = vec3(0,-9.8,0) * dynamicsMultiplier;
    vec3 velocity = vec3(0);
    float bounciness = 0.95;
    float nextAllowedCollisionTime = 0;
    float collisionTimer = 0.25;

    Ball(const vec3& pos, const vec3& vel, const vec3& col) {
      position = pos;
      velocity = vel;
      color = col;
    }
    
    void updateDynamics(const float& dt, const int& width, const int& height, std::vector<Ball>& balls, const float& elapsedTime){
      position += velocity * dt;
      velocity += acceleration * dt;
      checkBoxCollision(width, height);
      // checkBallCollision(balls, elapsedTime);
    }

    int checkBoxCollision(const int& width, const int& height){
      // Right collision
      if(position[0] + (radius * 0.5) >= width && velocity[0] > 0){
        velocity[0] = velocity[0] * -1 * bounciness;
        return 1;
      }
      // Left collision
      else if(position[0] - (radius * 0.5) <= 0 && velocity[0] < 0){
        velocity[0] = velocity[0] * -1 * bounciness;
        return 2;
      }
      // Top collision
      else if(position[1] + (radius * 0.5) >= height && velocity[1] > 0){
        velocity[1] = velocity[1] * -1 * bounciness;
        return 3;
      }
      // Bottom collision
      else if(position[1] - (radius * 0.5) <= 0 && velocity[1] < 0){\
        velocity[1] = velocity[1] * -1 * bounciness;
        return 4;
      }
      else {
        return 0;
      }
    }

    int checkBallCollision(std::vector<Ball>& balls, const float& elapsedTime){
      float collisionBuffer = 30;
      if(elapsedTime >= nextAllowedCollisionTime){
        for(Ball& ball : balls){
        if(&ball != this){
          float angleOfApproach = atan2((position - ball.position)[0], (position - ball.position)[1]);
          float velocityAngle = atan2(velocity[0], velocity[1]);
          float distance = length(position - ball.position);
          if(distance <= (radius * 2) + collisionBuffer && std::abs(angleOfApproach - velocityAngle) <= (pi<float>() / 2)){
            float currentVelocityMagnitude = length(velocity);
            float angleOfDeparture = angleOfApproach + pi<float>();
            float newVelocityMagnitude = currentVelocityMagnitude * bounciness;
            vec3 newVelocityUnitVector = vec3(cos(angleOfDeparture), sin(angleOfDeparture), 0);
            velocity = newVelocityUnitVector * newVelocityMagnitude;
            nextAllowedCollisionTime = elapsedTime + collisionTimer;
            return 0;
          }
        }
      }
      }
      return 0;
    }
};

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  vec3 center = vec3(width()*0.5, height()*0.5, 0);
  std::vector<Ball> balls;
  int nBalls = 20;
  float maxInitialXVelocity = 200;
  int ballZIndex = 0;

  std::vector<vec3> palette = {
    vec3(1,0.6824,0.0018),
    vec3(0.996, 0.306, 0),
    vec3(1, 0.059, 0.502),
    vec3(0.945, 0.949, 0.933)
  };

  virtual void setup() {
    for(int i=0; i<nBalls; i++){
      spawnBall();
    }
  }

  virtual void scene() {
    drawBackground();
    for(Ball& ball : balls){
      drawBall(ball);
    }
  }

  void drawBall(Ball& ball){
    setColor(ball.color);
    drawSphere(ball.position, ball.radius);
    ball.updateDynamics(dt(), width(), height(), balls, elapsedTime());
  }

  void drawBackground(){
    setColor(vec3(0.263, 0.314, 0.345));
    renderer.push();
    renderer.translate(center - vec3(0,0,100));
    renderer.scale(vec3(width(), height(), 1));
    renderer.cube();
    renderer.pop();
  }

  bool ballOverlaps(const vec3 pos){
    for(Ball& ball : balls){
      float threshold = ball.radius + 30;
      float distance = length(pos - ball.position);
      if(distance <= threshold){
        return true;
      }
    }
    return false;
  }

  vec3 randomBallPosition(){
    vec3 pos = vec3(agl::random() * width(), agl::random() * height(), 0);
    while(ballOverlaps(pos) && pos[1] > height() * 0.5){
      pos = vec3(agl::random() * width(), agl::random() * height(), 0);
    }
    return pos;
  }

  float random(){
    return ((float) rand()) / ((float) RAND_MAX);
  }

  Ball spawnBall(){
    Ball ball = Ball(vec3(random() * width(), random() * height(), 0), vec3((random() * maxInitialXVelocity * 2)-maxInitialXVelocity, 0, 0), palette[rand() % palette.size()]);
    balls.push_back(ball);
    return ball;
  }

  void gravityShift(){
    int side = rand() % 4;
    vec3 gravity;
    float dynamicsMultiplier = balls[0].dynamicsMultiplier;
    if(side == 0){
      gravity = vec3(0,-9.8,0);
    }
    else if(side == 1){
      gravity = vec3(-9.8,0,0);
    }
    else if(side == 2){
      gravity = vec3(0,9.8,0);
    }
    else if(side == 3){
      gravity = vec3(9.8,0,0);
    }
    for(Ball& ball : balls){
      ball.acceleration = gravity * dynamicsMultiplier;
    }
  }

  void reset(){
    balls = {};
    setup();
  }

  void keyUp(int key, int mods){
      // Space pressed
      if(key == GLFW_KEY_R){
         reset();
      }
      else if(key == GLFW_KEY_G){
        gravityShift();
      }
   }
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}

