# Basic Integrator Tests Documentation

## 1. Unit Tests (`test_basic_integrator.cpp`)

### Accuracy on Standard Problems

* **Tested (Polynomial - Trapezoidal & Midpoint):** $f(x) = x^2$
  * **Interval:** $[0.0, 1.0]$ with $n = 100$ strips
  * **Expected Value:** Converges closely to $1/3$ for both methods.

* **Tested (Trigonometric - Trapezoidal & Midpoint):** $f(x) = \sin(x)$
  * **Interval:** $[0.0, \pi]$ with $n = 1000$ strips
  * **Expected Value:** Converges closely to $2.0$ for both methods.

### Exactness Properties and Known Errors

* **Tested (Linear Exactness):** $f(x) = 2x + 1$
  * **Interval:** $[0.0, 3.0]$ with $n = 1$ and $n = 7$ strips
  * **Expected Value:** $12.0$ exactly. Both Trapezoidal and Midpoint rules are exact for straight lines, even with just a single strip.

* **Tested (Known Error Values):** $f(x) = x^2$
  * **Interval:** $[0.0, 1.0]$ with exactly $n = 10$ strips
  * **Expected Result (Trapezoidal):** Matches the exact theoretical error formula $1/3 + 1/(6n^2)$, resulting exactly in $1/3 + 1/600$.
  * **Expected Result (Midpoint):** Matches the exact theoretical error formula $1/3 - 1/(12n^2)$, resulting exactly in $1/3 - 1/1200$.

### Boundaries and Edge Cases

* **Tested (Reversed Limits):** Integrating $f(x) = x^2$ from $a = 1.0$ to $b = 0.0$ with $n = 1000$.
  * **Expected Result:** $-1/3$ (swapping the limits correctly flips the sign of the integral).

* **Tested (Zero-Width Interval):** Integrating $f(x) = x^2$ from $a = 2.0$ to $b = 2.0$.
  * **Expected Result:** $0.0$ exactly (the integral over a point is zero).

* **Tested (Negative Domain):** Integrating $f(x) = 2x + 1$ from $a = -2.0$ to $b = -1.0$.
  * **Expected Result:** $-2.0$ exactly.

### Invalid Input Exceptions

* **Tested (Invalid Strip Counts):** Rejection of $n = 0$ and $n = -5$ for both Trapezoidal and Midpoint rules.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Invalid Integration Limits):** Rejection of $a = \text{NaN}$ (Trapezoidal) and $b = \text{infinity}$ (Midpoint).
  * **Expected Result:** Throws `std::invalid_argument`.