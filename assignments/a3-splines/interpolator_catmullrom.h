#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include <iostream>
#include "interpolator.h"

class Curve {
  public:
    std::vector<glm::vec3> control_points;
    std::vector<float> range = {0,1};
    Curve(std::vector<glm::vec3> control_pts, float t_0, float t_1) {
      control_points = control_pts;
      range = {t_0, t_1};
    }
    Curve(std::vector<glm::vec3> control_pts) {
      control_points = control_pts;
    }
    glm::vec3 compute(float param, std::vector<glm::vec3> control_pts) {
      std::vector<glm::vec3> new_pts;
      float t = param / range[1];
      for(int i=0; i<control_pts.size()-1; i++){
        glm::vec3 pt = (1 - t)*control_pts[i] + t * control_pts[i+1];
        new_pts.push_back(pt);
      }
      if(new_pts.size() < 2) {
        return new_pts[0];
      }
      else {
        return compute(t, new_pts);
      }
    }
    glm::vec3 compute(float param) {
      return compute(param, control_points);
    }
    int factorial(int n){
      if(n<=1){
        return 1;
      } else {
        return n * factorial(n - 1);
      }
    }
    std::vector<std::vector<glm::vec3>> trace(){
      return trace(20);
    }
    std::vector<std::vector<glm::vec3>> trace(int resolution){
      std::vector<std::vector<glm::vec3>> lines;
      float step = 1.0f / resolution;
      for(int i=0; i<resolution; i++){
        std::vector<glm::vec3>  line = {compute(step*i), compute(step*(i+1))};
        lines.push_back(line);
      }
      return lines;
    }
};

//--------------------------------
// Catmull-Rom 
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
    InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

    virtual glm::vec3 interpolate(int segment, double u) const {
      int offset = segment * 3;
      std::vector<glm::vec3> segmentControlPoints = {mCtrlPoints[offset], mCtrlPoints[offset + 1], mCtrlPoints[offset + 2], mCtrlPoints[offset + 3]};
      Curve bezier = Curve(segmentControlPoints);
      glm::vec3 interpolatedPosition = bezier.compute(u);
      return interpolatedPosition;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       if(keys.size() >= 2){
         for(int i=0;i<keys.size()-1;i++){
            glm::vec3 b_0,b_1,b_2;
            b_0 = keys[i];
            if(i == 0){
               b_1 = keys[i] + (1.0f/6)*(keys[1] - keys[0]);
            } else {
               b_1 = keys[i] + (1.0f/6)*(keys[i+1] - keys[i-1]);
            }
            if(i == keys.size() - 2){
               b_2 = keys[i+1] - (1.0f/6)*(keys[keys.size() - 1] - keys[keys.size() - 2]);
            } else {
               b_2 = keys[i+1] - (1.0f/6)*(keys[i+2] - keys[i]);
            }
            mCtrlPoints.push_back(b_0);
            mCtrlPoints.push_back(b_1);
            mCtrlPoints.push_back(b_2);
         }
         mCtrlPoints.push_back(keys[keys.size() - 1]);
       }
    }
};

#endif
