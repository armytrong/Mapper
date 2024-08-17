//
// Created by jakob on 23.06.24.
//

#ifndef LINALGHELPERS_H
#define LINALGHELPERS_H
#include <ostream>

#include "typedefs.h"

namespace MapperLib {
/**
 * Compute the euclidian distance between two data points, i.e. \f$\|v_1 - v_2\|_2\f$
 * @param vec1 first data point
 * @param vec2 second data point
 * @return The distance between the vectors as a Scalar
 */
Scalar euclididan_distance(Vector const& vec1, Vector const& vec2);

/**
 * Compute the maximum distance between two data points, i.e. \f$\|v_1 - v_2\|_\infty\f$
 * @param vec1 first data point
 * @param vec2 second data point
 * @return The distance between the vectors as a Scalar
 */
Scalar maximum_distance(Vector const& vec1, Vector const& vec2);

/**
 * Checks that all columns in the matrix have the same dimension, as Matrix is only an alias for `std::vector<std::vector<Scalar>>`
 * @param mat the matrix to check
 * @return true if matrix is valid, else false
 */
bool check_data_equal_dimension(Matrix const& mat);

/**
 * Get the dimension of the data points in a matrix
 * @param mat the data matrix
 * @return dimension of the first element in the matrix
 * @warning This method does not assert that all data points are of equal length!
 */
Dimension get_data_dimension(Matrix const& mat);

void print(Vector const& vec);
} // Helper

std::ostream& operator<<(std::ostream& os, MapperLib::Vector const& vec);
std::ostream& operator<<(std::ostream& os, MapperLib::Matrix const& mat);

#endif //LINALGHELPERS_H
