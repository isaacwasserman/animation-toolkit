#include <iostream>
#include <deque>
#include<cmath>
#include "atkui/framework.h"
#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include <Eigen/Dense>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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

std::vector<std::vector<int>> eye = {{169,163,167,165,165,158,158,138,119,87,70,64,68,78,73,73,54,36,0,1,38,40,35,34,53,65,87,108,96,89,108,126},{158,163,156,131,90,76,46,0,1,0,3,2,0,0,0,3,0,1,0,0,36,27,30,56,46,43,59,71,56,74,73,89},{157,142,100,46,0,0,3,0,0,0,1,1,0,1,1,0,4,0,2,2,0,0,1,30,0,43,76,76,38,62,87,106},{129,105,59,5,0,0,0,2,0,0,0,0,1,1,3,0,0,0,0,0,0,4,3,38,0,5,44,74,48,38,46,65},{71,72,57,0,0,0,0,0,1,2,0,1,0,0,0,1,1,2,2,0,2,1,0,1,0,23,2,14,44,44,32,38},{58,42,38,3,0,1,2,0,1,0,0,0,2,1,0,0,0,0,1,2,0,1,3,0,0,0,0,17,41,52,20,38},{25,0,8,0,0,0,0,0,0,0,3,0,0,3,0,2,1,31,44,58,59,60,55,70,73,100,103,94,94,107,97,96},{0,0,23,0,1,0,0,0,1,0,0,0,19,46,54,64,69,62,65,65,76,83,96,109,113,117,119,119,134,138,141,136},{10,37,37,1,4,35,34,35,35,31,47,54,52,65,66,69,72,77,76,83,89,92,101,103,110,123,132,136,148,158,155,153},{59,59,56,66,67,65,58,62,61,62,51,58,59,65,65,79,94,99,111,122,116,127,129,120,126,130,130,138,147,154,154,163},{83,83,85,77,74,59,59,49,34,4,1,0,4,4,0,39,57,71,79,80,84,83,92,105,122,136,146,143,148,150,162,160},{97,90,84,73,62,54,0,2,1,3,1,1,18,52,65,65,88,108,134,144,138,121,79,94,92,74,97,133,151,150,151,161},{94,85,71,62,13,0,0,2,0,2,15,22,0,7,2,0,2,0,0,31,38,51,101,169,152,137,98,60,101,139,150,159},{84,64,46,0,2,4,0,0,0,2,0,1,7,0,0,4,0,44,1,0,0,0,0,58,117,137,123,129,71,81,134,155},{73,56,23,0,0,1,0,2,4,0,1,0,0,0,2,0,44,184,191,111,0,87,29,0,2,26,117,156,154,42,97,141},{62,36,0,2,16,0,2,0,30,80,0,0,1,1,0,0,2,217,254,115,0,161,194,118,1,0,26,98,163,135,49,113},{53,13,24,26,0,0,1,47,106,102,0,55,60,0,0,0,0,76,136,0,4,158,223,209,185,88,2,4,83,145,129,104},{51,47,37,2,0,1,78,105,117,111,12,103,115,2,0,1,0,2,0,0,0,178,219,210,207,203,90,0,2,78,132,135},{85,79,27,0,27,54,92,104,117,121,60,78,122,57,1,0,4,0,75,33,76,210,212,209,206,216,176,86,44,1,79,119},{104,91,153,129,56,35,80,102,118,119,107,15,96,111,59,43,89,92,54,0,170,227,212,216,215,203,153,144,144,101,81,126},{117,75,109,161,101,0,56,86,109,117,118,91,31,90,85,61,97,77,39,138,218,217,216,214,208,158,119,130,136,157,133,137},{132,119,73,70,74,50,47,71,89,101,105,111,95,55,36,54,64,75,165,215,216,216,217,191,155,135,139,138,150,156,162,160},{141,121,108,103,96,84,73,78,98,99,107,98,96,101,99,108,138,183,204,205,199,199,171,148,146,146,126,138,147,179,175,171},{147,126,107,98,91,86,82,79,89,94,96,103,114,117,116,122,143,162,162,153,157,139,147,129,110,100,128,163,177,174,187,183},{152,124,107,98,85,82,82,74,74,71,73,87,85,95,101,117,140,138,108,122,105,107,112,102,114,122,143,170,184,185,186,192},{168,143,122,109,99,97,92,79,76,69,77,71,62,69,61,79,111,106,91,101,99,106,112,111,139,146,160,176,186,188,191,195},{176,159,139,122,115,116,111,100,85,82,77,72,78,87,88,94,103,98,99,105,117,127,132,139,149,159,172,183,190,194,197,198},{181,174,157,138,127,123,121,119,112,107,91,86,99,101,102,98,115,116,120,119,132,138,139,146,155,167,179,187,191,194,195,195},{184,182,171,155,140,129,126,129,123,117,118,109,120,121,111,114,112,124,126,129,140,149,153,159,166,174,183,189,192,194,197,200},{184,184,179,170,157,141,132,131,127,126,129,127,135,131,127,128,131,145,142,145,149,157,162,167,172,177,185,191,194,196,200,205},{188,187,187,183,173,157,142,134,131,135,129,135,141,139,149,144,145,153,153,156,160,165,171,178,182,186,192,195,195,196,198,200},{184,186,190,191,185,173,159,148,135,134,137,137,139,148,152,154,161,162,166,169,173,174,177,185,189,193,196,196,196,198,200,201}};

class Unique : public atkui::Framework {
   public:
      Unique() : atkui::Framework(atkui::Orthographic) {
      }
      
      int resolution = 32;
      int pixel_depth = 1;
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
               cube.luminance = eye[y][x] / 255.0f;
               column.push_back(cube);
               std::cout << cube.position << std::endl;
            }
            cubes.push_back(column);
         }
      }
      virtual void scene() {
         renderer.beginShader("phong");
         renderer.setUniform("Material.specular", 0.1f, 0.1f, 0.1f);
         renderer.setUniform("Light.position", (height() / 2.0f) + (400 * sin(elapsedTime())), height()/2.0f, 25, 1.0f);
         setColor(vec3(1));
         for(std::vector<Cube> column : cubes){
            for(Cube cube : column){
               setColor(cube.color);
               vec3 euler = vec3(0,sin(elapsedTime())*(pi<float>()/2.0f)*(1 - cube.luminance),0);
               renderer.transform(rotationAroundPoint(cube.position, euler));
               drawCube(vec3(0), cube.scale);
               renderer.transform(glm::inverse(rotationAroundPoint(cube.position, euler)));
            }
         }
      }

      mat4 rotationAroundPoint(const vec3& point, const vec3& euler){
         float x1 = point[0];
         float y1 = point[1];
         float z1 = point[2];

         double t1[16] = {
            1, 0, 0, -1 * x1,
            0, 1, 0, -1 * y1,
            0, 0, 1, -1 * z1,
            0, 0, 0, 1
         };
         mat4 translation1 = glm::make_mat4(t1);

         Matrix3 rot;
         rot.fromEulerAnglesXYZ(Vector3(euler[0],euler[1],euler[2]));
         double r[16] = {
            rot.m11, rot.m12, rot.m13, 0,
            rot.m21, rot.m22, rot.m23, 0,
            rot.m31, rot.m32, rot.m33, 0,
            0,       0,       0,       1
         };
         mat4 rotation = glm::make_mat4(r);

         double t2[16] = {
            1, 0, 0, x1,
            0, 1, 0, y1,
            0, 0, 1, z1,
            0, 0, 0, 1
         };
         mat4 translation2 = glm::make_mat4(t2);

         double testm[16] = {
            1, 0, 0, 0,
            0, 1, 0, 50,
            0, 0, 1, 0,
            0, 0, 0, 1
         };
         mat4 test = glm::make_mat4(testm);

         mat4 test2 = glm::translate(glm::mat4(), vec3(10,10,10));
         
         Quaternion q;
         q.fromMatrix(rot);

         Vector3 axis;
         double angle;
         q.toAxisAngle(axis,angle);

         mat4 transformation = transpose(translation2) * transpose(rotation);

         return transformation;
      }
};


int main(int argc, char **argv) {
  Unique viewer;
  viewer.run();
}