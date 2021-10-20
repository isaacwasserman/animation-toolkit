#include <iostream>
#include <deque>
#include<cmath>
#include "atkui/framework.h"
#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include <Eigen/Dense>
#include <glm/gtc/type_ptr.hpp>

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
         // setColor(vec3(1));
         // for(std::vector<Cube> column : cubes){
         //    for(Cube cube : column){
         //       setColor(cube.color);
         //       // Matrix3 m;
         //       // m.fromEulerAnglesXYZ(Vector3(0,(pi<float>()/2.0f)*(1 - cube.luminance),0));
         //       // Quaternion q;
         //       // q.fromMatrix(m);
         //       vec3 euler = vec3(0,(pi<float>()/2.0f)*(1 - cube.luminance),0);
         //       renderer.transform(rotationAroundPoint(cube.position, euler));
         //       drawCube(vec3(0), cube.scale);
         //    }
         // }
         setColor(vec3(1));
         vec3 euler = vec3(0,(fmod(elapsedTime(), 2)/2.0f)*(pi<float>()/2.0f),0);
         renderer.transform(rotationAroundPoint(vec3(width()/2,height()/2,0), euler));
         drawCube(vec3(width()/2,height()/2,0), vec3(50));
      }

      mat4 rotationAroundPoint(const vec3& point, const vec3& euler){
         float x1 = point[0];
         float y1 = point[1];
         float z1 = point[2];

         float t1[16] = {
            1, 0, 0, x1,
            0, 1, 0, y1,
            0, 0, 1, z1,
            0, 0, 0, 1
         };
         mat4 translation1 = glm::make_mat4(t1);

         Matrix3 rot;
         rot.fromEulerAnglesXYZ(Vector3(euler[0],euler[1],euler[2]));
         float r[16] = {
            rot.m11, rot.m12, rot.m13, 0,
            rot.m21, rot.m22, rot.m23, 0,
            rot.m31, rot.m32, rot.m33, 0,
            0,       0,       0,       1
         };
         mat4 rotation = glm::make_mat4(r);

         float t2[16] = {
            1, 0, 0, -1 * x1,
            0, 1, 0, -1 * y1,
            0, 0, 1, -1 * z1,
            0, 0, 0, 1
         };
         mat4 translation2 = glm::make_mat4(t2);

         mat4 transformation = translation1;

         return transformation;
      }
};

int main(int argc, char **argv) {
  Unique viewer;
  viewer.run();
}