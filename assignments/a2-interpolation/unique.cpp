#include <iostream>
#include "atkui/framework.h"
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

class Unique : public atkui::Framework {
   public:
      Unique() : atkui::Framework(atkui::Orthographic) {
      }
      float t = 0;
      Curve path_curve = Curve({vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0)});
      Curve color_curve = Curve({agl::randomUnitVector(),agl::randomUnitVector(),agl::randomUnitVector()});
      virtual void setup(){
      }
      virtual void scene() {
         t += dt();
         if(t > 1){
            t = 0;
            path_curve = randomCurve(path_curve.control_points.back());
            color_curve = Curve({color_curve.control_points.back(),agl::randomUnitVector(),agl::randomUnitVector()});
         }
         setColor(vec3(1,1,1));
         drawSphere(path_curve.compute(t), 10);
         for(std::vector<vec3> line : path_curve.trace(20)){
            // drawLine(line[0],line[1]);
         }
         setColor(color_curve.compute(t));
         drawCube(vec3(width()*0.5,height()*0.5,-100), vec3(width(),height(),0));
         rotate(pi<float>()*t, path_curve.compute(t));
         setColor(vec3(1,1,1));
         drawCube(path_curve.compute(t), vec3(50,50,50));
      }

      Curve randomCurve(const vec3& start_point){
         std::vector<vec3> points = {start_point, vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0)};
         return Curve(points);
      }
      Curve randomCurve(){
         std::vector<vec3> points = {vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0), vec3(agl::random() * width(), agl::random() * height(), 0)};
         return Curve(points);
      }
      vec3 getVelocity(Curve& curve, const float& t){
         float interval = 0.01;
         vec3 p_i = curve.compute(t);
         vec3 p_i_plus = curve.compute(t+interval);
         vec3 diff = p_i_plus - p_i;
         vec3 unitDiff = diff / length(diff);
         return unitDiff;
      }
};

int main(int argc, char **argv) {
  Unique viewer;
  viewer.run();
}
