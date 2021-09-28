#ifndef _interpolator_linear_H_
#define _interpolator_linear_H_

#include <string>
#include <vector>
#include <iostream>
#include "interpolator.h"

//--------------------------------
// Linear 
//--------------------------------

class InterpolatorLinear : public Interpolator
{
public:
    InterpolatorLinear() : Interpolator("Linear") {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       int offset = segment;
       std::vector<glm::vec3> segmentControlPoints = {mCtrlPoints[offset],mCtrlPoints[offset+1]};
       glm::vec3 totalDisplacement = segmentControlPoints[1] - segmentControlPoints[0];
       glm::vec3 relativeDisplacement = (float)(u/1.0) * totalDisplacement;
       glm::vec3 interpolatedPosition = segmentControlPoints[0] + relativeDisplacement;
       return interpolatedPosition;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       if(keys.size() >= 2){
         for(int i=0;i<keys.size();i++){
            mCtrlPoints.push_back(keys[i]);
         }
       }
    }
};

#endif
