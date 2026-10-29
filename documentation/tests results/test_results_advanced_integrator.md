# Advanced Integrator Tests Documentation

## 1. Unit Tests (`test_advanced_integrator.cpp`)

### Accuracy on Standard Problems

* **Tested (Polynomial - Simpson & Romberg):** $f(x) = x^3$
  * **Interval:** $[0.0, 2.0]$
  * **Expected Value:** $4.0$ (Simpson with $n=100$, Romberg with $5$ rows).
* **Tested (Trigonometric - Simpson & Romberg):** $f(x) = \sin(x)$
  * **Interval:** $[0.0, \pi]$
  * **Expected Value:** $2.0$ (Simpson with $n=100$, Romberg with $8$ rows).

### Exactness Properties

* **Tested (Simpson's Rule Exactness):** $f(x) = x^3$
  * **Interval:** $[0.0, 2.0]$ with $n=2$
  * **Expected Value:** $4.0$ exactly (Simpson's rule is exact for cubic polynomials, even with only 2 strips).
* **Tested (Romberg Exactness - Cubic):** $f(x) = x^3$
  * **Interval:** $[0.0, 2.0]$ with $2$ rows
  * **Expected Value:** $4.0$ exactly.
* **Tested (Romberg Exactness - Quartic):** $f(x) = x^4$
  * **Interval:** $[0.0, 1.0]$ with $3$ rows
  * **Expected Value:** $1/5$ exactly (Romberg with 3 rows is exact up to degree 5).

### Edge Cases and Boundaries

* **Tested (Reversed Limits):** Integrating $f(x) = x^3$ from $a=2.0$ to $b=0.0$.
  * **Expected Value:** $-4.0$ (Integration handles reversed limits by negating the result).
* **Tested (Minimum Romberg Rows):** $f(x) = x^3$ evaluated with $1$ row on interval $[0.0, 2.0]$.
  * **Expected Value:** $8.0$ (With 1 row, Romberg is equivalent to a single trapezoid: $(f(0) + f(2)) / 2 \cdot 2 = 8$).
* **Tested (High Iteration Romberg):** Romberg integration called with $20$ rows.
  * **Expected Result:** Runs successfully without throwing exceptions.

### Invalid Input Exceptions

* **Tested (Simpson - Strips):** Rejection of an odd number of strips ($n=3$) and zero strips ($n=0$).
  * **Expected Result:** Throws `std::invalid_argument`.
* **Tested (Romberg - Iterations):** Rejection of zero rows ($max\_iter=0$) and rows exceeding the maximum allowed limit ($max\_iter=26$).
  * **Expected Result:** Throws `std::invalid_argument`.
* **Tested (Invalid Integration Limits):** Rejection of $a = \text{NaN}$ and $b = \text{infinity}$.
  * **Expected Result:** Throws `std::invalid_argument`.