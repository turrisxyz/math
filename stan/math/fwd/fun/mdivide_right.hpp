#ifndef STAN_MATH_FWD_FUN_MDIVIDE_RIGHT_HPP
#define STAN_MATH_FWD_FUN_MDIVIDE_RIGHT_HPP

#include <stan/math/prim/err.hpp>
#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/fun/mdivide_right.hpp>
#include <stan/math/prim/fun/multiply.hpp>
#include <stan/math/fwd/core.hpp>
#include <stan/math/fwd/fun/multiply.hpp>
#include <stan/math/fwd/fun/to_fvar.hpp>
#include <vector>

namespace stan {
namespace math {
/*
template <typename EigMat1, typename EigMat2,
          require_all_eigen_vt<is_fvar, EigMat1, EigMat2>* = nullptr>
inline auto
mdivide_right(const EigMat1& b, const EigMat2& A) {
  std::cout << "\nUsing 1: " << "\n";
  using A_fvar_inner_type = typename value_type_t<EigMat2>::Scalar;
  using b_fvar_inner_type = typename value_type_t<EigMat1>::Scalar;
  using inner_ret_t = return_type_t<A_fvar_inner_type, b_fvar_inner_type>;
  constexpr auto R1 = EigMat1::RowsAtCompileTime;
  constexpr auto C1 = EigMat1::ColsAtCompileTime;
  constexpr auto R2 = EigMat2::RowsAtCompileTime;
  constexpr auto C2 = EigMat2::ColsAtCompileTime;

  check_square("mdivide_right", "A", A);
  check_multiplicable("mdivide_right", "b", b, "A", A);
  if (A.size() == 0) {
    using ret_t = decltype(mdivide_right(b.val(), A.val()).eval());
    return promote_scalar_t<fvar<inner_ret_t>, ret_t>{b.rows(), 0};
  }

  Eigen::Matrix<A_fvar_inner_type, R2, C2> val_A(A.rows(), A.cols());
  Eigen::Matrix<A_fvar_inner_type, R2, C2> deriv_A(A.rows(), A.cols());

  const auto& A_ref = to_ref(A);
  for (int j = 0; j < A.cols(); j++) {
    for (int i = 0; i < A.rows(); i++) {
      val_A.coeffRef(i, j) = A_ref.coeff(i, j).val_;
      deriv_A.coeffRef(i, j) = A_ref.coeff(i, j).d_;
    }
  }

  Eigen::Matrix<b_fvar_inner_type, R1, C1> val_b(b.rows(), b.cols());
  Eigen::Matrix<b_fvar_inner_type, R1, C1> deriv_b(b.rows(), b.cols());
  const auto& b_ref = to_ref(b);
  for (Eigen::Index j = 0; j < b.cols(); j++) {
    for (Eigen::Index i = 0; i < b.rows(); i++) {
      val_b.coeffRef(i, j) = b_ref.coeff(i, j).val_;
      deriv_b.coeffRef(i, j) = b_ref.coeff(i, j).d_;
    }
  }
  auto A_mult_inv_b = mdivide_right(val_b, val_A).eval();
  promote_scalar_t<fvar<inner_ret_t>, decltype(A_mult_inv_b)>
ret(A_mult_inv_b.rows(), A_mult_inv_b.cols()); ret.val() = A_mult_inv_b; ret.d()
= mdivide_right(deriv_b, val_A)
      - multiply(A_mult_inv_b, mdivide_right(deriv_A, val_A));
  return ret;
}

template <typename EigMat1, typename EigMat2,
          require_eigen_vt<is_fvar, EigMat1>* = nullptr,
          require_eigen_vt<is_var_or_arithmetic, EigMat2>* = nullptr>
          inline auto
          mdivide_right(const EigMat1& b, const EigMat2& A) {
            using T_return = return_type_t<EigMat1, EigMat2>;
            check_square("mdivide_right", "A", A);
            check_multiplicable("mdivide_right", "b", b, "A", A);
            if (A.size() == 0) {
              using ret_type = decltype(A.transpose().template
cast<T_return>().lu().solve(b.template
cast<T_return>().transpose()).transpose().eval()); return ret_type{b.rows(), 0};
            }
            return A.transpose().template cast<T_return>().lu().solve(b.template
cast<T_return>().transpose()).transpose().eval();
          }

template <typename EigMat1, typename EigMat2,
          require_eigen_vt<is_var_or_arithmetic, EigMat1>* = nullptr,
          require_eigen_vt<is_fvar, EigMat2>* = nullptr>
          inline auto
          mdivide_right(const EigMat1& b, const EigMat2& A) {
            using T_return = return_type_t<EigMat1, EigMat2>;
            check_square("mdivide_right", "A", A);
            check_multiplicable("mdivide_right", "b", b, "A", A);
            if (A.size() == 0) {
              using ret_type = decltype(A.transpose().template
cast<T_return>().lu().solve(b.template
cast<T_return>().transpose()).transpose().eval()); return ret_type{b.rows(), 0};
            }
            return A.transpose().template cast<T_return>().lu().solve(b.template
cast<T_return>().transpose()).transpose().eval();
          }
*/
}  // namespace math
}  // namespace stan
#endif
