#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include "interpolator.h"

class Matrix {
public:
    int nRows;
    int nCols;
    std::vector<std::vector<float>> data;
    Matrix(const int& m, const int& n){
        nRows = m;
        nCols = n;
        for(int i=0; i<m; i++){
            std::vector<float> row;
            for(int j=0; j<n; j++){
                row.push_back(0);
            }
            data.push_back(row);
        }
    }
    Matrix(const std::vector<std::vector<float>>& v){
        data = v;
        nRows = v.size();
        nCols = v[0].size();
    }

    Matrix operator*(const float& c){
        Matrix m = Matrix(data);
        for(std::vector<float>& row : m.data){
            for(float& element : row){
                element *= c;
            }
        }
        return m;
    }

    std::string to_string(){
        std::string output = "";
        for(const std::vector<float>& row : data){
            output += "| ";
            for(const float& element : row){
                output += std::to_string(element) + " ";
            }
            output+= "|\n";
        }
        return output;
    }
};

//--------------------------------
// Hermite
//--------------------------------

class InterpolatorHermite : public Interpolator {
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1, 0, 0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const {
        // todo: your code here
        return glm::vec3(0);
    }

    virtual void computeControlPoints(const std::vector<glm::vec3> &keys) {
        std::vector<std::vector<float>> A_data;
        std::vector<std::vector<float>> P_data;
        for(int i=0; i<keys.size()-2; i++) {
            std::vector<float> row;
            for(int k=0; k<i; k++){
                row.push_back(0);
            }
            row.push_back(-2);
            row.push_back(-8);
            row.push_back(-2);
            while(row.size() < keys.size()){
                row.push_back(0);
            }
            A_data.push_back(row);
            P_data.push_back({6 * (keys[i][0]-keys[i+2][0]), 6 * (keys[i][1]-keys[i+2][1]), 6 * (keys[i][2]-keys[i+2][2])});
        }
        Matrix A = Matrix(A_data);
        Matrix P = Matrix(P_data);
        std::cout << P.to_string() << std::endl;

        Matrix P_prime = Matrix(keys.size(),3);
        // glm::vec3 b_1 = ((1f/3f) * vector_to_vec3(P_prime.data[i])) + b_0;
        // glm::vec3 b_2 = ((-1f/3f) * vector_to_vec3(P_prime.data[i+1])) + b_3;
    }

    glm:vec3 vector_to_vec3(const std::vector<float>& v){
        return glm::vec3(v[0],v[1],v[2]);
    }

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3 &v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = true;
    glm::vec3 mClampDir;
};

#endif
