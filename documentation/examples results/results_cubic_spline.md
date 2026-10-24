# Cubic Spline Examples Documentation

## 1. [Standard Cubic Spline](examples/example_cubic_spline.cpp) 

### Hand-Checkable Case
* **Tested:** Correct calculation of second derivatives ($M$) and intermediate interpolation.
* **Given Data ($x$):** $0.0, 1.0, 2.0$
* **Given Data ($y$):** $0.0, 1.0, 0.0$
* **Evaluation Point:** $0.5$
* **Expected $M$ values:** $M_0 = 0.0$ (natural end), $M_1 = -3.0$, $M_2 = 0.0$ (natural end)
* **Expected Interpolation:** $S(0.5) = 0.6875$

### Spline vs Linear Interpolation
* **Tested:** Accuracy comparison between cubic spline and linear interpolation against an exact function.
* **Equation:** $f(x) = \sin(x)$
* **Given Data:** 6 evenly spaced points on interval $[0, \pi]$ (where $x_i = i\pi/5$)
* **Evaluation Points ($t$):** $0.3, 0.9, 1.5, 2.2, 2.9$
* **Expected Result:** Cubic spline produces a curve significantly closer to the exact $\sin(t)$ values than linear interpolation.

### Straight Line Reproduction
* **Tested:** Exact reproduction of a linear dataset without unwanted curvature.
* **Equation:** $y = 2x + 1$
* **Given Data ($x$):** $0.0, 0.7, 2.0, 3.1$
* **Given Data ($y$):** $1.0, 2.4, 5.0, 7.2$
* **Evaluation Point:** $1.6$
* **Expected Value:** $4.2$

### Invalid Input Exceptions
* **Tested (Size):** Rejection of arrays containing only one data point.
* **Tested (Uniqueness):** Rejection of datasets with repeated $x$ values (e.g., $x = \{0.0, 1.0, 1.0\}$).
* **Tested (Extrapolation):** Rejection of evaluation points outside the data range (e.g., $-1.0$ for range $[0.0, 2.0]$).

---

## 2.  [Applied Cubic Spline](examples/example_cubic_spline_applied.cpp)

### Speedometer Data Smoothing
* **Tested:** Generating a smooth continuous profile from discrete, real-world data points.
* **Given Data (Time in s):** $0, 10, 20, 30, 40, 50$
* **Given Data (Speed in km/h):** $0, 32, 55, 61, 58, 70$
* **Evaluation Points:** Every $5.0$ seconds from $0.0$ to $50.0$.

### Convergence Study (Fourth-Order Accuracy)
* **Tested:** Rate at which the maximum interpolation error shrinks as data points increase.
* **Equation:** $f(x) = \sin(x)$
* **Interval:** $[0, \pi]$
* **Sample Sizes ($n$):** $5, 9, 17, 33, 65$
* **Error Metric:** Maximum absolute error calculated across 2001 intermediate sub-intervals.
* **Expected Result:** Halving the step size ($h$) divides the maximum error by approximately $16$, confirming fourth-order accuracy ($O(h^4)$).