#include <stdlib.h>

#include <fstream>
using std::ifstream;

#include <sstream>
using std::stringstream;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;


#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

#include "modelloader.h"


namespace Models {

    //Constructor for ObjModel class
    //Reads obj file and populates 
    //VertexCount and vertices, normals, texCoords,
    //and colors (currently with silver) tables
    ObjModel::ObjModel(const char* filename) {
        //TODO Load model data from obj file
        //
        ifstream inputModel;

        string currentWord; //temporary variable for currently read word
        
        //Temporary containers for vertices Texture coordinates and normals
        vector<vec3> vertices;
        vector<vec2> texCoords;
        vector<vec3> normals;


        cout << "Loading: Begin" << endl;
        inputModel.open(filename);

        vertexCount = 0;

        //Containers for real data to be loaded to proper model containers
        vector<float> v_vertices; 
        vector<float> v_vertexNormals;
        vector<float> v_texCoords;
        vector<float> v_colors;


        inputModel >> currentWord;

        //File read loop
        while (!inputModel.eof()) {
            //if line describes a normal vector add it to normals
            if (currentWord.compare("vn") == 0) { 
                vec3 input;

                inputModel >> input.x;
                inputModel >> input.y;
                inputModel >> input.z;
                normals.push_back(input);
            }
            //if line describes texture coordinate add it to texCoords
            else if (currentWord.compare("vt") == 0) { 
                vec2 input;
                inputModel >> input.x;
                inputModel >> input.y;
                texCoords.push_back(input);
            }
            //if line describes a vertex add it to vertices
            else if (currentWord.compare("v") == 0) { 
                vec3 input;
                inputModel >> input.x;
                inputModel >> input.y;
                inputModel >> input.z;
                vertices.push_back(input);
            }
            //if line describes a face add correct vertice, normal and texCoord
            else if (currentWord.compare("f") == 0) {

                for (int i = 0; i < 3; i++) {
                    inputModel >> currentWord;
                    stringstream face_stream(currentWord);

                    //Get vertice index and load it into v_Vertices
                    string face_vertice_desc;
                    getline(face_stream, face_vertice_desc, '/');
                    int vertice_index = std::stoi(face_vertice_desc) - 1;
                    v_vertices.push_back(vertices[vertice_index].x);
                    v_vertices.push_back(vertices[vertice_index].y);
                    v_vertices.push_back(vertices[vertice_index].z);
                    v_vertices.push_back(1.f);


                    //Get texCoords index and load it into v_TexCoords
                    string face_texCoords_desc;
                    getline(face_stream, face_texCoords_desc, '/');
                    int texCoords_index = std::stoi(face_texCoords_desc) - 1;
                    v_texCoords.push_back(texCoords[texCoords_index].x);
                    v_texCoords.push_back(texCoords[texCoords_index].y);

                    //Get normal index and load it into v_vertexNormals
                    string face_normal_desc;
                    getline(face_stream, face_normal_desc, '/');
                    int normal_index = std::stoi(face_normal_desc) - 1;
                    v_vertexNormals.push_back(normals[normal_index].x);
                    v_vertexNormals.push_back(normals[normal_index].y);
                    v_vertexNormals.push_back(normals[normal_index].z);
                    v_vertexNormals.push_back(0.f);

                    v_colors.push_back(0.7f);
                    v_colors.push_back(0.7f);
                    v_colors.push_back(0.7f);
                    v_colors.push_back(1.f);

                    vertexCount++;
                }

            }

            inputModel >> currentWord;
            cout << endl;
        }


        inputModel.close();
        cout << "Loading: Finished" << endl;

        //Copy dynamic vectors contents into static containers 
        //from parent Model class

        Model::vertices = new float[v_vertices.size()];
        std::copy(v_vertices.begin(), v_vertices.end(), Model::vertices);

        Model::vertexNormals = new float[v_vertexNormals.size()];
        std::copy(v_vertexNormals.begin(), v_vertexNormals.end(), Model::vertexNormals);

        Model::texCoords = new float[v_texCoords.size()];
        std::copy(v_texCoords.begin(), v_texCoords.end(), Model::texCoords);

        Model::colors = new float[v_colors.size()];
        std::copy(v_colors.begin(), v_colors.end(), Model::colors);

    }


    ObjModel::~ObjModel() {
        //TODO Release any dynamic data

        delete[] vertices;
        delete[] vertexNormals;
        delete[] texCoords;
        delete[] colors;
    }

    void ObjModel::drawSolid() {
        //TODO Draw loaded model data 
        
        //Draw function copied from other model class
        glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(4,GL_FLOAT,0,vertices);
        //glColorPointer(4,GL_FLOAT,0,colors);
        glNormalPointer(GL_FLOAT,sizeof(float)*4,vertexNormals);
        glTexCoordPointer(2,GL_FLOAT,0,texCoords);

        glDrawArrays(GL_TRIANGLES,0,vertexCount);

        glDisableClientState(GL_VERTEX_ARRAY);
        //glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

