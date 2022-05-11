#ifndef STAN_MATH_REV_FUN_INV_PHI_LOG_HPP
#define STAN_MATH_REV_FUN_INV_PHI_LOG_HPP

#include <stan/math/rev/meta.hpp>
#include <stan/math/rev/core.hpp>
#include <stan/math/prim/fun/constants.hpp>
#include <stan/math/prim/fun/inv_Phi_log.hpp>
#include <cmath>

namespace stan {
namespace math {

/**
 * The inverse of unit normal cumulative density function evaluated
 * at the log probability
 *
 * The derivative is the reciprocal of unit normal density function
 * adjusted by the derivative of the exponentiated log input
 *
 * @param log_p log probability
 * @return the unit normal inverse cdf evaluated at log_p
 */
inline var inv_Phi_log(const var& log_p) {
  auto precomp_exp = to_arena(as_array_or_scalar(exp(log_p.val())));
  return make_callback_var(
      inv_Phi_log(log_p.val()), [log_p, precomp_exp](auto& vi) mutable {
        log_p.adj() += precomp_exp * vi.adj() * SQRT_TWO_PI
                       / std::exp(-0.5 * vi.val() * vi.val());
      });
}

/**
 * Return the elementwise inverse of unit normal cumulative density function.
 *
 * @tparam T a `var_value` with inner Eigen type
 * @param p log probability vector
 * @return Elementwise unit normal inverse cdf
 */
template <typename T, require_var_matrix_t<T>* = nullptr>
inline auto inv_Phi_log(const T& log_p) {
  auto precomp_exp = to_arena(as_array_or_scalar(exp(log_p.val())));
  return make_callback_var(
      inv_Phi_log(log_p.val()), [log_p, precomp_exp](auto& vi) mutable {
        log_p.adj().array() += precomp_exp * vi.adj().array() * SQRT_TWO_PI
                               / (-0.5 * vi.val().array().square()).exp();
      });
}

}  // namespace math
}  // namespace stan
#endif
