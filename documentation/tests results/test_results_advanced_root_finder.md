# Advanced Root Finder Tests Documentation

## 1. Unit Tests (`test_advanced_root_finder.cpp`)

### Accuracy on Standard Problems

* **Tested (Secant Method - Polynomial):** $f(x) = x^2 - 4$
  * **Initial Guesses:** $1.0$ and $3.0$
  * **Expected Value:** $2.0$

* **Tested (Fixed-Point Iteration - Square Root):** $g(x) = \sqrt{x + 2}$
  * **Initial Guess:** $1.0$
  * **Expected Value:** $2.0$ (Root of $x^2 - x - 2 = 0$)

* **Tested (Secant Method - Trigonometric):** $f(x) = \cos(x) - x$
  * **Initial Guesses:** $0.0$ and $1.0$
  * **Expected Value:** $\sim 0.7390851332$

* **Tested (Fixed-Point Iteration - Trigonometric):** $g(x) = \cos(x)$
  * **Initial Guess:** $1.0$
  * **Expected Value:** $\sim 0.7390851332$

### Boundaries and Edge Cases

* **Tested (Secant - Reversed Guesses):** $f(x) = x^2 - 4$
  * **Initial Guesses:** Provided in reverse order ($3.0$ then $1.0$).
  * **Expected Result:** Still converges successfully to $2.0$.

* **Tested (Fixed-Point - Exact Start):** $g(x) = \sqrt{x + 2}$
  * **Initial Guess:** $2.0$ (Starting exactly on the root).
  * **Expected Result:** Returns $2.0$ immediately within the required tolerance.

### Invalid Input and Divergence Exceptions

* **Tested (Invalid Tolerance/Iterations):** Rejection of $tol = 0$, $tol = \text{NaN}$, and $max\_iter = 0$ for both methods.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Failure to Converge - Iteration Limit):** Both methods run with a tight tolerance ($10^{-12}$) but restricted to a maximum of $2$ iterations.
  * **Expected Result:** Throws `std::runtime_error` when the iteration limit is reached before convergence.

* **Tested (Secant - Division by Zero Protection):** Providing equal initial guesses ($x_0 = 1.0, x_1 = 1.0$), which would cause $f(x_1) - f(x_0) = 0$.
  * **Expected Result:** Intercepted and throws `std::runtime_error`.

* **Tested (Fixed-Point - Diverging Function):** $g(x) = 2x + 1$, starting at $x_0 = 1.0$. This function runs away from its fixed point ($-1$).
  * **Expected Result:** Fails to converge within the iteration limit and properly throws `std::runtime_error`.