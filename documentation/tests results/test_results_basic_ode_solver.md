# Basic ODE Solver Tests Documentation

## 1. Unit Tests (`test_basic_ode_solver.cpp`)

### Accuracy on Standard Problems

* **Tested (Constant Exactness - Euler):** $dy/dt = 1$, $y(0) = 0.0$
  * **Interval:** $[0.0, 1.0]$ with $h = 0.1$
  * **Expected Value:** $1.0$ exactly.

* **Tested (Linear Known Error - Euler):** $dy/dt = 2t$, $y(0) = 0.0$
  * **Interval:** $[0.0, 1.0]$ with $h = 0.1$
  * **Expected Value:** $0.9$ exactly. This demonstrates Euler's known first-order truncation error ($h^2 \cdot n(n-1) = 0.9$).

* **Tested (Linear Exactness - Heun):** $dy/dt = 2t$, $y(0) = 0.0$
  * **Interval:** $[0.0, 1.0]$ with $h = 0.1$
  * **Expected Value:** $1.0$ exactly. Heun's method averages the slopes, making it exact when $f$ is linear with respect to $t$.

* **Tested (Trigonometric):** $dy/dt = \cos(t)$, $y(0) = 0.0$
  * **Interval:** $[0.0, 2.0]$
  * **Expected Value:** Converges closely to $\sin(2.0)$ (Euler evaluated with $h = 0.001$, Heun with $h = 0.01$).

* **Tested (Exponential Growth):** $dy/dt = y$, $y(0) = 1.0$
  * **Interval:** $[0.0, 1.0]$
  * **Expected Value:** Converges closely to $e$ (Euler evaluated with $h = 0.001$, Heun with $h = 0.01$).

### Observed Order of Convergence

* **Tested:** Error reduction scaling when halving the step size ($h$) from $0.01$ to $0.005$ on the exponential growth problem.
* **Expected Result (Euler):** Halving $h$ halves the error (measured ratio strictly between 1.8 and 2.2), demonstrating first-order accuracy.
* **Expected Result (Heun):** Halving $h$ quarters the error (measured ratio strictly between 3.6 and 4.4), demonstrating second-order accuracy.

### Boundaries and Curve Shape

* **Tested (Vector Shape):** Euler with $h = 0.1$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The first point is exactly $(t_0, y_0)$, the returned vector contains exactly 11 points, and the final time value is exactly $t_{end}$.
* **Tested (Uneven Steps):** Heun with $h = 0.3$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The final step is properly shortened to end exactly at $t = 1.0$, returning a total of 5 points.
* **Tested (Oversized Step):** Euler with $h = 5.0$ on interval $[0.0, 1.0]$ (step is larger than the entire interval).
  * **Expected Result:** Gracefully completes in a single step to $t_{end}$, returning exactly 2 points.
* **Tested (Negative Start Time):** Heun evaluating $dy/dt = 2t$ starting from $t_0 = -1.0$ with $y(-1) = 1.0$, up to $t_{end} = 1.0$.
  * **Expected Value:** $1.0$ exactly.

### Invalid Input Exceptions

* **Tested (Invalid parameters):** Rejection of $h = 0$, $h < 0$, $h = \text{NaN}$, $t_{end} = t_0$, $t_{end} < t_0$, and requested intervals requiring too many steps (e.g., $h = 10^{-7}$).
  * **Expected Result:** Both Euler and Heun correctly intercept these inputs and throw `std::invalid_argument`.