#ifndef STAN_MATH_PRIM_FUN_EIGENVALUES_HPP
#define STAN_MATH_PRIM_FUN_EIGENVALUES_HPP

#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/err.hpp>

namespace stan {
namespace math {

template <typename Mat, require_eigen_t<Mat>* = nullptr>
inline auto eigenvalues(const Mat& m) {
  check_nonzero_size("eigenvalues", "m", m);
  check_square("eigenvalues", "m", m);
  return m.eigenvalues().eval();
}

}  // namespace math
}  // namespace stan
#endif
