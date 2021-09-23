#include "atkui/framework.h"
#include <math.h>
#include <map>
using namespace glm;

class Curve {
  public:
    std::vector<glm::vec3> control_points;
    std::vector<float> range = {0,1};
    vec3 color = vec3(1,1,1);
    Curve(std::vector<glm::vec3> control_pts, float t_0, float t_1) {
      control_points = control_pts;
      range = {t_0, t_1};
    }
    Curve(std::vector<glm::vec3> control_pts) {
      control_points = control_pts;
    }
    Curve(std::vector<glm::vec3> control_pts, vec3 c) {
      control_points = control_pts;
      color = c;
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

  Curve start_curve = Curve({vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0)}, agl::randomUnitVector());
  Curve end_curve = Curve({vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0)}, agl::randomUnitVector());
  Curve animated_curve = start_curve;
  std::list<Curve> past_curves;
  float lastSaveTime = 0;
  float duration = 1;
  float t = 0;
  std::vector<std::vector<vec3>> lines;
  int max_trail = 50;

  void setup() {
  }

  int resolution = 20;

  void scene() {
    t += dt();
    if(t > 1){
      t = 0;
      start_curve = end_curve;
      animated_curve = start_curve;
      end_curve = Curve({vec3(agl::random() * width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0), vec3(agl::random()* width(), agl::random()* height(), 0)}, agl::randomUnitVector());
    }

    if(elapsedTime() - lastSaveTime >= 0.1){
      past_curves.push_back(animated_curve);
      if(past_curves.size() > max_trail){
        past_curves.pop_front();
      }
      lastSaveTime = elapsedTime();
    }

    Curve p0_curve = Curve({start_curve.control_points[0], end_curve.control_points[0]});
    Curve p1_curve = Curve({start_curve.control_points[1], end_curve.control_points[1]});
    Curve p2_curve = Curve({start_curve.control_points[2], end_curve.control_points[2]});
    Curve p3_curve = Curve({start_curve.control_points[3], end_curve.control_points[3]});
    Curve color_curve = Curve({start_curve.color, end_curve.color});
    std::vector<Curve> animation_curves = {p0_curve, p1_curve, p2_curve, p3_curve};
    for(int i=0; i<animation_curves.size(); i++){
      animated_curve.control_points[i] = animation_curves[i].compute(t);
    }
    animated_curve.color = color_curve.compute(t);
    std::vector<std::vector<vec3>> start_lines = start_curve.trace(resolution);
    std::vector<std::vector<vec3>> end_lines = end_curve.trace(resolution);
    std::vector<std::vector<vec3>> animated_lines = animated_curve.trace(resolution);
    for(int i=0; i<start_lines.size(); i++){
      setColor(start_curve.color);
      drawLine(start_lines[i][0],start_lines[i][1]);
    }
    // for(int i=0; i<end_lines.size(); i++){
    //   setColor(end_curve.color);
    //   drawLine(end_lines[i][0],end_lines[i][1]);
    // }
    for(int i=0; i<animated_lines.size(); i++){
      setColor(animated_curve.color);
      drawLine(animated_lines[i][0],animated_lines[i][1]);
    }
    for(int i=0; i<past_curves.size(); i++){
      std::vector<Curve> past_curves_v{ std::begin(past_curves), std::end(past_curves) };
      std::vector<std::vector<vec3>> trail_lines = past_curves_v[i].trace(resolution);
      setColor(past_curves_v[i].color);
      for(int k=0; k<trail_lines.size(); k++){
        drawLine(trail_lines[k][0],trail_lines[k][1]);
      }
    }
  }

  vec3 lerp(const vec3& start, const vec3& end, const float& t){
    vec3 delta = end - start;
    return delta * t;
  }
};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

