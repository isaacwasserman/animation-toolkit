#include <iostream>
#include <deque>
#include<cmath>
#include "atkui/framework.h"
#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
using namespace glm;
using atkmath::Vector3;
using atkmath::Quaternion;
using atkmath::Matrix3;

struct Cube {
   vec3 position;
   vec3 color;
   vec3 scale;
   float luminance;
};

class Unique : public atkui::Framework {
   public:
      Unique() : atkui::Framework(atkui::Orthographic) {
      }
      
      int resolution = 10;
      int pixel_depth = 10;
      vec3 cube_scale = vec3(width() / ((float) resolution), height() / ((float) resolution), pixel_depth);
      vec3 base_position = vec3(0,height(),0) - vec3(cube_scale[0] / -2.0f, cube_scale[1] / 2.0f, 0);
      std::vector<std::vector<Cube>> cubes;

      virtual void setup(){
         // For column
         for(int x=0; x<resolution; x++){
            std::vector<Cube> column;
            // For item
            for(int y=0; y<resolution; y++){
               Cube cube;
               cube.scale = cube_scale;
               cube.position = base_position + vec3(cube_scale[0] * x, cube_scale[1] * y * -1, 0);
               cube.color = vec3(1);
               cube.luminance = 0.8;
               column.push_back(cube);
               std::cout << cube.position << std::endl;
            }
            cubes.push_back(column);
         }
      }
      virtual void scene() {
         // renderer.beginShader("phong");
         setColor(vec3(1));
         for(std::vector<Cube> column : cubes){
            for(Cube cube : column){
               setColor(cube.color);
               Matrix3 m;
               m.fromEulerAnglesXYZ(Vector3(0,(pi<float>()/2.0f)*(1 - cube.luminance),0));
               Quaternion q;
               q.fromMatrix(m);
               rotate(q.w(), vec3(q.x(),q.y(),q.z()));
               drawCube(cube.position, cube.scale);
            }
         }
         // drawCube(vec3(125),vec3(250));
      }
};

int main(int argc, char **argv) {
  Unique viewer;
  viewer.run();
}