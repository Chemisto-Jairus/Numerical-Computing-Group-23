# Advanced Integrator Examples Documentation

## 1. [Standard Integration](../../examples/example_advanced_integrator.cpp)

### Simpson's Rule Baseline Tests
* **Tested:** $f(x) = x^3$
  * **Interval:** $[0, 2]$
  * **Strips ($n$):** $2$
  * **Expected Value:** $4.0$
* **Tested:** $f(x) = \sin(x)$
  * **Interval:** $[0, \pi]$
  * **Strips ($n$):** $20$
  * **Expected Value:** $2.0$
* **Tested:** $f(x) = e^x$
  * **Interval:** $[0, 1]$
  * **Strips ($n$):** $20$
  * **Expected Value:** $e - 1.0$

### Simpson's Rule Convergence
* **Tested:** Fourth-order convergence (error ratio when doubling $n$)
* **Equation:** $f(x) = \sin(x)$
* **Interval:** $[0, \pi]$
* **Values of $n$ tested:** $4, 8, 16, 32$

### Romberg Integration Accuracy
* **Tested:** Error reduction based on the number of rows
* **Equation:** $f(x) = \sin(x)$
* **Interval:** $[0, \pi]$
* **Rows tested:** $1, 2, 3, 4, 5, 6$
* **Expected Value:** Approaches $2.0$

### Invalid Input Exceptions
* **Tested (Simpson):** Rejection of odd $n$ value ($n = 5$)
* **Tested (Romberg):** Rejection of excessive rows ($\text{rows} = 30$)

---

## 2. [Applied Integrals](../../examples/example_advanced_integrator_applied.cpp)

### Error Function $\text{erf}(1)$
* **Tested:** $\frac{2}{\sqrt{\pi}} \int_0^1 e^{-t^2} dt$
* **Equation:** $g(t) = e^{-t^2}$
* **Interval:** $[0, 1]$
* **Simpson's Rule:** $n = 10$
* **Romberg Integration:** $\text{rows} = 5$
* **Expected Value:** $\text{erf}(1)$

### Arc Length of Parabola $y = x^2$
* **Tested:** Arc length from $x = 0$ to $1$
* **Equation:** $ds(x) = \sqrt{1 + 4x^2}$
* **Interval:** $[0, 1]$
* **Simpson's Rule:** $n = 20$
* **Romberg Integration:** $\text{rows} = 6$
* **Expected Value:** $\frac{\sqrt{5}}{2} + \frac{\text{asinh}(2)}{4}$

### Algorithm Cost Comparison
* **Tested:** Number of function evaluations needed to achieve an error $< 10^{-10}$
* **Equation:** $f(x) = e^x$
* **Interval:** $[0, 1]$
* **Expected Value:** $e - 1.0$
* **Metric:** Dynamic counting of $f(x)$ calls while incrementally increasing $n$ (Simpson) and rows (Romberg) until tolerance is met.