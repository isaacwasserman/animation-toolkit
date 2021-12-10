#include "steerable.h"
#include "behaviors.h"

using namespace glm;
using namespace atk;

float ASteerable::kVelKv = 16; 
float ASteerable::kOriKv = 16;  
float ASteerable::kOriKp = 64;

// Given a desired velocity, veld, and dt, compute a transform holding 
// the new orientation and change in position
//
// rotation control: tau = I * (-Kv1 * thetaDot + Kp1 * (thetaD - theta))
// translation control: f = m * Kv0 * (vd - v)
void ASteerable::senseControlAct(const vec3& veld, float dt)
{
   // Compute _vd and _thetad
   _vd = length(veld);
   _thetad = atan2(veld[0], veld[2]);

   // std::cout << to_string(veld) << std::endl;

   float position = _state[POS];
   float speed = _state[VEL];
   float orientation = _state[ORI];
   float angularVelocity = _state[AVEL];

   // compute _force and _torque
   _force = _mass * kVelKv * (_vd - speed);

   float angle = _thetad - orientation;
   if(angle > 180){
      angle -= 360;
   } else if(angle < -180){
      angle += 360;
   }
   _torque = _inertia * (kOriKp * angle - kOriKv * _derivative[ORI]);

   // find derivative
   float posDeriv = speed;
   float velDeriv = _force / _mass;
   float oriDeriv = angularVelocity;
   float avelDeriv = _torque / _inertia;

   _derivative[POS] = posDeriv;
   _derivative[VEL] = velDeriv;
   _derivative[ORI] = oriDeriv;
   _derivative[AVEL] = avelDeriv;

   // update state
   _state[POS] = position + dt*speed;
   _state[VEL] = speed + dt*velDeriv;
   _state[ORI] = orientation + dt*oriDeriv;
   _state[AVEL] = angularVelocity + dt*avelDeriv;

   // compute global position and orientation and update _characterRoot
   quat rot = glm::angleAxis(_state[ORI], vec3(0,1,0));
   vec3 localPos(0,0,_state[POS]);

   _characterRoot.setT(rot * localPos + _characterRoot.t());
   _characterRoot.setR(rot); 
}

// randomize the colors, characters, and animation times here
void ASteerable::randomizeAppearance()
{
   // to randomize the walking animations, compute different initial values 
   // for _time

   // to randomize color, call _drawer.setColor

   // to randomize shape, compute random values for _drawer.setJointRadius
   // or randomly assign different drawers to have a mix of characters
}

