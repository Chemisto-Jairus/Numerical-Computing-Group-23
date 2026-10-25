# Advanced ODE Solver Examples Documentation

## 1.  [Standard Runge-Kutta Methods](../../examples/example_advanced_ode_solver.cpp)

### Exponential Decay Baseline
* **Tested:** Accuracy of RK3 and RK4 solving a standard decay model.
* **Equation:** $\frac{dy}{dt} = -2y$
* **Initial Condition:** $y(0) = 1.0$
* **Step Size ($h$):** $0.1$
* **Evaluation Interval:** $[0.0, 1.0]$
* **Expected Result:** Both methods approximate the exact solution $y(t) = e^{-2t}$.

### Order of Accuracy Verification
* **Tested:** Error reduction scaling at $t = 1.0$ when halving the step size.
* **Equation:** $\frac{dy}{dt} = -2y$
* **Initial Condition:** $y(0) = 1.0$
* **Step Sizes Tested ($h$):** $0.2, 0.1, 0.05, 0.025$
* **Expected Result:** Halving $h$ reduces the error of RK3 by a factor of $\approx 8$ and RK4 by a factor of $\approx 16$.

### Alternative Right-Hand Sides
* **Tested:** Sine function integration (RK4).
  * **Equation:** $\frac{dy}{dt} = -\sin(t)$
  * **Conditions:** $y(0) = 1.0, t = \pi, h = 0.05$
  * **Expected Value:** $-1.0$
* **Tested:** Polynomial integration (RK4).
  * **Equation:** $\frac{dy}{dt} = 4t^3$
  * **Conditions:** $y(0) = 0.0, t = 2.0, h = 0.05$
  * **Expected Value:** $16.0$
* **Tested:** Linear ODE (RK3).
  * **Equation:** $\frac{dy}{dt} = y - t$
  * **Conditions:** $y(0) = 2.0, t = 1.0, h = 0.05$
  * **Expected Value:** $e^1 + 2.0$

### Invalid Input Exceptions
* **Tested:** Rejection of negative step sizes ($h = -0.1$).
* **Tested:** Rejection of zero-length intervals where the start time equals the end time ($t_0 = 1.0, t_{\text{end}} = 1.0$).

---

## 2. [Applied ODE Models](../../examples/example_advanced_ode_solver_applied.cpp) 

### RC Circuit Charging
* **Tested:** Modeling voltage over time in a charging capacitor through a resistor.
* **Equation:** $\frac{dV}{dt} = \frac{12 - V}{\tau}$
* **Constants:** Time constant $\tau (RC) = 0.5$ seconds, Source $= 12.0$ Volts.
* **Initial Condition:** $V(0) = 0.0$
* **Step Size ($h$):** $0.05$
* **Evaluation Interval:** $[0.0, 2.5]$
* **Expected Result:** RK4 output closely matches the exact exact analytical voltage $V(t) = 12(1 - e^{-t/0.5})$.

### Pharmacokinetics (Drug Infusion)
* **Tested:** Modeling drug concentration under constant infusion and proportional elimination.
* **Equation:** $\frac{dC}{dt} = 5.0 - 0.25C$
* **Initial Condition:** $C(0) = 0.0$
* **Step Size ($h$):** $0.5$ hours
* **Evaluation Time:** $t = 12.0$ hours
* **Expected Result:** Both RK3 and RK4 accurately evaluate to the exact concentration $C(12) = 20(1 - e^{-0.25 \times 12})$.
* **Expected Steady State:** Evaluates to $20.0$ mg/L over infinite time ($5.0 / 0.25$).