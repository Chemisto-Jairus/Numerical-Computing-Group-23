# Interpolator Tests Documentation

## 1. Unit Tests (`test_interpolator.cpp`)

### Linear Interpolation
* **Tested (Linear Exactness):** Interpolating a point at $x = 1.5$ given the dataset $y = 2x$.
  * **Expected Value:** $3.0$ exactly.
* **Tested (Uneven Spacing):** Interpolating at $x = 2.0$ given uneven data points $(0,0)$, $(1,1)$, and $(4,16)$.
  * **Expected Value:** $6.0$ exactly (computed as $1 + 15 \cdot (2-1)/3$).
* **Tested (Negative Domain):** Interpolating at $x = -1.5$ across data with negative $x$ and $y$ values.
  * **Expected Value:** $1.5$ exactly.
* **Tested (Node Exactness):** Evaluating exactly at the first, last, and an inner data point.
  * **Expected Result:** Returns the exact corresponding $y$ value without interpolation errors.
* **Tested (Invalid Inputs):** Mismatched vector sizes, arrays with only 1 point, unsorted $x$ arrays, repeated $x$ values, and $x$ arrays containing $\text{NaN}$.
  * **Expected Result:** Throws `std::invalid_argument`.
* **Tested (Out of Bounds):** Attempting to interpolate an $x$ value strictly below the first data point or above the last data point.
  * **Expected Result:** Throws `std::out_of_range` (extrapolation is intentionally rejected).

### Chebyshev Nodes
* **Tested (Node Generation - Standard):** Generating $n = 2$ nodes on the interval $[-1.0, 1.0]$.
  * **Expected Result:** Precisely returns the two theoretical roots: $\pm \sqrt{2}/2 \approx \pm 0.7071$.
* **Tested (Node Generation - Midpoint):** Generating $n = 1$ node on $[0.0, 4.0]$.
  * **Expected Result:** Returns the exact midpoint of $2.0$.
* **Tested (Interval Bounding):** Generating $n = 7$ nodes on the interval $[2.0, 5.0]$.
  * **Expected Result:** All 7 generated nodes are mathematically guaranteed to lie strictly inside the open interval $(2.0, 5.0)$.
* **Tested (Invalid Parameters):** Rejection of $n = 0$, $n < 0$, excessively large $n > 1,000,000$, intervals where $a \ge b$, or endpoints containing $\text{NaN}$.
  * **Expected Result:** Throws `std::invalid_argument`.

### Chebyshev Approximation
* **Tested (Smooth Function Convergence):** Approximating $f(x) = e^x$ on $[0.0, 1.0]$ using $12$ terms.
  * **Expected Result:** The approximation achieves near full double precision (maximum error $< 10^{-13}$ across the domain), and high-order coefficients shrink rapidly.
* **Tested (Polynomial Exactness):** Approximating a cubic polynomial $f(x) = 2x^3 - x + 1$ on $[-2.0, 3.0]$ using exactly $4$ terms.
  * **Expected Result:** The approximation reproduces the cubic function exactly (tested at $x = 0.7$).
* **Tested (Runge's Phenomenon Mitigation):** Approximating the Runge function $1 / (1 + 25x^2)$ on $[-1.0, 1.0]$.
  * **Expected Result:** Using 40 terms yields a maximum error strictly less than $10\%$ of the error when using 10 terms, proving the Chebyshev nodes prevent the oscillation problems common with uniform grids.
* **Tested (Constant Exactness):** Approximating a constant function $f(x) = 5.0$ with $n = 1$.
  * **Expected Result:** Evaluates exactly to $5.0$.
* **Tested (Invalid Inputs/Coefficients):** Rejection of $n = 0$, $n > 1000$, intervals where $a > b$, and evaluation with an empty coefficient array.
  * **Expected Result:** Throws `std::invalid_argument`.
* **Tested (Function Failure):** The provided function $f(x)$ evaluates to $\text{NaN}$ (e.g., taking the log of a negative number).
  * **Expected Result:** Throws `std::runtime_error`.
* **Tested (Out of Bounds Evaluation):** Attempting to evaluate the approximated Chebyshev series at an $x$ outside the original approximation interval $[a, b]$, or if $x$ is $\text{NaN}$.
  * **Expected Result:** Throws `std::out_of_range`.