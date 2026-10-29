# Basic Root Finder Tests Documentation

## 1. Unit Tests (`test_basic_root_finder.cpp`)

### Accuracy on Standard Problems

* **Tested (Bisection - Polynomial):** $f(x) = x^2 - 4$
  * **Interval:** $[0.0, 3.0]$
  * **Expected Value:** $2.0$

* **Tested (Bisection - Cubic):** $f(x) = x^3 - x - 2$
  * **Interval:** $[1.0, 2.0]$
  * **Expected Value:** $\sim 1.5213797068$

* **Tested (Newton-Raphson - Polynomial):** $f(x) = x^2 - 4$ with $f'(x) = 2x$
  * **Initial Guess:** $x_0 = 3.0$
  * **Expected Value:** $2.0$

* **Tested (Newton-Raphson - Cubic):** $f(x) = x^3 - x - 2$ with $f'(x) = 3x^2 - 1$
  * **Initial Guess:** $x_0 = 1.5$
  * **Expected Value:** $\sim 1.5213797068$

* **Tested (Newton-Raphson - Trigonometric):** $f(x) = \cos(x) - x$ with $f'(x) = -\sin(x) - 1$
  * **Initial Guess:** $x_0 = 1.0$
  * **Expected Value:** $\sim 0.7390851332$

### Boundaries and Edge Cases

* **Tested (Bisection - Reversed Interval):** $f(x) = x^2 - 4$
  * **Interval:** Provided in reverse order as $[3.0, 0.0]$.
  * **Expected Result:** Properly handles the reversal and converges to $2.0$.

* **Tested (Bisection - Negative Root):** $f(x) = x^2 - 4$
  * **Interval:** $[-3.0, 0.0]$
  * **Expected Result:** Successfully finds the negative root at $-2.0$.

* **Tested (Bisection - Root on Boundary):** $f(x) = x^2 - 4$
  * **Interval:** Tests with the root exactly at the lower bound $[2.0, 5.0]$ and the upper bound $[0.0, 2.0]$.
  * **Expected Result:** Immediately returns $2.0$.

* **Tested (Newton-Raphson - Exact Start):** $f(x) = x^2 - 4$
  * **Initial Guess:** $x_0 = 2.0$ (Starting exactly on the root).
  * **Expected Result:** Stays on the root and returns $2.0$ immediately.

### Invalid Input and Divergence Exceptions

* **Tested (Invalid Tolerance/Iterations):** Rejection of $tol = 0$, $tol = \text{NaN}$, and $max\_iter = 0$ for both methods.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Bisection - Missing Root Bracket):** Interval where both ends share the same sign (e.g., $[3.0, 5.0]$ for $x^2 - 4$).
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Bisection - NaN Evaluation):** Function explicitly returns $\text{NaN}$ during evaluation.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Newton-Raphson - Zero Derivative):** Starting at a local minimum/maximum where $f'(x) = 0$ (e.g., $x_0 = 0.0$ for $x^2 - 4$).
  * **Expected Result:** Throws `std::runtime_error` to prevent division by zero.

* **Tested (Failure to Converge - Iteration Limit):** Methods restricted to a maximum of $2$ iterations with a tight tolerance constraint.
  * **Expected Result:** Throws `std::runtime_error` because they run out of iterations before convergence.