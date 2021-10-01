#include <iostream>
#include "atkui/framework.h"
#include "spline.h"
#include "interpolator_hermite.h"
using glm::vec3;

class Unique : public atkui::Framework
{
 public:
  Unique() : atkui::Framework(atkui::Orthographic, 400, 400) {
  }

  int resolution = 20;
  std::vector<std::vector<vec3>> lines;


   std::vector<Spline> splines;
   virtual void setup(){
     int nSplines = 10;
     int nSegments = 4;
     float slopeMultiplier = 30;
     for(int i=0; i<nSplines; i++){
       Spline spline;
       InterpolatorHermite Hermite;
       spline.addInterpolationType("Hermite", &Hermite);
       spline.setInterpolationType("Hermite");
       for(int k=0; k<nSegments; k++){
         spline.appendKey(k, vec3(agl::random() * width(), agl::random() * height(), 0));
         if(k == nSegments - 1){
           spline.appendKey(k+1, vec3(agl::random() * width(), agl::random() * height(), 0));
         }
       }
       splines.push_back(spline);
     }
   }

  virtual void scene()
  {
    setColor(vec3(0,0,1));
    for(int j=0;j<splines.size();j++){
      Spline mSpline = splines[j];
      std::vector<std::vector<glm::vec3>> lines;
      // std::cout << j << std::endl;
      std::vector<glm::vec3> points = {mSpline.getKey(0)};
      for(int k=0; k<mSpline.getNumKeys()-1; k++){
        float segmentStartTime = mSpline.getTime(k);
        float segmentEndTime = mSpline.getTime(k+1);
        float tStep = (segmentEndTime - segmentStartTime) / resolution;
        float t = segmentStartTime + tStep;
        for(int i=0; i<resolution; i++){
          std::cout << t << std::endl;
          points.push_back(mSpline.getValue(t));
          // glm::vec3 p1 = points[points.size() - 2];
          // glm::vec3 p2 = points[points.size() - 1];
          // lines.push_back({p1,p2});
          // t+=tStep;
        }
      }
      setColor(glm::vec3(0,1,0));
      for(int i=0; i<lines.size(); i++){
        drawLine(lines[i][0],lines[i][1]);
      }
    }
    // std::cout << c << std::endl;
  }
};

int main(int argc, char **argv) {
  Unique viewer;
  viewer.run();
}

