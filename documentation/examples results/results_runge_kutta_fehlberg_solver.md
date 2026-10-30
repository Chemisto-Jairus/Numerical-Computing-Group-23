# Runge-Kutta-Fehlberg (RKF45) Solver Examples Documentation

## 1. [Adaptive Step Size Control](../../examples/example_runge_kutta_fehlberg_solver.cpp)

### Tolerance-Based Error Control
* **Tested:** How effectively the solver adapts its internal steps to meet a user-defined accuracy tolerance.
* **Equation:** $\frac{dy}{dt} = -2y$
* **Initial Condition:** $y(0) = 1.0$
* **Evaluation Point:** $t = 1.0$ (Initial step $h = 0.1$)
* **Tested Tolerances:** $10^{-3}, 10^{-5}, 10^{-7}, 10^{-9}$
* **Expected Result:** The solver automatically increases the number of steps to guarantee the final error remains below the specified tolerance.

### Dynamic Step Adjustment
* **Tested:** The algorithm's ability to shrink step sizes during rapid changes and expand them during smooth periods.
* **Equation:** $\frac{dy}{dt} = -50(y - \cos t)$
* **Initial Condition:** $y(0) = 0.0$
* **Interval:** $t = 0$ to $1$ (Tolerance $= 10^{-6}$, Initial step $= 0.01$)
* **Expected Result:** The solver takes very small steps initially as $y$ jumps to track $\cos(t)$, then takes larger steps once the curve smooths out.

### Direct Value Extraction (`rkf45_value`)
* **Tested:** Computing only the final value without storing the full trajectory.
* **Tested 1:** $\frac{dy}{dt} = y - t$, $y(0) = 2.0$. Evaluated at $t = 1.0$. Expected: $e + 2.0$
* **Tested 2:** $\frac{dy}{dt} = ty$, $y(0) = 1.0$. Evaluated at $t = 2.0$. Expected: $e^2$
* **Tolerance:** $10^{-9}$

### Invalid Input Exceptions
* **Tested (Tolerance):** Rejection of a $0.0$ tolerance constraint.
* **Tested (Step Size):** Rejection of a negative initial step size ($-0.1$).
* **Tested (Singularity):** Rejection when the solution blows up to infinity within the integration interval (e.g., $\frac{dy}{dt} = y^2$ evaluating at $t = 2.0$).

---

## 2. [Applied Models](../../examples/example_runge_kutta_fehlberg_solver_applied.cpp)

### Skydiver with Rapidly Opening Parachute
* **Tested:** Modeling a physical system with a sudden, rapid parameter change to see if the solver automatically catches the event.
* **Equation:** $\frac{dv}{dt} = 9.81 - \text{drag}(t) \cdot v^2$
* **Drag Function:** $\text{drag}(t) = 0.004 + \frac{0.396}{1 + e^{-10(t - 10)}}$ (S-curve centered at $t = 10$)
* **Interval:** $t = 0$ to $20$ seconds (Tolerance $= 10^{-6}$, Initial step $= 0.5$)
* **Expected Result:** The algorithm naturally detects the parachute opening at $t = 10$ and drastically shrinks its step size to maintain accuracy. The final speed approaches terminal velocity $\sqrt{9.81 / 0.4}$.

### Drug Elimination (Michaelis-Menten Kinetics)
* **Tested:** Solving a non-linear biological elimination model.
* **Equation:** $\frac{dC}{dt} = \frac{-10C}{2 + C}$
* **Initial Condition:** $C(0) = 20.0$ mg/L
* **Evaluation Points:** $t = 0.5, 1.0, 1.5, 2.0, 3.0$ hours (Tolerance $= 10^{-9}$)
* **Expected Result:** Captures near-constant drug removal while concentration is high, which smoothly transitions into exponential decay as the concentration drops.