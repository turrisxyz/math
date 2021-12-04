#include <stan/math.hpp>
#include <stan/math/laplace/laplace.hpp>
#include <test/unit/math/laplace/laplace_utility.hpp>
#include <test/unit/math/rev/fun/util.hpp>
#include <stan/math/prim/fun/lgamma.hpp>

#include <gtest/gtest.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <vector>

struct poisson_log_likelihood {
  template <typename T_theta, typename T_eta>
  stan::return_type_t<T_theta, T_eta> operator() (
    const Eigen::Matrix<T_theta, -1, 1>& theta,
    const Eigen::Matrix<T_eta, -1, 1>& eta,
    const Eigen::VectorXd& y,
    const std::vector<int>& delta_int,
    std::ostream* pstream) const {
    return stan::math::poisson_log_lpmf(delta_int, theta);
  }
};

TEST(laplace_marginal_lpdf, poisson_log_phi_dim_2) {
  using stan::math::laplace_marginal_lpmf;
  using stan::math::to_vector;
  using stan::math::value_of;
  using stan::math::var;

  int dim_phi = 2;
  Eigen::Matrix<var, Eigen::Dynamic, 1> phi(dim_phi);
  phi << 1.6, 0.45;

  int dim_theta = 2;
  Eigen::VectorXd theta_0(dim_theta);
  theta_0 << 0, 0;

  int dim_x = 2;
  std::vector<Eigen::VectorXd> x(dim_theta);
  Eigen::VectorXd x_0(2);
  x_0 << 0.05100797, 0.16086164;
  Eigen::VectorXd x_1(2);
  x_1 << -0.59823393, 0.98701425;
  x[0] = x_0;
  x[1] = x_1;

  Eigen::Matrix<var, Eigen::Dynamic, 1> eta_dummy;
  Eigen::VectorXd y_dummy;
  std::vector<double> delta_dummy;
  std::vector<int> delta_int_dummy;

  std::vector<int> n_samples = {1, 1};
  std::vector<int> sums = {1, 0};

  stan::math::test::squared_kernel_functor K;
  var target
    = laplace_marginal_lpmf<FALSE>(sums, poisson_log_likelihood(),
                                   eta_dummy, y_dummy,
                                   K, phi, x, delta_dummy,
                                   delta_int_dummy, theta_0);

  // TODO: benchmark target against gpstuff.
  // Expected: -2.53056
  double tol = 1e-4;
  EXPECT_NEAR(-2.53056, value_of(target), tol);

  // Test with optional arguments
  double tolerance = 1e-6;
  int max_num_steps = 100;
  int hessian_block_size = 0;
  int solver = 1;
  int do_line_search = 1;
  int max_steps_line_search = 10;

  target = laplace_marginal_lpmf<FALSE>(sums, poisson_log_likelihood(),
                                        eta_dummy, y_dummy,
                                        K, phi, x, delta_dummy,
                                        delta_int_dummy, theta_0,
                                        tolerance, max_num_steps,
                                        hessian_block_size, solver,
                                        do_line_search, max_steps_line_search);
  EXPECT_NEAR(-2.53056, value_of(target), tol);

  std::vector<double> g;
  std::vector<stan::math::var> parm_vec{phi(0), phi(1)};
  target.grad(parm_vec, g);

  // // finite diff test
  double diff = 1e-7;
  Eigen::VectorXd phi_dbl = value_of(phi);
  Eigen::VectorXd phi_1l = phi_dbl, phi_1u = phi_dbl, phi_2l = phi_dbl,
                  phi_2u = phi_dbl;
  phi_1l(0) -= diff;
  phi_1u(0) += diff;
  phi_2l(1) -= diff;
  phi_2u(1) += diff;

  Eigen::VectorXd eta_dummy_dbl = value_of(eta_dummy);

  double target_1u = laplace_marginal_lpmf<FALSE>(
    sums, poisson_log_likelihood(), eta_dummy_dbl, y_dummy, K, phi_1u, x,
    delta_dummy, delta_int_dummy, theta_0),
  target_1l = laplace_marginal_lpmf<FALSE>(
    sums, poisson_log_likelihood(), eta_dummy_dbl, y_dummy, K, phi_1l, x,
    delta_dummy, delta_int_dummy, theta_0),
  target_2u = laplace_marginal_lpmf<FALSE>(
    sums, poisson_log_likelihood(), eta_dummy_dbl, y_dummy, K, phi_2u, x,
    delta_dummy, delta_int_dummy, theta_0),
  target_2l = laplace_marginal_lpmf<FALSE>(
    sums, poisson_log_likelihood(), eta_dummy_dbl, y_dummy, K, phi_2l, x,
    delta_dummy, delta_int_dummy, theta_0);

  std::vector<double> g_finite(dim_phi);
  g_finite[0] = (target_1u - target_1l) / (2 * diff);
  g_finite[1] = (target_2u - target_2l) / (2 * diff);

  tol = 1.1e-4;
  EXPECT_NEAR(g_finite[0], g[0], tol);
  EXPECT_NEAR(g_finite[1], g[1], tol);
}

struct poisson_log_exposure_likelihood {
  template <typename T_theta, typename T_eta>
  stan::return_type_t<T_theta, T_eta> operator() (
    const Eigen::Matrix<T_theta, -1, 1>& theta,
    const Eigen::Matrix<T_eta, -1, 1>& eta,
    const Eigen::VectorXd& ye,
    const std::vector<int>& delta_int,
    std::ostream* pstream) const {
      return stan::math::poisson_log_lpmf(delta_int, theta
                                                       + stan::math::log(ye));
  }
};

TEST_F(laplace_disease_map_test, laplace_marginal_lpmf) {
  using stan::math::laplace_marginal_lpmf;
  using stan::math::laplace_marginal_poisson_log_lpmf;
  using stan::math::var;
  using stan::math::value_of;

  Eigen::Matrix<var, Eigen::Dynamic, 1> eta_dummy;
  // Eigen::VectorXd y_dummy;
  std::vector<double> delta_dummy;
  std::vector<int> delta_int_dummy;
  stan::math::test::sqr_exp_kernel_functor K;

  double tolerance = 1e-6;
  int max_num_steps = 100;
  int hessian_block_size = 0;
  int solver = 1;
  int do_line_search = 0;
  int max_steps_line_search = 10;

  var marginal_density
    = laplace_marginal_lpmf<FALSE>(y, poisson_log_exposure_likelihood(),
                                   eta_dummy, ye,
                                   K, phi, x, delta, delta_int, theta_0);

  double tol = 6e-4;
  // Benchmark from GPStuff.
  EXPECT_NEAR(-2866.88, value_of(marginal_density), tol);

  std::vector<double> g;
  std::vector<var> parm_vec{phi(0), phi(1)};
  marginal_density.grad(parm_vec, g);

  // finite diff
  Eigen::VectorXd phi_dbl = value_of(phi);
  Eigen::VectorXd phi_u0 = phi_dbl, phi_u1 = phi_dbl, phi_l0 = phi_dbl,
                  phi_l1 = phi_dbl;
  double eps = 1e-7;

  phi_u0(0) += eps;
  phi_u1(1) += eps;
  phi_l0(0) -= eps;
  phi_l1(1) -= eps;

  Eigen::VectorXd eta_dummy_dbl = value_of(eta_dummy);

  double target_u0 = laplace_marginal_lpmf<FALSE>(
      y, poisson_log_exposure_likelihood(), eta_dummy_dbl, ye, K, phi_u0, x,
      delta, delta_int, theta_0),
    target_u1 = laplace_marginal_lpmf<FALSE>(
        y, poisson_log_exposure_likelihood(), eta_dummy_dbl, ye, K, phi_u1, x,
        delta, delta_int, theta_0),
    target_l0 = laplace_marginal_lpmf<FALSE>(
            y, poisson_log_exposure_likelihood(), eta_dummy_dbl, ye, K, phi_l0, x,
          delta, delta_int, theta_0),
    target_l1 = laplace_marginal_lpmf<FALSE>(
              y, poisson_log_exposure_likelihood(), eta_dummy_dbl, ye, K, phi_l1, x,
              delta, delta_int, theta_0);

  EXPECT_NEAR((target_u0 - target_l0) / (2 * eps), g[0], 3e-3);
  EXPECT_NEAR((target_u1 - target_l1) / (2 * eps), g[1], 2e-4);
}

struct bernoulli_logit_likelihood {
  template <typename T_theta, typename T_eta>
  stan::return_type_t<T_theta, T_eta> operator() (
    const Eigen::Matrix<T_theta, -1, 1>& theta,
    const Eigen::Matrix<T_eta, -1, 1>& eta,
    const Eigen::VectorXd& ye,
    const std::vector<int>& delta_int,
    std::ostream* pstream) const {
      return stan::math::bernoulli_logit_lpmf(delta_int, theta);
  }
};

TEST(laplace_marginal_lpdf, bernoulli_logit_phi_dim500) {
  using stan::math::laplace_marginal_lpmf;
  using stan::math::to_vector;
  using stan::math::var;

  int dim_theta = 500;
  int n_observations = 500;
  std::string data_directory = "test/unit/math/laplace/aki_synth_data/";
  std::vector<double> x1(dim_theta), x2(dim_theta);
  std::vector<int> y(n_observations);
  stan::math::test::read_in_data(dim_theta, n_observations, data_directory, x1,
                                 x2, y);

  int dim_x = 2;
  std::vector<Eigen::VectorXd> x(dim_theta);
  for (int i = 0; i < dim_theta; i++) {
    Eigen::VectorXd coordinate(dim_x);
    coordinate << x1[i], x2[i];
    x[i] = coordinate;
  }
  Eigen::VectorXd theta_0 = Eigen::VectorXd::Zero(dim_theta);
  Eigen::VectorXd delta_L;
  std::vector<double> delta;
  std::vector<int> delta_int;
  int dim_phi = 2;
  Eigen::Matrix<var, Eigen::Dynamic, 1> phi(dim_phi);
  phi << 1.6, 1;
  Eigen::Matrix<var, Eigen::Dynamic, 1> eta_dummy;

  stan::math::test::sqr_exp_kernel_functor K;
  bernoulli_logit_likelihood L;
  var target
    = laplace_marginal_lpmf<FALSE>(y, L, eta_dummy, delta_L,
                                   K, phi, x, delta, delta_int,
                                   theta_0);

  double tol = 8e-5;
  // Benchmark against gpstuff.
  EXPECT_NEAR(-195.368, value_of(target), tol);

  std::vector<double> g;
  std::vector<stan::math::var> parm_vec{phi(0), phi(1)};
  target.grad(parm_vec, g);

  // finite diff benchmark
  double diff = 1e-7;
  Eigen::VectorXd phi_dbl = value_of(phi);
  Eigen::VectorXd phi_1l = phi_dbl, phi_1u = phi_dbl, phi_2l = phi_dbl,
                  phi_2u = phi_dbl;
  phi_1l(0) -= diff;
  phi_1u(0) += diff;
  phi_2l(1) -= diff;
  phi_2u(1) += diff;
  Eigen::VectorXd eta_dummy_dbl;

  double target_1u = laplace_marginal_lpmf<FALSE>(y, L, eta_dummy_dbl, delta_L,
                                 K, phi_1u, x, delta, delta_int,
                                 theta_0),
         target_1l = laplace_marginal_lpmf<FALSE>(y, L, eta_dummy_dbl, delta_L,
                                        K, phi_1l, x, delta, delta_int,
                                        theta_0),
         target_2u = laplace_marginal_lpmf<FALSE>(y, L, eta_dummy_dbl, delta_L,
                                        K, phi_2u, x, delta, delta_int,
                                        theta_0),
         target_2l = laplace_marginal_lpmf<FALSE>(y, L, eta_dummy_dbl, delta_L,
                                        K, phi_2l, x, delta, delta_int,
                                        theta_0);

  std::vector<double> g_finite(dim_phi);
  g_finite[0] = (target_1u - target_1l) / (2 * diff);
  g_finite[1] = (target_2u - target_2l) / (2 * diff);

  tol = 1e-3;
  EXPECT_NEAR(g_finite[0], g[0], tol);
  EXPECT_NEAR(g_finite[1], g[1], tol);
}
