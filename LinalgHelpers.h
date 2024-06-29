//
// Created by jakob on 23.06.24.
//

#ifndef LINALGHELPERS_H
#define LINALGHELPERS_H
#include <ostream>

#include "typedefs.h"

namespace Mapper {
Scalar euclididan_distance(Vector const& vec1, Vector const& vec2);
Scalar maximum_distance(Vector const& vec1, Vector const& vec2);
bool check_data_equal_dimension(Matrix const& mat);
Dimension get_data_dimension(Matrix const& mat);

void print(Vector const& vec);
} // Helper

std::ostream& operator<<(std::ostream& os, Mapper::Vector const& vec);
std::ostream& operator<<(std::ostream& os, Mapper::Matrix const& mat);

#endif //LINALGHELPERS_H
