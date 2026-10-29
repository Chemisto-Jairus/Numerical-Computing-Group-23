# Numerical Differentiator Examples Documentation

## 1. [Standard Differentiation Formulas](../../examples/example_numerical_differentiator.cpp)

### First and Second Derivatives Baseline
* **Tested:** Accuracy of forward, backward, central, Richardson extrapolation, and second derivative formulas at a specific point.
* **Equation:** $f(x) = \sin(x)$
* **Evaluation Point:** $x = 1.0$
* **Step Size ($h$):** $0.01$
* **Expected Result (First Derivatives):** All approximate $\cos(1.0)$.
* **Expected Result (Second Derivative):** Approximates $-\sin(1.0)$.

### Order of Accuracy
* **Tested:** The rate at which the error shrinks as the step size is halved.
* **Equation:** $f(x) = e^x$
* **Evaluation Point:** $x = 0.0$
* **Tested Step Sizes ($h$):** $0.1, 0.05, 0.025, 0.0125$
* **Expected Result:** Errors decrease by factors of 2 for forward/backward (1st order), 4 for central/second derivative (2nd order), and 16 for Richardson extrapolation (4th order).

### Rounding Error Limits
* **Tested:** The threshold where a step size becomes too small, causing floating-point rounding errors to dominate the result.
* **Equation:** $f(x) = e^x$
* **Evaluation Point:** $x = 0.0$
* **Tested Step Sizes ($h$):** $10^{-2}, 10^{-4}, 10^{-6}, 10^{-8}, 10^{-10}, 10^{-12}$
* **Method:** Central difference.
* **Expected Result:** Error minimizes around $h = 10^{-5}$. For smaller $h$, the error increases due to subtraction cancellation noise.

### Non-Differentiable Corners
* **Tested:** Behavior of the formulas at a non-differentiable point.
* **Equation:** $f(x) = |x|$
* **Evaluation Point:** $x = 0.0$
* **Step Size ($h$):** $0.1$
* **Expected Result:** Forward difference yields $1.0$, backward yields $-1.0$, and central averages them to $0.0$.

### Invalid Input Exceptions
* **Tested (Zero Step):** Rejection of $h = 0.0$.
* **Tested (Negative Step):** Rejection of $h = -0.1$.
* **Tested (Domain Error):** Rejection when $f(x-h)$ evaluates outside the mathematical domain (e.g., $\ln(x)$ evaluated at $x = 0.05$ with $h = 0.1$, resulting in a negative argument).

---

## 2. [Applied Problems](../../examples/example_numerical_differentiator_applied.cpp)

### Kinematics (Velocity and Acceleration)
* **Tested:** Calculating instantaneous velocity (1st derivative) and acceleration (2nd derivative) from a position formula.
* **Equation:** $s(t) = 2t^3 - 3t^2 + 5t$
* **Evaluation Points ($t$):** $0.0, 1.0, 2.0, 3.0$
* **Step Sizes ($h$):** $10^{-4}$ (velocity via central), $10^{-3}$ (acceleration via second derivative).
* **Expected Result:** Numerical outputs match the exact velocity $v(t) = 6t^2 - 6t + 5$ and exact acceleration $a(t) = 12t - 6$.

### Economics (Marginal Cost)
* **Tested:** Evaluating the marginal cost of production using Richardson extrapolation.
* **Equation:** $C(q) = 500 + 20q + 2000\ln(1 + \frac{q}{50})$
* **Evaluation Points ($q$):** $10.0, 100.0, 400.0$
* **Step Size ($h$):** $1.0$
* **Expected Result:** Matches the exact marginal cost derivative $20 + \frac{2000}{50 + q}$.

### Derivative-Free Newton-Raphson
* **Tested:** Using a numerical central difference instead of a hand-coded exact derivative within a root-finding algorithm.
* **Equation:** $f(x) = x^3 - 2x - 5.0$
* **Initial Guess:** $x_0 = 2.0$
* **Tolerance:** $10^{-12}$
* **Numerical Step Size ($h$):** $10^{-6}$
* **Expected Result:** The numerical derivative provides enough accuracy for the Newton-Raphson solver to successfully converge to the root $2.0945514815423265$ at the same speed as an exact derivative.