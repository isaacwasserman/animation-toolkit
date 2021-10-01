#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include <iostream>
#include "interpolator.h"

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
