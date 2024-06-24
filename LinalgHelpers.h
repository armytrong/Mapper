//
// Created by jakob on 23.06.24.
//

#ifndef LINALGHELPERS_H
#define LINALGHELPERS_H
#include "typedefs.h"

namespace Helper {
Scalar euclididan_distance(Vector const& vec1, Vector const& vec2);
Scalar maximum_distance(Vector const& vec1, Vector const& vec2);
bool check_data_equal_dimension(Matrix const& mat);
size_t get_data_dimension(Matrix const& mat);
} // Helper

#endif //LINALGHELPERS_H
