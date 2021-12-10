#include "behaviors.h"
#include "steerable.h"
#include <cmath>
#include <time.h>
#include <chrono>

using namespace glm;
using namespace atk;

//--------------------------------------------------------------
// Shared properties for all behaviors
ABehavior::ABehavior(const char* name) : _name(name)
{
   // TODO: set good values
   setParam("MaxSpeed", 100);
   setParam("AgentRadius", 50);
}

//--------------------------------------------------------------
// Seek behavior

ASeek::ASeek() : ABehavior("Seek") 
{ 
}

//
// Steer the actor TOWARDS a target at max speed
//
// @param actor: steerable character to move
// @return desired velocity
//
// @note: call actor.getPosition to get teh actor's position
// @note: call getParam("MaxSpeed") to get the max speed
vec3 ASeek::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   if(actor.unique){
      setParam("MaxSpeed", 150 * actor.animationSpeed);
   }
   vec3 direction = normalize(target - actor.getPosition());
   float distance = length(target - actor.getPosition());
   float magnitude = getParam("MaxSpeed");
   vec3 velocity = direction * magnitude;
   if(distance <= getParam("AgentRadius")){
      return vec3(0);
   }
   return velocity;
}

//--------------------------------------------------------------
// Flee behavior

AFlee::AFlee() : ABehavior("Flee") 
{
}

//
// Steer the actor AWAY from a target at max speed
//
// @param actor: steerable character to move
// @return desired velocity
//
// @note: call actor.getPosition to get teh actor's position
// @note: call getParam("MaxSpeed") to get the max speed
vec3 AFlee::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{
   vec3 direction = normalize(targetPos - actor.getPosition());
   float magnitude = getParam("MaxSpeed");
   vec3 velocity = direction * magnitude;
   return -1.0f * velocity;
}

//--------------------------------------------------------------
// Arrival behavior

AArrival::AArrival() : ABehavior("Arrival") 
{
   // TODO: Set good parameters
   setParam("kArrival", 1);
   setParam("TargetRadius", 100);
}

//
// Returns a velocity whose speed is proportional to the actors distance
// from the target
//
// @param actor: steerable character to move
// @return desired velocity
vec3 AArrival::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{
   vec3 target_offset = targetPos - actor.getPosition();
   float distance = length(target_offset);
   float ramped_speed = getParam("MaxSpeed") * (distance / (getParam("TargetRadius") + getParam("AgentRadius")));
   float clipped_speed = min(ramped_speed, getParam("MaxSpeed"));
   vec3 desired_velocity = (clipped_speed / distance) * target_offset;
   if(distance <= getParam("AgentRadius")){
      return vec3(0);
   }
   return desired_velocity;
}

//--------------------------------------------------------------
// Departure behavior

ADeparture::ADeparture() : ABehavior("Departure") 
{
   setParam("InnerRadius", 1);
   setParam("OuterRadius", 1);
   setParam("kDeparture", 1);
}

// Calculate a repelent velocity based on the actor's 
// distance from the target
vec3 ADeparture::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{
   vec3 target_offset = -1.0f * (targetPos - actor.getPosition());
   float distance = length(target_offset);
   float ramped_speed = getParam("MaxSpeed") * (distance / (getParam("TargetRadius") + getParam("AgentRadius")));
   float clipped_speed = max(min(ramped_speed, getParam("MaxSpeed")),10.0f);
   vec3 desired_velocity = (clipped_speed / distance) * target_offset;
   if(length(desired_velocity) < 10.0f){
      desired_velocity = (clipped_speed / distance) * vec3(10,0,0);
   }
   return desired_velocity;
}

//--------------------------------------------------------------
// Avoid behavior

AAvoid::AAvoid() : ABehavior("Avoid") 
{
   setParam("kAvoid", 1);
}

// If an actor is near an obstacle, avoid adds either a tangential or
// normal response velocity
//  Obstacles are in getWorld()->getObstacle(i) and have class type AObstacle

bool obstacleOfConcern(vec3 sightline, AObstacle obstacle, float AgentRadius, vec3 actorPos){
   float obstacleRadius = obstacle.radius + 50;
   return length(obstacle.position - sightline) <= (obstacleRadius + AgentRadius) || length(obstacle.position - (sightline / 2.0f)) <= obstacleRadius + AgentRadius || length(obstacle.position - (sightline / 4.0f)) <= obstacleRadius + AgentRadius || length(obstacle.position - actorPos) <= obstacleRadius + AgentRadius;
}

vec3 AAvoid::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{
   vec3 actorPos = actor.getPosition();
   vec3 direction = normalize(targetPos - actorPos);
   float distance = length(targetPos - actorPos);
   float magnitude = getParam("MaxSpeed");
   vec3 baseVelocity = direction * magnitude;
   if(distance <= getParam("AgentRadius")){
      return vec3(0);
   }

   if(world.getNumObstacles() == 0){
      return baseVelocity;
   }
   
   vec3 sightline = actorPos + normalize(baseVelocity) * (length(baseVelocity) / getParam("MaxSpeed"));
   std::vector<AObstacle> obstaclesOfConcern;

   for(int i = 0; i < world.getNumObstacles(); i++){
      if(obstacleOfConcern(sightline, world.getObstacle(i), getParam("AgentRadius"), actorPos)){
         obstaclesOfConcern.push_back(world.getObstacle(i));
      }
   }
   if(obstaclesOfConcern.size() == 0){
      return baseVelocity;
   }

   AObstacle closestObstacleOfConcern = obstaclesOfConcern[0];
   for(int i = 0; i < obstaclesOfConcern.size(); i++){
      if(length(obstaclesOfConcern[i].position - actorPos) - obstaclesOfConcern[i].radius < length(closestObstacleOfConcern.position - actorPos) - closestObstacleOfConcern.radius){
         closestObstacleOfConcern = obstaclesOfConcern[i];
      }
   }

   vec3 deterrenceDirection = normalize(sightline - closestObstacleOfConcern.position);
   float deterrenceMagnitude = 50.0f;
   vec3 deterrence = deterrenceDirection * deterrenceMagnitude;

   std::cout << deterrence << std::endl;

   return baseVelocity + deterrence;
}

//--------------------------------------------------------------
// Wander behavior

AWander::AWander() : ABehavior("Wander")
{
   setParam("kWander", 1);
}
// Credit: Alessandro Pezzato https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// Wander returns a velocity whose direction changes randomly (and smoothly)
vec3 AWander::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   int t = timeSinceEpochMillisec();
   float theta = (db::perlin((t + phase)/5000.0f) * pi<float>() * 2.0f) - (pi<float>());
   vec3 direction = vec3(sin(theta), 0, cos(theta));
   std::cout << theta << std::endl;
   float magnitude = getParam("MaxSpeed");
   return direction * magnitude;
}

//--------------------------------------------------------------
// Separation behavior

ASeparation::ASeparation() : ABehavior("Separation")
{
   setParam("Neighborhood", 1);
   setParam("kSeparation", 1);
}


// Separation tries to maintain a constant distance between all agents
// within the neighborhood of the agent
// getWorld()->getAgent(i) returns the ith agent in the environment
// you can check whether two agents are the same with ==
vec3 ASeparation::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   return vec3(0,0,0);
}


//--------------------------------------------------------------
// Cohesion behavior

ACohesion::ACohesion() : ABehavior("Cohesion")
{
   setParam("Neighborhood", 1);
   setParam("kCohesion", 1);
}

// Cohesion moves actors towards the center of a group of agents
vec3 ACohesion::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
    return vec3(0,0,0);
}

//--------------------------------------------------------------
// Alignment behavior
AAlignment::AAlignment() : ABehavior("Alignment")
{
   setParam("Neighborhood", 1);
   setParam("kAlignment", 1);
}

// Alignment returns an average velocity of all active agents
vec3 AAlignment::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
    return vec3(0);
}

//--------------------------------------------------------------
// Flocking behavior
AFlocking::AFlocking() : ABehavior("Flocking")
{
}

// Flocking combines separation, cohesion, and alignment
vec3 AFlocking::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   return vec3(0,0,0);
}

//--------------------------------------------------------------
// Leader
ALeader::ALeader() : ABehavior("Leader")
{
   setParam("CSeparation", 1);
   setParam("CCohesion", 1);
   setParam("CAlignment", 1);
   setParam("CArrival", 1);
}

// You need to find the leader, who is always the first agent in agents
// If the actor is the leader, move towards the target; otherwise, 
// follow the leader as a flock
vec3 ALeader::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   return vec3(0,0,0);
}


