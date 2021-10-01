#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include "interpolator.h"

//--------------------------------
// Hermite
//--------------------------------

class InterpolatorHermite : public Interpolator {
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1, 0, 0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const {
        glm::vec3 b_0 = mCtrlPoints[segment * 2];
        glm::vec3 p_0_prime = mCtrlPoints[(segment * 2) + 1];
        glm::vec3 b_3 = mCtrlPoints[(segment * 2) + 2];
        glm::vec3 p_1_prime = mCtrlPoints[(segment * 2) + 3];

        glm::vec3 b_1 = ((1.0f/3.0f) * p_0_prime) + b_0;
        glm::vec3 b_2 = ((-1.0f/3.0f) * p_1_prime) + b_3;

        Curve segment_curve = Curve({b_0,b_1,b_2,b_3});

        return segment_curve.compute(u);
    }

    virtual void computeControlPoints(const std::vector<glm::vec3> &keys) {
        Eigen::MatrixXd A(keys.size(), keys.size());
        A.setZero(keys.size(), keys.size());
        Eigen::MatrixXd P(keys.size(), 3);
        P.setZero(keys.size(), 3);

        for(int i=1; i<keys.size()-1; i++) {
            int column_index = 0;
            for(int k=1; k<i; k++){
                A(i,column_index) = 0;
                column_index++;
            }
            A(i,column_index) = -2; column_index++;
            A(i,column_index) = -8; column_index++;
            A(i,column_index) = -2; column_index++;
            while(column_index < keys.size()){
                A(i,column_index) = 0;
                column_index++;
            }
        }
        for(int i=1; i<keys.size()-1; i++) {
            P(i,0) = 6 * (keys[i-1][0]-keys[i+1][0]);
            P(i,1) = 6 * (keys[i-1][1]-keys[i+1][1]);
            P(i,2) = 6 * (keys[i-1][2]-keys[i+1][2]);
        }
        P = -1 * P;
        // std::cout << "earlier P:\n" << P << std::endl;
        if(isClamped()){
            glm::vec3 c = mClampDir;
            P(0,0) = c[0]; P(0,1) = c[1]; P(0,2) = c[2];
            P(P.rows() - 1,0) = c[0]; P(P.rows() - 1,1) = c[1]; P(P.rows() - 1,2) = c[2];
            A(0,0) = 1;
            A(A.rows()-1, A.cols()-1) = 1;
        }
        else {
            glm::vec3 top_endpoint = 6.0f * (keys[1] - keys[0]);
            glm::vec3 bottom_endpoint = 6.0f * (keys[keys.size()-1] - keys[keys.size()-2]);
            P(0,0) = top_endpoint[0]; P(0,1) = top_endpoint[1]; P(0,2) = top_endpoint[2];
            P(P.rows()-1,0) = bottom_endpoint[0]; P(P.rows()-1,1) = bottom_endpoint[1]; P(P.rows()-1,2) = bottom_endpoint[2];
            A(0,0) = -4; A(0,1) = -2;
            A(A.rows()-1,A.cols()-2) = -2; A(A.rows()-1,A.cols()-1) = -4;
        }
        A = (-1.0f/2.0f) * A;
        P = (1.0f/2.0f) * P;
        // std::cout << "P:\n" << P << std::endl;
        Eigen::MatrixXd P_prime = A.inverse() * P;

        // std::cout << "P_prime:\n" << P_prime << std::endl;
        // std::cout << "A:\n" << A << std::endl;

        int nSegments = keys.size() - 1;
        mCtrlPoints = {};
        for(int i=0; i<nSegments; i++){
            mCtrlPoints.push_back(keys[i]);
            glm::vec3 p_prime_1 = glm::vec3(P_prime(i,0),P_prime(i,1),P_prime(i,2));
            glm::vec3 b_1 = ((1.0f/3.0f) * p_prime_1) + keys[i];
            mCtrlPoints.push_back(p_prime_1);
            if(i == nSegments - 1){
                glm::vec3 p_prime_2 = glm::vec3(P_prime(i+1,0),P_prime(i+1,1),P_prime(i+1,2));
                glm::vec3 b_2 = ((-1.0f/3.0f) * p_prime_2) + keys[i+1];
                mCtrlPoints.push_back(keys[i+1]);
                mCtrlPoints.push_back(p_prime_2);
            }
        }
        // std::cout << std::endl;
    }

    glm::vec3 vector_to_vec3(const std::vector<float>& v){
        return glm::vec3(v[0],v[1],v[2]);
    }

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3 &v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = false;
    glm::vec3 mClampDir;
};

#endif
