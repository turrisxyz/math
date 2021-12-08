#include <test/unit/math/test_ad.hpp>
#include <vector>
/*
TEST(MathMixMatFun, mdivideRightSizes) {
  auto f = [](const auto& x, const auto& y) {
    return stan::math::mdivide_right(x, y);
  };

  // size zero inputs
  Eigen::MatrixXd m00(0, 0);
  Eigen::MatrixXd m20(2, 0);
  Eigen::RowVectorXd rv0(0);
  stan::test::expect_ad(f, m00, m00);
  stan::test::expect_ad(f, m20, m00);
  stan::test::expect_ad(f, rv0, m00);

  Eigen::MatrixXd aa(1, 1);
  aa << 2;
  Eigen::MatrixXd bb(1, 1);
  bb << 3;
  stan::test::expect_ad(f, bb, aa);
  Eigen::MatrixXd b0(0, 1);
  stan::test::expect_ad(f, b0, aa);

  Eigen::RowVectorXd cc(1);
  cc << 3;
  stan::test::expect_ad(f, cc, aa);
}

TEST(MathMixMatFun, mdivideRight) {
  auto f = [](const auto& x, const auto& y) {
    return stan::math::mdivide_right(x, y);
  };
  Eigen::MatrixXd a(2, 2);
  a << 2, 3, 3, 7;

  Eigen::MatrixXd b(2, 2);
  b << 2, 0, 0, 3;

  Eigen::MatrixXd c(2, 2);
  c << 12, 13, 15, 17;

  Eigen::MatrixXd d(2, 2);
  d << 2, 3, 5, 7;

  Eigen::MatrixXd e(0, 2);

  // matrix, matrix
  for (const auto& m1 : std::vector<Eigen::MatrixXd>{a, b, c, d, e}) {
    for (const auto& m2 : std::vector<Eigen::MatrixXd>{a, b, c, d}) {
      stan::test::expect_ad(f, m1, m2);
    }
  }
}
*/
TEST(MathMixMatFun, mdivideRight_rowvector_matrix1) {
  auto f = [](const auto& x, const auto& y) {
    return stan::math::mdivide_right(x, y);
  };
  Eigen::MatrixXd a(2, 2);
  a << 2, 3, 3, 7;

  Eigen::MatrixXd b(2, 2);
  b << 1, 0, 0, 1;

  Eigen::MatrixXd c(2, 2);
  c << 12, 13, 15, 17;

  Eigen::MatrixXd d(2, 2);
  d << 2, 3, 5, 7;

  Eigen::MatrixXd e(0, 2);

  Eigen::RowVectorXd g(2);
  g << 1, 1;

  stan::test::expect_ad(f, g, b);
  // vector, matrix
  /*
  for (const auto& m : std::vector<Eigen::MatrixXd>{b}) {
      stan::test::expect_ad(f, g, m);
  }
  Eigen::MatrixXd m = b;
  Eigen::Matrix<stan::math::var, -1, -1> m_var = b;
  Eigen::Matrix<stan::math::fvar<double>, -1, -1> m_fvar = b;
  Eigen::Matrix<stan::math::var, 1, -1> g_var = g;
  Eigen::Matrix<stan::math::fvar<double>, 1, -1> g_fvar = g;
  g_fvar.d().setOnes();
  Eigen::MatrixXd ans1 = f(g, m);
  std::cout << "\nans1: \n" << ans1 << "\n";
  auto ans2 = f(g_var, m);
  auto ans3 = f(g_fvar, m);
  ans2.array().sum().grad();
  std::cout << "\nans1 vval: \n" << ans2.val() << "\n";
  std::cout << "\nans1 vadj: \n" << ans2.adj() << "\n";
  std::cout << "\nans1 fval: \n" << ans3.val() << "\n";
  std::cout << "\nans1 fadj: \n" << ans3.d() << "\n";

  auto ans4 = f(g, m_var);
  auto ans5 = f(g, m_fvar);
  auto ans6 = f(g_var, m_var);
  auto ans7 = f(g_fvar, m_fvar);
  */
}
/*
TEST(MathMixMatFun, mdivideRight_rowvector_matrix) {
  auto f = [](const auto& x, const auto& y) {
    return stan::math::mdivide_right(x, y);
  };

  Eigen::RowVectorXd u(5);
  u << 62, 84, 84, 76, 108;
  Eigen::MatrixXd v(5, 5);
  v << 20, 8, -9, 7, 5, 8, 20, 0, 4, 4, -9, 0, 20, 2, 5, 7, 4, 2, 20, -5, 5, 4,
      5, -5, 20;
  stan::test::expect_ad(f, u, v);
}


TEST(MathMixMatFun, mdivideRightZeros) {
  auto f = [](const auto& x, const auto& y) {
    return stan::math::mdivide_right(x, y);
  };
  Eigen::MatrixXd m33 = Eigen::MatrixXd::Zero(3, 3);
  Eigen::MatrixXd m44 = Eigen::MatrixXd::Zero(4, 4);
  Eigen::VectorXd v3 = Eigen::VectorXd::Zero(3);
  Eigen::VectorXd v4 = Eigen::VectorXd::Zero(4);
  Eigen::RowVectorXd rv3 = Eigen::RowVectorXd::Zero(3);
  Eigen::RowVectorXd rv4 = Eigen::RowVectorXd::Zero(4);

  // exceptions: wrong sizes
  stan::test::expect_ad(f, m33, m44);
  stan::test::expect_ad(f, rv3, m44);

  // exceptions: wrong types
  stan::test::expect_ad(f, v3, m33);
}
*/
