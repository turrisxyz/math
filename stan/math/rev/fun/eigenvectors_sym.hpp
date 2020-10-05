#ifndef STAN_MATH_REV_FUN_EIGENVECTORS_HPP
#define STAN_MATH_REV_FUN_EIGENVECTORS_HPP

#include <stan/math/rev/meta.hpp>
#include <stan/math/rev/core.hpp>
#include <stan/math/rev/fun/value_of_rec.hpp>
#include <stan/math/rev/fun/typedefs.hpp>
#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/err/check_symmetric.hpp>
#include <stan/math/prim/err/check_nonzero_size.hpp>
#include <stan/math/prim/fun/typedefs.hpp>
#include <stan/math/prim/fun/value_of_rec.hpp>

namespace stan {
namespace math {

/**
 * Return the eigenvectors of the specified symmetric matrix.
 * <p>See <code>eigen_decompose()</code> for more information.
 *
 * @tparam T Type of input matrix
 * @param m Input matrix
 * @return Eigenvectors of matrix
 */
template <typename T, require_eigen_vt<is_var, T>* = nullptr>
inline auto eigenvectors_sym(const T& m) {
  using ret_type = promote_scalar_t<var, Eigen::MatrixXd>;

  check_square("eigenvalues_sym", "m", m);
  check_nonzero_size("eigenvalues_sym", "m", m);
  
  auto arena_m = to_arena(m);
  Eigen::MatrixXd m_val = value_of(arena_m);

  check_symmetric("eigenvalues_sym", "m", m_val);

  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(m_val);
  auto arena_eigenvectors_val = to_arena(solver.eigenvectors());
  arena_t<ret_type> res = arena_eigenvectors_val;
  
  int M = m_val.rows();
  arena_t<Eigen::MatrixXd> arena_f(M, M);
  for (int i = 0; i < M; i++)
    for (int j = 0; j < M; j++)
      arena_f.coeffRef(j, i) = (i != j ? 1
				/ (solver.eigenvalues().coeff(i)
				   - solver.eigenvalues().coeff(j))
				: 0);

  reverse_pass_callback([arena_m, arena_eigenvectors_val, arena_f,
                         res]() mutable {
    Eigen::MatrixXd tmp = arena_f.cwiseProduct(arena_eigenvectors_val.transpose().eval() * res.adj_op());
    arena_m.adj()
        += arena_eigenvectors_val * tmp * arena_eigenvectors_val.transpose();
  });

  return res;
}

}  // namespace math
}  // namespace stan
#endif
