// Tests for numcomp::Interpolator (linear interpolation + Chebyshev nodes)
// Owner: Batte Derrick Calvin (Member 2)
// Uses the shared helpers in test_helpers.hpp.

#include "numerical-computing/interpolator.hpp"
#include "test_helpers.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>

using test::check;
using test::close;

void linear_normal_and_edge()
{
    numcomp::Interpolator interp;
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {5.0, 7.0, 3.0};

    check("Linear: y = 2x at x = 1.5 gives 3", close(interp.linear_interpolate(1.5, {0.0, 1.0, 2.0, 3.0}, {0.0, 2.0, 4.0, 6.0}), 3.0, 1e-12));
    // Between (1,1) and (4,16): 1 + 15*(2-1)/3 = 6
    check("Linear: uneven spacing gives 6 at x = 2", close(interp.linear_interpolate(2.0, {0.0, 1.0, 4.0}, {0.0, 1.0, 16.0}), 6.0, 1e-12));
    check("Linear: negative x and y", close(interp.linear_interpolate(-1.5, {-2.0, -1.0, 0.0}, {4.0, -1.0, 0.0}), 1.5, 1e-12));
    check("Linear: x at first data point", close(interp.linear_interpolate(0.0, x, y), 5.0, 1e-12));
    check("Linear: x at last data point", close(interp.linear_interpolate(2.0, x, y), 3.0, 1e-12));
    check("Linear: x exactly on an inner point", close(interp.linear_interpolate(1.0, x, y), 7.0, 1e-12));
    check("Linear: works with only 2 points", close(interp.linear_interpolate(0.25, {0.0, 1.0}, {0.0, 4.0}), 1.0, 1e-12));
}

void linear_invalid()
{
    numcomp::Interpolator interp;
    test::expect_throw<std::invalid_argument>("Linear: different sizes throw invalid_argument", [&] { interp.linear_interpolate(0.5, {0.0, 1.0, 2.0}, {0.0, 1.0}); });
    test::expect_throw<std::invalid_argument>("Linear: one point throws invalid_argument", [&] { interp.linear_interpolate(0.0, {0.0}, {1.0}); });
    test::expect_throw<std::invalid_argument>("Linear: unsorted x throws invalid_argument", [&] { interp.linear_interpolate(1.5, {0.0, 2.0, 1.0}, {0.0, 4.0, 1.0}); });
    test::expect_throw<std::invalid_argument>("Linear: repeated x throws invalid_argument", [&] { interp.linear_interpolate(1.0, {0.0, 1.0, 1.0, 2.0}, {0.0, 1.0, 2.0, 3.0}); });
    test::expect_throw<std::out_of_range>("Linear: x below range throws out_of_range", [&] { interp.linear_interpolate(-1.0, {0.0, 1.0}, {0.0, 1.0}); });
    test::expect_throw<std::out_of_range>("Linear: x above range throws out_of_range", [&] { interp.linear_interpolate(1.5, {0.0, 1.0}, {0.0, 1.0}); });
    // Regression test for the Week 3 bug: NaN slipped past the order check
    test::expect_throw<std::invalid_argument>("Linear: NaN in x values throws invalid_argument", [&] { interp.linear_interpolate(0.5, {0.0, std::nan(""), 2.0}, {0.0, 1.0, 2.0}); });
}

void chebyshev()
{
    numcomp::Interpolator interp;

    std::vector<double> two = interp.chebyshev_nodes(-1.0, 1.0, 2);
    double r = std::sqrt(2.0) / 2.0;     // cos(pi/4)
    check("Chebyshev: n = 2 on [-1,1] gives +/- 0.7071", two.size() == 2 && close(two[0], r, 1e-12) && close(two[1], -r, 1e-12));

    std::vector<double> seven = interp.chebyshev_nodes(2.0, 5.0, 7);
    bool inside = seven.size() == 7;
    for (double node : seven)
    {
        if (node <= 2.0 || node >= 5.0) inside = false;
    }
    check("Chebyshev: all 7 nodes lie strictly inside [2, 5]", inside);

    std::vector<double> one = interp.chebyshev_nodes(0.0, 4.0, 1);
    check("Chebyshev: n = 1 gives the midpoint", one.size() == 1 && close(one[0], 2.0, 1e-12));

    test::expect_throw<std::invalid_argument>("Chebyshev: n = 0 throws invalid_argument", [&] { interp.chebyshev_nodes(0.0, 1.0, 0); });
    test::expect_throw<std::invalid_argument>("Chebyshev: n < 0 throws invalid_argument", [&] { interp.chebyshev_nodes(0.0, 1.0, -3); });
    test::expect_throw<std::invalid_argument>("Chebyshev: a == b throws invalid_argument", [&] { interp.chebyshev_nodes(1.0, 1.0, 3); });
    test::expect_throw<std::invalid_argument>("Chebyshev: a > b throws invalid_argument", [&] { interp.chebyshev_nodes(2.0, 1.0, 3); });
    // Regression test for the Week 3 bug: a huge n tried to allocate gigabytes
    test::expect_throw<std::invalid_argument>("Chebyshev: n > 1,000,000 throws invalid_argument", [&] { interp.chebyshev_nodes(0.0, 1.0, 1000000001); });
    test::expect_throw<std::invalid_argument>("Chebyshev: NaN end point throws invalid_argument", [&] { interp.chebyshev_nodes(std::nan(""), 1.0, 3); });
}

int main()
{
    std::cout << "[*] Running Interpolator Tests..." << std::endl;
    linear_normal_and_edge();
    linear_invalid();
    chebyshev();
    return test::finish("Interpolator");
}
