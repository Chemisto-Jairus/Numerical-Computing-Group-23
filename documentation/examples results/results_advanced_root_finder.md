# Advanced Root Finder Examples Documentation

## 1. [Standard Root Finding](../../examples/example_advanced_root_finder.cpp)

### Secant Method Baseline
* **Tested:** $f(x) = x^2 - 4.0$
  * **Guesses:** $1.0, 3.0$
  * **Tolerance:** $10^{-12}$
  * **Expected Value:** $2.0$
* **Tested:** $f(x) = x^3 - 2x - 5.0$
  * **Guesses:** $2.0, 3.0$
  * **Tolerance:** $10^{-12}$
  * **Expected Value:** $2.0945514815423265$
* **Tested:** $f(x) = e^x - 3.0$
  * **Guesses:** $0.0, 2.0$
  * **Tolerance:** $10^{-12}$
  * **Expected Value:** $\ln(3.0)$

### Fixed-Point Iteration Baseline
* **Tested:** $g(x) = \sqrt{x + 2.0}$
  * **Initial Guess:** $1.0$
  * **Tolerance:** $10^{-12}$
  * **Expected Value:** $2.0$
* **Tested:** $g(x) = \cos(x)$
  * **Initial Guess:** $1.0$
  * **Tolerance:** $10^{-12}$
  * **Expected Value:** $0.7390851332151607$
* **Tested:** $g(x) = e^{-x}$
  * **Initial Guess:** $0.5$
  * **Tolerance:** $10^{-12}$
  * **Expected Value:** $0.5671432904097838$

### Divergence and Algorithm Stability (Choice of $g$)
* **Tested:** Comparison of converging vs. diverging rearrangements for the root of $x^2 - x - 2 = 0$.
* **Converging Equation:** $g(x) = \sqrt{x + 2.0}$ (because $|g'(2)| < 1$)
  * **Initial Guess:** $1.0$
  * **Expected Result:** Successfully converges to $2.0$
* **Diverging Equation:** $g(x) = x^2 - 2.0$ (because $|g'(2)| > 1$)
  * **Initial Guess:** $2.5$
  * **Expected Result:** Throws an exception or diverges away from the root.

### Invalid Input Exceptions
* **Tested (Secant):** Rejection of identical initial guesses ($1.0$ and $1.0$).
* **Tested (Secant):** Rejection of max iterations set to $0$.
* **Tested (Fixed-Point):** Rejection of negative tolerance ($-1.0$).

---

## 2. [ Applied Problems](../../examples/example_advanced_root_finder_applied.cpp)

### Loan Interest Rate
* **Tested:** Finding the monthly interest rate given a principal, monthly payment, and term using the Secant Method.
* **Equation:** $f(r) = \frac{P \cdot r}{1 - (1 + r)^{-n}} - \text{payment}$
* **Values:** Principal $P = 20000.0$, Payment $= 400.0$, Months $n = 60$
* **Guesses:** $0.001, 0.02$
* **Tolerance:** $10^{-14}$
* **Expected Result:** Calculates the monthly rate $r$ and annual rate ($12r$).

### Babylonian Square Roots
* **Tested:** Computing square roots using Fixed-Point Iteration.
* **Equation:** $g(x) = \frac{x + a / x}{2}$
* **Values of $a$:** $2.0, 10.0, 144.0, 10^6$
* **Initial Guess:** $a$
* **Tolerance:** $10^{-12}$
* **Expected Result:** Matches the output of the standard `std::sqrt(a)` function.

### Spherical Tank Water Depth
* **Tested:** Finding the depth of water in a spherical tank given its volume using the Secant Method.
* **Equation:** $f(h) = \frac{\pi h^2 (3R - h)}{3} - V$
* **Values:** Radius $R = 1.0$ m, Volume $V = 1.5$ m³
* **Guesses:** $0.5, 1.5$
* **Tolerance:** $10^{-12}$
* **Expected Result:** Successfully calculates the depth $h$ in meters.