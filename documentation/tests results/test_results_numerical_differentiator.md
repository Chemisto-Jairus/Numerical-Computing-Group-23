# Numerical Differentiator Tests Documentation

## 1. Unit Tests (`test_numerical_differentiator.cpp`)

### Accuracy on Standard Problems

* **Tested (Trigonometric):** $f(x) = \sin(x)$ evaluated at $x = 1.0$.
  * **Expected Result:** Evaluated using Forward ($h=10^{-6}$), Backward ($h=10^{-6}$), Central ($h=10^{-5}$), and Richardson Extrapolation ($h=0.01$). All methods converge closely to the exact first derivative $\cos(1.0)$.
  * **Expected Result (Second Derivative):** Evaluated using the second derivative formula ($h=10^{-3}$), converging closely to the exact value $-\sin(1.0)$.

* **Tested (Exponential):** $f(x) = e^x$ evaluated at $x = 0.0$.
  * **Expected Result:** Central difference ($h=10^{-5}$) evaluates closely to exactly $1.0$.

### Exactness and Observed Order of Convergence

* **Tested (Polynomial Exactness):**
  * **Forward Difference:** Exact for linear functions ($3x + 1$).
  * **Central Difference:** Exact for quadratic functions ($x^2$ evaluated at $x=3.0$ yields $6.0$).
  * **Second Derivative:** Exact for cubic functions ($x^3$ evaluated at $x=2.0$ yields $12.0$).

* **Tested (Order of Accuracy):** Error reduction scaling when halving the step size ($h$) from $0.1$ to $0.05$ on $f(x) = \sin(x)$.
  * **Expected Result (Forward):** Error is halved (ratio $\sim 2$), demonstrating 1st-order accuracy.
  * **Expected Result (Central):** Error is divided by 4 (ratio $\sim 4$), demonstrating 2nd-order accuracy.
  * **Expected Result (Second Derivative):** Error is divided by 4 (ratio $\sim 4$), demonstrating 2nd-order accuracy.
  * **Expected Result (Richardson):** Error is divided by 16 (ratio $\sim 16$), demonstrating 4th-order accuracy.

### Boundaries and Edge Cases

* **Tested (Negative Domain):** Central difference of $x^2$ evaluated at $x = -2.0$.
  * **Expected Result:** Successfully computes negative slope $-4.0$.

* **Tested (Constant Function):** Second derivative of $f(x) = 7.0$.
  * **Expected Result:** Evaluates exactly to $0.0$.

* **Tested (Non-Differentiable Corner):** Function $f(x) = |x|$ evaluated exactly at the corner $x = 0.0$.
  * **Expected Result (Forward):** Captures the right-hand slope ($+1.0$).
  * **Expected Result (Backward):** Captures the left-hand slope ($-1.0$).
  * **Expected Result (Central):** Returns the average of the two slopes ($0.0$).

### Invalid Input and Mathematical Exceptions

* **Tested (Invalid Step Sizes & Points):** Rejection of $h = 0$, $h < 0$, $h = \text{NaN}$, $h = \infty$, $x = \text{NaN}$, and $x = \infty$.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Function Undefined/NaN):** Evaluating central difference for $\log(x)$ where $x - h < 0$, resulting in a $\text{NaN}$ evaluation from the provided function.
  * **Expected Result:** Intercepts the non-finite result and throws `std::runtime_error`.

* **Tested (Function Overflow):** Evaluating $e^x$ at $x = 800.0$, causing a floating-point overflow to infinity.
  * **Expected Result:** Intercepts the non-finite result and throws `std::runtime_error`.