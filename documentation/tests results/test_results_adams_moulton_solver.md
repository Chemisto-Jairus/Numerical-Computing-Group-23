# Adams-Moulton Solver Tests Documentation

## 1. Unit Tests (`test_adams_moulton_solver.cpp`)

### Accuracy on Standard Problems

* **Tested (Exponential Decay):** $dy/dt = -2y$, $y(0) = 1.0$
  * **Interval:** $[0.0, 1.0]$ with $h = 0.05$
  * **Expected Value:** AM2 and AM4 converge closely to exact value $e^{-2}$ (notably, with tighter tolerances than Adams-Bashforth).
* **Tested (Polynomial Exactness - AM2):** $dy/dt = 2t$, $y(0) = 0.0$
  * **Interval:** $[0.0, 3.0]$ with $h = 0.5$
  * **Expected Value:** $9.0$ exactly. The AM2 method (equivalent to the trapezoidal rule) integrates linear slopes exactly.
* **Tested (Mixed Function - AM4):** $dy/dt = y - t$, $y(0) = 2.0$
  * **Interval:** $[0.0, 1.0]$ with $h = 0.01$
  * **Expected Value:** $e + 2.0$ (matching the exact solution $y = e^t + t + 1$).

### Observed Order of Accuracy

* **Tested:** Error reduction scaling when halving the step size ($h$) from $0.05$ to $0.025$ on the exponential decay problem.
* **Expected Result (AM2):** Halving $h$ divides the error by approximately 4 (measured ratio strictly between 3.5 and 4.5).
* **Expected Result (AM4):** Halving $h$ divides the error by approximately 16 (measured ratio strictly between 13.0 and 19.0).

### Boundaries and Curve Shape

* **Tested (Vector Shape):** AM4 with $h = 0.1$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The first point is exactly $(t_0, y_0)$, the returned vector contains exactly 11 points, and the final time value is exactly $t_{end}$.
* **Tested (Uneven Steps):** AM2 with $h = 0.3$ on interval $[0.0, 1.0]$.
  * **Expected Result:** The final step is properly shortened to end exactly at $t = 1.0$, returning a total of 5 points.
* **Tested (Oversized Step):** AM2 with $h = 5.0$ on interval $[0.0, 1.0]$ (step is larger than the entire interval).
  * **Expected Result:** Gracefully completes in a single step to $t_{end}$, returning exactly 2 points.

### Invalid Input Exceptions & Corrector Convergence

* **Tested (Invalid parameters):** Rejection of $h = 0$, $h < 0$, $h = \text{NaN}$, $t_{end} = t_0$, $t_{end} < t_0$, and requested intervals requiring too many steps (e.g., $h = 10^{-7}$).
  * **Expected Result:** Throws `std::invalid_argument`.
* **Tested (Stiff Problem Divergence):** Evaluating a stiff equation $dy/dt = -50y$ with a large step size $h = 0.1$.
  * **Expected Result:** Throws `std::runtime_error` because the step size is too large for the implicit corrector iteration to converge (since $h \cdot \frac{1}{2} \cdot 50 = 2.5 > 1$). 
* **Tested (Stiff Problem Convergence):** The same stiff equation $dy/dt = -50y$ evaluated with a smaller, appropriate step size $h = 0.01$.
  * **Expected Result:** The corrector successfully converges and the method runs without throwing an exception.