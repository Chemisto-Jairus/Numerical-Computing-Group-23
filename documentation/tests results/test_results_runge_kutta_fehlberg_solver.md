# Runge-Kutta-Fehlberg Solver Tests Documentation

## 1. Unit Tests (`test_runge_kutta_fehlberg_solver.cpp`)

### Accuracy and Adaptive Tolerance

* **Tested (Tolerance Enforcement):** $dy/dt = -2y$, $y(0) = 1.0$ evaluated at $t = 1.0$.

  * **Expected Result (Loose):** With $tol = 10^{-4}$, the absolute error compared to the exact solution $e^{-2}$ is strictly less than $10^{-4}$.

  * **Expected Result (Tight):** With $tol = 10^{-8}$, the absolute error is strictly less than $10^{-8}$.

* **Tested (Adaptive Effort):** Solving the decay problem with $tol = 10^{-8}$ vs $tol = 10^{-4}$.

  * **Expected Result:** Asserts that the tighter tolerance fundamentally requires and performs more time steps to maintain accuracy.

* **Tested (`rkf45_value` Helper):** $dy/dt = y - t$, $y(0) = 2.0$ evaluated at $t = 1.0$.

  * **Expected Result:** Helper successfully returns just the final value, which converges closely to $e + 2.0$.

### Exactness and Step-Size Adaptation

* **Tested (Polynomial Exactness):** $dy/dt = 4t^3$, $y(0) = 0.0$ evaluated at $t = 2.0$.

  * **Expected Result:** Exact result of $16.0$ computed in $\le 3$ steps. Because the solution is a fourth-order polynomial ($t^4$), the internal 4th-order formula is exact.

* **Tested (Dynamic Step Scaling):** $dy/dt = -50(y - \cos(t))$, $y(0) = 0.0$. This solution changes rapidly near $t=0$ but smooths out later.

  * **Expected Result:** The solver successfully adapts by making the largest accepted step at least $3\times$ larger than the very first step.

### Boundaries and Curve Shape

* **Tested (Vector Shape):** Standard evaluation on interval $[0.0, 1.0]$.

  * **Expected Result:** The first point is exactly $(t_0, y_0)$ and the final time value is exactly $t_{end}$.

* **Tested (Strict Monotonicity):** Standard evaluation.

  * **Expected Result:** Every generated time step $t_i$ is strictly greater than the previous step $t_{i-1}$.

* **Tested (Oversized Initial Step):** $h_{initial} = 10.0$ on interval $[0.0, 1.0]$.

  * **Expected Result:** Gracefully truncates the step and successfully ends exactly at $t = 1.0$.

### Invalid Input and Mathematical Blow-up

* **Tested (Invalid parameters):** Rejection of $h_{initial} = 0$, $h_{initial} < 0$, $t_{end} = t_0$, $t_{end} < t_0$, $tol = 0$, $tol = \text{NaN}$, and $t_{end} = \text{NaN}$.

  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Mathematical Singularity):** Evaluating $dy/dt = y^2$, $y(0) = 1.0$ past its singularity at $t = 1.0$ (running up to $t = 2.0$).

  * **Expected Result:** As the solution approaches infinity, the adaptive step size shrinks indefinitely to maintain tolerance until it fails, properly throwing `std::runtime_error`.