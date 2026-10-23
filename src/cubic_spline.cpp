#include "numerical-computing/cubic_spline.hpp"
#include "internal/input_checks.hpp"

namespace numcomp 
{
    /// 2nd derivatives of the natural spline. See cubic_spline.hpp.
    std::vector<double> CubicSpline::second_derivatives(const std::vector<double>& x_vals, const std::vector<double>& y_vals) 
    {
        // Same data checks as linear interpolation, written once in the shared helper
        checks::check_data_points(x_vals, y_vals);

        int n = static_cast<int>(x_vals.size());   // number of data points

        // M holds the answer. Natural spline: M[0] = 0 and M[n-1] = 0.
        std::vector<double> M(n, 0.0);

        // With only 2 points there are no inner points.
        // The spline is just a straight line, so all M are 0.
        if (n == 2)
        {
            return M;
        }

        // Step 1: widths of each interval, h[i] = x[i+1] - x[i]
        std::vector<double> h(n - 1);
        for (int i = 0; i < n - 1; ++i)
        {
            h[i] = x_vals[i+1] - x_vals[i];
        }

        // Step 2: build the tridiagonal system for the inner points 1..n-2.
        // Row for point i:
        //   h[i-1]*M[i-1] + 2*(h[i-1]+h[i])*M[i] + h[i]*M[i+1]
        //       = 6 * ( (y[i+1]-y[i])/h[i] - (y[i]-y[i-1])/h[i-1] )
        int m = n - 2;                      // number of unknowns
        std::vector<double> lower(m);       // below the diagonal
        std::vector<double> diag(m);        // the diagonal
        std::vector<double> upper(m);       // above the diagonal
        std::vector<double> rhs(m);         // right-hand side

        for (int k = 0; k < m; ++k)
        {
            int i = k + 1;                  // data point index for this row
            lower[k] = h[i-1];
            diag[k]  = 2.0 * (h[i-1] + h[i]);
            upper[k] = h[i];
            rhs[k]   = 6.0 * ((y_vals[i+1] - y_vals[i]) / h[i] - (y_vals[i] - y_vals[i-1]) / h[i-1]);
        }

        // Step 3: Thomas algorithm, forward sweep.
        // Remove the "lower" entry from each row using the row above.
        // The diagonal is always bigger than the other two entries,
        // so we never divide by zero here.
        for (int k = 1; k < m; ++k)
        {
            double factor = lower[k] / diag[k-1];
            diag[k] = diag[k] - factor * upper[k-1];
            rhs[k]  = rhs[k]  - factor * rhs[k-1];
        }

        // Step 4: Thomas algorithm, back substitution (last row first).
        std::vector<double> inner(m);
        inner[m-1] = rhs[m-1] / diag[m-1];
        for (int k = m - 2; k >= 0; --k)
        {
            inner[k] = (rhs[k] - upper[k] * inner[k+1]) / diag[k];
        }

        // Step 5: put the inner values into M (M[0] and M[n-1] stay 0)
        for (int k = 0; k < m; ++k)
        {
            M[k+1] = inner[k];
        }

        return M;
    }

    /// Spline value at x. See cubic_spline.hpp.
    double CubicSpline::interpolate(double x, const std::vector<double>& x_vals, const std::vector<double>& y_vals)
    {
        // This also checks the input (sizes, order) for us
        std::vector<double> M = second_derivatives(x_vals, y_vals);

        // Find the interval [x_i, x_(i+1)] that contains x (shared helper).
        // Throws out_of_range if x is outside the data.
        std::size_t i = checks::find_interval(x, x_vals);

        double h  = x_vals[i+1] - x_vals[i];
        double t0 = x - x_vals[i];          // distance from left point
        double t1 = x_vals[i+1] - x;        // distance from right point

        // Cubic piece on this interval:
        // S(x) = M_i * t1^3 / (6h) + M_(i+1) * t0^3 / (6h)
        //      + (y_i / h - M_i * h / 6) * t1
        //      + (y_(i+1) / h - M_(i+1) * h / 6) * t0
        double part1 = M[i]   * t1 * t1 * t1 / (6.0 * h);
        double part2 = M[i+1] * t0 * t0 * t0 / (6.0 * h);
        double part3 = (y_vals[i]   / h - M[i]   * h / 6.0) * t1;
        double part4 = (y_vals[i+1] / h - M[i+1] * h / 6.0) * t0;

        return part1 + part2 + part3 + part4;
    }
}
