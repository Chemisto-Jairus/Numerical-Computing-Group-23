# Cubic Spline Tests Documentation

## 1. Unit Tests (`test_cubic_spline.cpp`)

### Accuracy on Standard Problems

* **Tested (Node Exactness):** Interpolation evaluated exactly at the given $x$ data points.
  * **Expected Result:** The computed spline passes exactly through every original $(x_i, y_i)$ data point.

* **Tested (Hand-Calculated Verification):** Data points $(0,0)$, $(1,1)$, $(2,0)$.
  * **Expected Result (Interpolation):** Symmetrical interpolation yields $S(0.5) = 0.6875$ and $S(1.5) = 0.6875$.
  * **Expected Result (Second Derivatives):** The computed second derivatives ($M$) exactly match the hand-calculated values: $M = \{0, -3, 0\}$.

* **Tested (Natural Spline Boundary Conditions):** Random assortment of 5 data points.
  * **Expected Result:** The second derivative ($M$) is exactly $0.0$ at both the first and last boundary points, validating the "natural" spline behavior.

* **Tested (Linear Exactness):** Data generated from a straight line $y = 2x + 1$.
  * **Expected Result:** Evaluates exactly to $6.0$ at $x = 2.5$, showing the cubic spline perfectly represents linear data.

* **Tested (Trigonometric Approximation):** $11$ data points of $y = \sin(x)$ sampled over the interval $[0, \pi]$.
  * **Expected Result:** Interpolating at $20$ intermediate points between the nodes remains accurate to the true $\sin(x)$ curve within a tolerance of $10^{-3}$.

### Boundaries and Edge Cases

* **Tested (Minimal Dataset):** Interpolation using exactly $2$ data points.
  * **Expected Result:** Gracefully falls back to calculating a straight line between the two points.

* **Tested (Boundary Evaluation):** Evaluating the spline exactly at the first data point ($x_0$) and the last data point ($x_{n-1}$).
  * **Expected Result:** Returns the exact respective $y_0$ and $y_{n-1}$ values without out-of-bounds errors.

* **Tested (Uneven Spacing and Negative Domain):** Linear data with negative $x$ values and uneven spacing (e.g., $x = \{-4.0, -3.5, -1.0, 0.0, 2.5\}$).
  * **Expected Result:** Interpolates perfectly regardless of the irregular spacing and negative coordinates.

### Invalid Input and Extrapolation Exceptions

* **Tested (Invalid Data Arrays):** Rejection of mismatched $x$ and $y$ vector sizes, vectors with only $1$ point, and empty vectors.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Invalid Domain/Sorting):** Rejection of an unsorted $x$ array and arrays with repeated/duplicate $x$ values.
  * **Expected Result:** Throws `std::invalid_argument` because cubic splines require strictly increasing $x$ coordinates.

* **Tested (Extrapolation Bounds):** Attempting to interpolate a value of $x$ that is below the first data point or above the last data point.
  * **Expected Result:** Throws `std::out_of_range` because this implementation strictly performs interpolation, not extrapolation.