#include <iostream>
#include "atkui/framework.h"
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
};

class Cube {
  public:
    vec3 position;
    vec3 scale;
    vec3 color;
    Cube(vec3 pos, vec3 sca, vec3 col) {
      position = pos;
      scale = sca;
      color = col;
    }
};

class Gradient : public atkui::Framework {
 public:
  Gradient() : atkui::Framework(atkui::Perspective) {
  }

  vec3 computeColor(vec3 pos, vec2 dimensions) {
    float x = pos[0];
    float y = pos[1];
    vec3 color_nw = vec3(1,1,0);
    vec3 color_ne = vec3(0,1,1);
    vec3 color_sw = vec3(1,0,0);
    vec3 color_se = vec3(1,0,1);
    Curve north_gradient = Curve({color_nw, color_ne}, 0, dimensions[0]);
    Curve south_gradient = Curve({color_sw, color_se}, 0, dimensions[0]);
    vec3 north_color = north_gradient.compute(x);
    vec3 south_color = south_gradient.compute(x);
    Curve compound_gradient = Curve({south_color, north_color}, 0, dimensions[1]);
    vec3 target_color = compound_gradient.compute(y);
    return target_color;
  }

  int n = 20;
  Curve scale_curve = Curve({vec3(0,0,1),vec3(0,0,25),vec3(0,0,25),vec3(0,0,50)});

  virtual void scene() {
   rotate(pi<float>() / -4, vec3(0,1,0));
   rotate(pi<float>() / -4, vec3(1,0,0));
   scale(vec3(2.2,2.2,2.2));
   translate(vec3(width()*-0.5,height()*-0.5,0));
    std::vector<Cube> cubes;
    vec3 size = vec3(width()/n, height()/n, 1);
    for(int i=0; i<n; i++){
      for(int k=0; k<n; k++){
        vec3 pos = vec3((k*size[0])+(size[0]/2), (i*size[1])+(size[1]/2), 0);
        vec3 col = computeColor(pos, vec2(width(),height()));
        Cube cube = Cube(pos, size, col);
        cubes.push_back(cube);
      }
    }
    for(const Cube& cube : cubes){
      vec3 newScale = cube.scale;

      setColor(cube.color);
      drawCube(cube.position, cube.scale);
    }
  }

  void keyUp(int key, int mods){
    if(key == GLFW_KEY_UP){
      n++;
    }
    else if(key == GLFW_KEY_DOWN){
      if(n>1){
        n--;
      }
    }
  }
};

int main(int argc, char **argv) {
  Gradient viewer;
  viewer.run();
}
