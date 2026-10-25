# Basic Integrator Examples Documentation

## 1. Standard Integrals (`example_basic_integrator.cpp`)

### Trapezoidal and Midpoint Baselines
* **Tested:** $f(x) = x^2$
  * **Interval:** $[0, 1]$
  * **Strips ($n$):** $100$
  * **Expected Value:** $1/3$
* **Tested:** $f(x) = \sin(x)$
  * **Interval:** $[0, \pi]$
  * **Strips ($n$):** $100$
  * **Expected Value:** $2.0$
* **Tested:** $f(x) = e^x$
  * **Interval:** $[0, 1]$
  * **Strips ($n$):** $100$
  * **Expected Value:** $e - 1.0$

### Second-Order Convergence
* **Tested:** Error reduction scaling for Trapezoidal and Midpoint rules.
* **Equation:** $f(x) = \sin(x)$
* **Interval:** $[0, \pi]$
* **Strips ($n$):** $4, 8, 16, 32, 64$
* **Expected Result:** Doubling the number of strips divides the error by approximately $4$. The Midpoint rule error is roughly half of the Trapezoidal rule error.

### Reversed Limits
* **Tested:** Integration behavior when the lower limit is greater than the upper limit.
* **Equation:** $f(x) = x^2$
* **Interval:** $[1, 0]$
* **Strips ($n$):** $1000$
* **Expected Value:** $-1/3$ (correctly reverses the sign of the result).

### Invalid Input Exceptions
* **Tested (Strips):** Rejection of an invalid number of strips ($n = 0$).
* **Tested (Limits):** Rejection of an infinite upper limit (`INFINITY`).

---

## 2. Applied Integrals (`example_basic_integrator_applied.cpp`)

### Distance from Velocity
* **Tested:** Calculating total distance traveled given a kinematic velocity function.
* **Equation:** $v(t) = 3t^2 + 2t$
* **Interval:** $[0, 4]$
* **Strips ($n$):** $200$
* **Expected Value:** $80.0$ meters

### Normal Distribution Probability
* **Tested:** Calculating the probability $P(-1 < Z < 1)$ for a standard normal distribution curve.
* **Equation:** $pdf(z) = \frac{e^{-0.5z^2}}{\sqrt{2\pi}}$
* **Interval:** $[-1, 1]$
* **Strips ($n$):** $100$
* **Expected Value:** $\text{erf}(1 / \sqrt{2})$

### Work Done by a Spring
* **Tested:** Calculating the work required to stretch a spring with constant $k = 250$ N/m.
* **Equation:** $F(x) = 250x$
* **Interval:** $[0.1, 0.3]$
* **Strips ($n$):** $1$
* **Expected Value:** $10.0$ Joules (Note: a single strip evaluates this exactly because the force function is linear).