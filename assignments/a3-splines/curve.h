#include <string>
#include <vector>
#include <iostream>

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
        std::vector<glm::vec3> line = {compute(step*i), compute(step*(i+1))};
        lines.push_back(line);
      }
      return lines;
    }
};