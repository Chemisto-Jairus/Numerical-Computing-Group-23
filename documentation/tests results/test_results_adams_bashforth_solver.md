# Adams-Bashforth Solver Tests Documentation

## 1. Unit Tests (`test_adams_bashforth_solver.cpp`)

### Accuracy on Standard Problems
* **Tested (Exponential Decay):** $dy/dt = -2y$, $y(0) = 1.0$
  * **Interval:** $[0.0, 1.0]$ with $h = 0.05$
  * **Expected Value:** AB2 and AB4 converge closely to exact value $e^{-2}$
* **Tested (Polynomial Exactness - AB2):** $dy/dt = 2t$, $y(0) = 0.0$
  * **Interval:** $[0.0, 2.0]$ with $h = 0.1$
  * **Expected Value:** $4.0$ exactly, as AB2 is exact for polynomials of degree 1
* **Tested (Polynomial Exactness - AB4):** $dy/dt = 3t^2$, $y(0) = 0.0$
  * **Interval:** $[0.0, 2.0]$ with $h = 0.1$
  * **Expected Value:** $8.0$ exactly, as AB4 is exact for polynomials of degree 2
* **Tested (Trigonometric):** $dy/dt = -\sin(t)$, $y(0) = 1.0$
  * **Interval:** $[0.0, \pi]$ with $h = 0.01$
  * **Expected Value:** $-1.0$

### Observed Order of Accuracy
* **Tested:** Error reduction scaling when halving the step size ($h$) from $0.05$ to $0.025$ on the decay problem.
* **Expected Result (AB2):** Halving $h$ divides the error by approximately 4 (measured ratio strictly between 3.5 and 4.5).
* **Expected Result (AB4):** Halving $h$ divides the error by approximately 16 (measured ratio strictly between 13.0 and 19.0).

### Boundaries and Curve Shape
* **Tested (Vector Shape):** AB4 with $h = 0.1$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The first point is exactly $(t_0, y_0)$, the returned vector contains exactly 11 points, and the final time value is exactly $t_{end}$.
* **Tested (Short Intervals):** AB4 with $t_{end} = 0.2$ and $h = 0.1$.
  * **Expected Result:** Produces exactly 3 points (2 steps) relying entirely on the RK4 start-up values, remaining highly accurate with expected value $e^{-0.4}$.
* **Tested (Uneven Steps):** AB2 with $h = 0.3$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The final step is properly shortened to end exactly at $t = 1.0$, producing 5 points.
* **Tested (Non-zero Start Time):** AB2 evaluating $dy/dt = 2t$ starting from $t_0 = 1.0$ to $t_{end} = 2.0$.
  * **Expected Value:** $4.0$.

### Invalid Input Exceptions
* **Tested (Invalid parameters):** Rejection of $h = 0$, $h < 0$, $h = \text{NaN}$, $t_{end} = t_0$, $t_{end} < t_0$, and requested intervals requiring too many steps (e.g., $h = 10^{-7}$).
  * **Expected Result:** Throws `std::invalid_argument`.
* **Tested (Mathematical Blow-up):** Evaluating $dy/dt = y^2$, $y(0) = 1.0$ past its singularity at $t = 1.0$ (running up to $t = 3.0$).
  * **Expected Result:** Throws `std::runtime_error` when the computed solution becomes infinite.