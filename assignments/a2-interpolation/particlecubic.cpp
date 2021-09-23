#include "atkui/framework.h"
#include <math.h>
using namespace glm;

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
    vec3 compute(float param, std::vector<glm::vec3> control_pts) {
      std::vector<glm::vec3> new_pts;
      float t = param / range[1];
      for(int i=0; i<control_pts.size()-1; i++){
        vec3 pt = (1 - t)*control_pts[i] + t * control_pts[i+1];
        new_pts.push_back(pt);
      }
      if(new_pts.size() < 2) {
        return new_pts[0];
      }
      else {
        return compute(t, new_pts);
      }
    }
    vec3 compute(float param) {
      return compute(param, control_points);
    }
    int factorial(int n){
      if(n<=1){
        return 1;
      } else {
        return n * factorial(n - 1);
      }
    }
    vec3 compute_bernstein(float param) {
      float t = param / range[1];
      vec3 sum = vec3(0);
      int n = control_points.size() - 1;
      for(int i=0; i<=n; i++){
        vec3 b = control_points[i];
        float nChooseI = factorial(n) / (factorial(i) * factorial(n - i));
        float otherPartOfB = std::pow(t, i) * std::pow(1 - t, n - i);
        float B = nChooseI * otherPartOfB;
        vec3 part = B * b;
        sum += part;
      }
      return sum;
    }
    std::vector<std::vector<vec3>> trace(int resolution){
      std::vector<std::vector<vec3>> lines;
      float step = 1.0f / resolution;
      for(int i=0; i<resolution; i++){
        std::vector<vec3> line = {compute(step*i), compute(step*(i+1))};
        lines.push_back(line);
      }
      return lines;
    }
    std::vector<std::vector<vec3>> trace_bernstein(int resolution){
      std::vector<std::vector<vec3>> lines;
      float step = 1.0f / resolution;
      for(int i=0; i<resolution; i++){
        std::vector<vec3> line = {compute_bernstein(step*i), compute_bernstein(step*(i+1))};
        lines.push_back(line);
      }
      return lines;
    }
};

class ParticleCubic : public atkui::Framework {
 public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic) {
  }

  vec3 B0 = vec3(100,  50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  Curve curve = Curve({B0,B1,B2,B3});
  int resolution = 20;
  std::vector<std::vector<vec3>> lines = curve.trace(resolution);
  float duration = 5;

  void setup() {
  }

  void scene() {
    setColor(vec3(1));
    drawSphere(curve.compute(fmod(elapsedTime(), duration) / duration), 10);
    for(const std::vector<vec3>& line : lines){
      drawLine(line[0],line[1]);
    }
  }
};

int main(int argc, char** argv) {
  ParticleCubic viewer;
  viewer.run();
}

