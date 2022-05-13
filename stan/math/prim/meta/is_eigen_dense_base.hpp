#ifndef STAN_MATH_PRIM_META_IS_EIGEN_DENSE_BASE_HPP
#define STAN_MATH_PRIM_META_IS_EIGEN_DENSE_BASE_HPP

#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/meta/bool_constant.hpp>
#include <stan/math/prim/meta/is_base_pointer_convertible.hpp>
#include <stan/math/prim/meta/require_helpers.hpp>
#include <type_traits>

namespace stan {

/** \ingroup type_trait
 * Checks whether type T is derived from Eigen::DenseBase.
 * If true this will have a static member function named value with a type
 * of true, else value is false.
 * @tparam T Type to check if it is derived from `DenseBase`
 * @tparam Enable used for SFINAE deduction.
 * @ingroup type_trait
 */
template <typename T>
struct is_eigen_dense_base
    : bool_constant<is_base_pointer_convertible<Eigen::DenseBase, T>::value> {};

STAN_ADD_REQUIRE_UNARY(eigen_dense_base, is_eigen_dense_base,
                       require_eigens_types);
STAN_ADD_REQUIRE_CONTAINER(eigen_dense_base, is_eigen_dense_base,
                           require_eigens_types);

}  // namespace stan

#endif
