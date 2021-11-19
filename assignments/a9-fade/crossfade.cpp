#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <stdio.h>
#include <string.h>

using namespace atk;
using namespace atkui;
using namespace glm;

class ACrossfadeViewer : public atkui::Framework
{
public:
  ACrossfadeViewer() : atkui::Framework(atkui::Perspective) {}
  virtual ~ACrossfadeViewer() {}

  void loadMotion1(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion1_);
  }

  void loadMotion2(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion2_);
  }

  Motion reorient(const Motion& motion, const vec3& pos, float heading)
   {
      Motion result;
      result.setFramerate(motion.getFramerate());
      
      quat R0 = motion.getKey(0).jointRots[0];
      vec3 D0 = motion.getKey(0).rootPos;
      mat4 D0_matrix = mat4(
        1,0,0,D0[0],
        0,1,0,D0[1],
        0,0,1,D0[2],
        0,0,0,1
      );
      mat4 R0_matrix = glm::mat4_cast(R0);
      mat4 T0 = D0_matrix * R0_matrix;

      quat RHeading = eulerAngleRO(XYZ, vec3(0, heading, 0));
      mat4 DPos_matrix = mat4(
        1,0,0,pos[0],
        0,1,0,pos[1],
        0,0,1,pos[2],
        0,0,0,1
      );
      mat4 RHeading_matrix = mat4_cast(RHeading);
      mat4 TOrient = DPos_matrix * RHeading_matrix;

      mat4 T = TOrient * inverse(T0);
      vec3 offsetTranslation = vec3(T[0][3],T[1][3],T[2][3]);
      mat3 offsetRotation = mat3(T);

      for(int k=0; k<motion.getNumKeys(); k++){
         Pose pose = motion.getKey(k);
         pose.jointRots[0] = quat_cast(T) * pose.jointRots[0];
         pose.rootPos = offsetTranslation + pose.rootPos;
         result.appendKey(pose);
      }
      
      return result;
   }

  void crossfade(int numBlendFrames)
  {
    assert(motion1_.getNumKeys() > 0);
    assert(motion2_.getNumKeys() > 0);
    assert(numBlendFrames > 0);
    assert(numBlendFrames <= motion1_.getNumKeys());
    assert(numBlendFrames <= motion2_.getNumKeys());

    int start1 = motion1_.getNumKeys() - numBlendFrames;
    int start2 = 0;

    motion2_ = reorient(motion2_, motion1_.getKey(motion1_.getNumKeys() - 1).rootPos, glm::eulerAngles(motion1_.getKey(motion1_.getNumKeys() - 1).jointRots[0]).y);

    blend_.setFramerate(motion1_.getFramerate());

    for(int i=0; i<motion1_.getNumKeys() - numBlendFrames; i++){
      blend_.appendKey(motion1_.getKey(i));
    }

    for(int i=0; i<numBlendFrames; i++){
      Pose p1 = motion1_.getKey(i + start1);
      Pose p2 = motion2_.getKey(i + start2);
      float u = ((float) i)/((float) numBlendFrames);
      Pose blendedPose = Pose::Lerp(p1,p2,u);
      blend_.appendKey(blendedPose);
    }
    for(int i=numBlendFrames; i<motion2_.getNumKeys(); i++){
      blend_.appendKey(motion2_.getKey(i));
    }
  }

  void save(const std::string &filename)
  {
    BVHWriter writer;
    writer.save(filename, skeleton_, blend_);
  }

  void scene()
  {
    blend_.update(skeleton_, elapsedTime());
    drawer_.draw(skeleton_, *this);
  }

private:
  Skeleton skeleton_;
  SkeletonDrawer drawer_;
  Motion motion1_;
  Motion motion2_;
  Motion blend_;
};

std::string PruneName(const std::string &name)
{
  int idx1 = name.find_last_of("/");
  int idx2 = name.find_last_of(".");
  return name.substr(idx1 + 1, idx2 - idx1 - 1);
}

int main(int argc, char **argv)
{
  std::string motion1 = "../motions/Beta/walking.bvh";
  std::string motion2 = "../motions/Beta/jump.bvh";
  int numFrames = 10;

  try
  {
    for (int i = 0; i < argc; i++)
    {
      if (strncmp(argv[i], "-m1", 3) == 0)
        motion1 = argv[i + 1];
      else if (strncmp(argv[i], "-m2", 3) == 0)
        motion2 = argv[i + 1];
      else if (strncmp(argv[i], "-nframes", 7) == 0)
        numFrames = std::stoi(argv[i + 1]);
    }
  }
  catch (const std::exception &e)
  {
    std::cout << "usage: crossfade -m1 <path> -m2 <path> -nframes <num frames>\n";
  }

  ACrossfadeViewer viewer;
  viewer.loadMotion1(motion1);
  viewer.loadMotion2(motion2);
  viewer.crossfade(numFrames);

  std::string saveName = "blend-" + PruneName(motion1) + PruneName(motion2);
  saveName += "-" + std::to_string(numFrames) + ".bvh";
  viewer.save(saveName);

  viewer.run();
  return 0;
}
