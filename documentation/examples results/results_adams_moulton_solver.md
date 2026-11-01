# Adams-Moulton Solver Examples Documentation

## 1. [Standard Implicit Multistep Methods](../../examples/example_adams_moulton_solver.cpp)

### Exponential Decay Baseline
* **Tested:** Accuracy of AM2 (implicit trapezoidal) and AM4 methods against an exact decay curve.
* **Equation:** $\frac{dy}{dt} = -2y$
* **Initial Condition:** $y(0) = 1.0$
* **Step Size ($h$):** $0.1$
* **Expected Result:** Both methods accurately track the exact analytical solution $y(t) = e^{-2t}$.

### Implicit vs. Explicit Error Comparison
* **Tested:** Comparison of error constants between explicit (AB) and implicit (AM) methods of the same order at $t = 1.0$.
* **Equation:** $\frac{dy}{dt} = -2y$
* **Tested Step Sizes ($h$):** $0.1, 0.05, 0.025$
* **Expected Result:** While AM2 and AB2 share the same order (and similarly for AM4 and AB4), the implicit AM methods yield significantly smaller error constants.

### Fast-Decaying (Stiff) Problem Stability
* **Tested:** Algorithm stability on a stiff differential equation.
* **Equation:** $\frac{dy}{dt} = -50(y - 1)$
* **Initial Condition:** $y(0) = 0.0$
* **Evaluation Point:** $t = 1.0$ with $h = 0.03$
* **Expected Result:** The explicit AB2 method becomes unstable and its error explodes, whereas the implicit AM2 method remains stable and closely approximates the exact value $1 - e^{-50}$.

### Invalid Input Exceptions
* **Tested (Step Size):** Rejection of $h = 0.0$.
* **Tested (Corrector Divergence):** Rejection when the fixed-point corrector fails to converge (e.g., evaluating $\frac{dy}{dt} = -50y$ with an overly large step size $h = 0.1$).

---

## 2. [ Applied Models](../../examples/example_adams_moulton_solver_applied.cpp)

### Second-Order Chemical Reaction
* **Tested:** Modeling the concentration of a reactant over time.
* **Equation:** $\frac{dC}{dt} = -0.5C^2$
* **Initial Condition:** $C(0) = 2.0$ mol/L
* **Interval:** $t = 0$ to $10$ minutes with $h = 0.1$
* **Expected Result:** The numerical output using AM4 accurately follows the exact concentration curve $C(t) = \frac{2}{1 + t}$.

### Draining Tank (Torricelli's Law)
* **Tested:** Calculating the water level of a draining tank using AM2 and AM4.
* **Equation:** $\frac{dh}{dt} = -0.2\sqrt{\max(h, 0)}$
* **Initial Condition:** $h(0) = 4.0$ m
* **Evaluation Point:** $t = 10.0$ minutes with $h = 0.1$
* **Expected Result:** Both methods align with the exact water level $h(t) = (2 - 0.1t)^2$. The tank is expected to empty entirely at $t = 20.0$ minutes.