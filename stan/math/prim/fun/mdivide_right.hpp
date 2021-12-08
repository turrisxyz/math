#ifndef STAN_MATH_PRIM_FUN_MDIVIDE_RIGHT_HPP
#define STAN_MATH_PRIM_FUN_MDIVIDE_RIGHT_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/err.hpp>
#include <stan/math/prim/fun/Eigen.hpp>

namespace stan {
namespace math {

/**
 * Returns the solution of the system xA=b.
 *
 * @tparam EigMat1 type of the right-hand side matrix or vector
 * @tparam EigMat2 type of the second matrix
 *
 * @param A Matrix.
 * @param b Right hand side matrix or vector.
 * @return x = b A^-1, solution of the linear system.
 * @throws std::domain_error if A is not square or the rows of b don't
 * match the size of A.
 */
template <typename EigMat1, typename EigMat2,
          require_all_eigen_t<EigMat1, EigMat2>* = nullptr>
inline auto mdivide_right(const EigMat1& b, const EigMat2& A) {
  using T_return = return_type_t<EigMat1, EigMat2>;
  check_square("mdivide_right", "A", A);
  check_multiplicable("mdivide_right", "b", b, "A", A);
  if (A.size() == 0) {
    using ret_type
        = decltype(A.transpose()
                       .template cast<T_return>()
                       .lu()
                       .solve(b.template cast<T_return>().transpose())
                       .transpose()
                       .eval());
    return ret_type{b.rows(), 0};
  }
  return A.transpose()
      .template cast<T_return>()
      .lu()
      .solve(b.template cast<T_return>().transpose())
      .transpose()
      .eval();
}

}  // namespace math
}  // namespace stan

#endif
