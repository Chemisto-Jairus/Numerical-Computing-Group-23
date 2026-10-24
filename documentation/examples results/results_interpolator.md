# Interpolator Examples Documentation

## 1. [Standard Interpolation](<examples/example_interpolator.cpp>)

### Linear Interpolation Baseline
* **Tested:** Accuracy of piecewise linear interpolation against an exact convex curve.
* **Equation:** $y = x^2$
* **Given Data ($x$):** $0.0, 1.0, 2.0, 3.0, 4.0$
* **Given Data ($y$):** $0.0, 1.0, 4.0, 9.0, 16.0$
* **Evaluation Points ($t$):** $0.5, 1.5, 2.25, 3.0, 3.9$
* **Expected Result:** Evaluates exactly at nodes; produces an interpolation error $> 0$ between nodes.

### Uneven Spacing and Negative Values
* **Tested:** Handling of non-uniform grid spacing and negative coordinates.
* **Given Data ($x$):** $-3.0, -1.0, 0.5, 4.0$
* **Given Data ($y$):** $9.0, -2.0, 1.0, 8.0$
* **Evaluation Points:** $-2.0, 0.0, 2.25$

### Chebyshev Nodes Generation
* **Tested:** Calculation and distribution of Chebyshev nodes.
* **Interval:** $[-1.0, 1.0]$
* **Number of Nodes ($n$):** $3, 5, 8$
* **Expected Result:** Nodes successfully generated, clustering densely near the endpoints $-1.0$ and $1.0$.

### Invalid Input Exceptions
* **Tested (Extrapolation):** Rejection of $x = 5.0$ when data bounds are $[0.0, 2.0]$.
* **Tested (Sorting):** Rejection of non-increasing $x$ arrays (e.g., $x = \{0.0, 2.0, 1.0\}$).
* **Tested (Chebyshev):** Rejection of invalid intervals where $a > b$ (e.g., interval $[2.0, 1.0]$).

---

## 2. [Applied Interpolation](<examples/example_interpolator_applied.cpp>)

### Temperature Table Look-up
* **Tested:** Estimating values between discrete, real-world data points.
* **Given Data (Hours):** $6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0$
* **Given Data (Temp °C):** $14.2, 17.8, 22.5, 26.1, 27.4, 24.9, 20.3$
* **Evaluation Points:** $7.0, 11.5, 15.25$
* **Tested Exception:** Rejection of extrapolation at $20.0$ (outside the measured table).

### Node Distribution Error Comparison (Runge's Phenomenon)
* **Tested:** Comparing the maximum piecewise linear interpolation error between equally spaced nodes and Chebyshev nodes.
* **Equation:** $f(x) = \frac{1}{1 + 25x^2}$
* **Interval:** $[-1.0, 1.0]$
* **Sample Sizes ($n$):** $5, 9, 17, 33$
* **Error Metric:** Maximum absolute error evaluated across $2001$ intermediate test points.
* **Expected Result:** Equal spacing yields a smaller error for piecewise linear interpolation because Chebyshev nodes leave wider gaps in the center of the interval.