#pragma once

#include <string>
using std::string;

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

#include <vector>
using std::vector;

#include <fstream>
using std::ifstream;

#include <iostream>
using std::flush;
using std::cout;

#include "model.h"

namespace Models {
    class ObjModel : public Model{
        private: 
            template <class T>
                void load_into_vector(vector<T> &output, ifstream &fileInput, int number);
            void construct_face(
                    ifstream &inputModel, 
                    vector<float> &out_vertices,
                    vector<float> &out_texCoords, 
                    vector<float> &out_normals,
                    vector<float> &out_colors,
                    int &vertexCount,
                    vector<vec3> in_vertices,
                    vector<vec2> in_texCoords,
                    vector<vec3> in_normals,
                    float color_R,
                    float color_G,
                    float color_B,
                    float color_A
                    ); 
        public:
            ObjModel(const char* filename);
            virtual ~ObjModel();

            virtual void drawSolid();
    };
    template <class T>
        void ObjModel::load_into_vector(vector<T> &output, ifstream &fileInput, int number) {
            T out_vector; 
            for (int i = 0; i < number; i++) {
                fileInput >> out_vector[i];
            }
            output.push_back(out_vector);
        }
}

