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
    std::vector<std::vector<vec3>> trace(){
      return trace(20);
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

class Screensaver : public atkui::Framework {
 public:
  Screensaver() : atkui::Framework(atkui::Orthographic) {
  }

  Curve start_curve = Curve({vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0)});
  Curve end_curve = Curve({vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0)});
  Curve animated_curve = start_curve;
  float duration = 1;
  float t = 0;
  std::vector<std::vector<vec3>> lines;

  void setup() {
  }

  void scene() {
    t += dt();
    if(t > 1){
      t = 0;
      start_curve = end_curve;
      animated_curve = start_curve;
      end_curve = Curve({vec3(agl::random() * width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0)});
    }
    setColor(vec3(1,1,1));
    lines = {};
    std::vector<std::vector<vec3>> start_lines = start_curve.trace();
    std::vector<std::vector<vec3>> end_lines = end_curve.trace();
    std::vector<std::vector<vec3>> animated_lines = animated_curve.trace();
    lines.insert(lines.end(), start_lines.begin(), start_lines.end());
    lines.insert(lines.end(), end_lines.begin(), end_lines.end());
    lines.insert(lines.end(), animated_lines.begin(), animated_lines.end());
    for(const std::vector<vec3>& line : lines){
      drawLine(line[0],line[1]);
    }
  }
};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

