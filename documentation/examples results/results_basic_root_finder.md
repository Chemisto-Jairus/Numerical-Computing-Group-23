# Basic Root Finder Examples Documentation

## [1. Standard Equations](../../examples/example_basic_root_finder.cpp)

### Equations and Values Tested

1. **Square Root of 2:**
   * **Equation:** $f(x) = x^2 - 2.0$
   * **Derivative:** $f'(x) = 2.0x$
   * **Bisection:** Interval $[0.0, 2.0]$
   * **Newton-Raphson:** Initial guess $x_0 = 1.0$
   * **Tolerance:** $10^{-12}$
   * **Expected Value:** $\sqrt{2} \approx 1.41421356...$

2. **Cubic Polynomial:**
   * **Equation:** $f(x) = x^3 - x - 2.0$
   * **Derivative:** $f'(x) = 3.0x^2 - 1.0$
   * **Bisection:** Interval $[1.0, 2.0]$
   * **Newton-Raphson:** Initial guess $x_0 = 1.5$
   * **Tolerance:** $10^{-12}$
   * **Expected Value:** $1.5213797068045676$

3. **Transcendental Function:**
   * **Equation:** $f(x) = \cos(x) - x$
   * **Derivative:** $f'(x) = -\sin(x) - 1.0$
   * **Bisection:** Interval $[0.0, 1.0]$
   * **Newton-Raphson:** Initial guess $x_0 = 1.0$
   * **Tolerance:** $10^{-12}$
   * **Expected Value:** $0.7390851332151607$

### Error Handling & Edge Cases
* **Missing Root:** Bisection on $[2.0, 3.0]$ for $f(x) = x^2 - 2$ (No sign change).
* **Zero Tolerance:** Bisection with tolerance $= 0.0$.
* **Zero Derivative:** Newton-Raphson for $x^2 - 2$ starting at $x_0 = 0.0$ (Causes division by zero).


## 2. [Applied Problems](../../examples/example_basic_root_finder_applied.cpp)

Demonstrates how the root-finding algorithms can be used to solve practical physics and economics problems.

### Equations and Values Tested (Applied)

1. **Projectile Motion (Kinematics):**
   * **Scenario:** Finding the landing time of a ball thrown upwards from $1.5$m at $20$m/s.
   * **Equation:** $h(t) = 1.5 + 20.0t - 4.905t^2$
   * **Derivative:** $h'(t) = 20.0 - 9.81t$
   * **Bisection:** Interval $[1.0, 10.0]$
   * **Newton-Raphson:** Initial guess $t_0 = 5.0$
   * **Tolerance:** $10^{-12}$
   * **Expected Value:** $t \approx 4.149$ seconds.

2. **Orbital Mechanics (Kepler's Equation):**
   * **Scenario:** Calculating the eccentric anomaly ($E$) for an orbit.
   * **Parameters:** Eccentricity $e = 0.3$, Mean anomaly $M = 1.2$ rad.
   * **Equation:** $f(E) = E - 0.3\sin(E) - 1.2$
   * **Derivative:** $f'(E) = 1.0 - 0.3\cos(E)$
   * **Newton-Raphson:** Initial guess $E_0 = 1.2$
   * **Tolerance:** $10^{-14}$

3. **Business Economics (Break-Even Point):**
   * **Scenario:** Finding the sales quantity ($q$) where profit equals zero.
   * **Revenue:** $50q$
   * **Cost:** $2000 + 10q + 0.02q^2$
   * **Profit Equation:** $P(q) = 40.0q - 2000.0 - 0.02q^2$
   * **Bisection:** Interval $[0.0, 500.0]$
   * **Tolerance:** $10^{-10}$
   * **Expected Value:** $q \approx 53.5898$ units.

### Algorithm Performance Comparison
The script runs a benchmark on $f(x) = x^2 - 2$ to compare iteration efficiency between methods, capping the `max_iter` at $3, 5, 10, 20, 30,$ and $45$.

