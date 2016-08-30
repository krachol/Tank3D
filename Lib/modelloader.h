#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "model.h"

namespace Models {
    class ObjModel : public Model{
        public:
        ObjModel(const char* filename);
        virtual ~ObjModel();
        
        virtual void drawSolid();
    };
}
