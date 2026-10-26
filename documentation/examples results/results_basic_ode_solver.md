# Basic ODE Solver Examples Documentation

## 1.  [Standard ODE Solving](../../examples/example_basic_ode_solver.cpp)
### Exponential Growth Baseline
* **Tested:** Accuracy of Euler and Heun methods for a standard growth model over a full curve.
* **Equation:** $\frac{dy}{dt} = y$
* **Initial Condition:** $y(0) = 1.0$
* **Step Size ($h$):** $0.1$
* **Expected Result:** Both methods approximate the exact curve $y(t) = e^t$.

### Order of Accuracy
* **Tested:** The rate of error reduction at $t = 1.0$ when step size is halved.
* **Equation:** $\frac{dy}{dt} = y$
* **Tested Step Sizes ($h$):** $0.1, 0.05, 0.025, 0.0125$
* **Expected Result:** Halving $h$ halves the Euler method's error (First-order accuracy) and quarters the Heun method's error (Second-order accuracy).

### Additional Right-Hand Sides
* **Tested (Heun):** $\frac{dy}{dt} = \cos(t)$
  * **Initial Condition:** $y(0) = 0.0$
  * **Evaluation Point:** $t = 2.0$ with $h = 0.01$
  * **Expected Value:** $\sin(2.0)$
* **Tested (Heun):** $\frac{dy}{dt} = -2ty$
  * **Initial Condition:** $y(0) = 1.0$
  * **Evaluation Point:** $t = 2.0$ with $h = 0.01$
  * **Expected Value:** $e^{-4.0}$
* **Tested (Euler):** $\frac{dy}{dt} = 2t$
  * **Initial Condition:** $y(0) = 0.0$
  * **Evaluation Point:** $t = 2.0$ with $h = 0.01$
  * **Expected Value:** $4.0$

### Invalid Input Exceptions
* **Tested (Step Size):** Rejection of $h = 0.0$.
* **Tested (Time Limits):** Rejection of intervals where the end time is before the start time ($t_{\text{end}} < t_0$).
* **Tested (Performance Safety):** Rejection of excessively small step sizes that trigger too many steps (e.g., $h = 10^{-8}$).

---

## 2. [ Applied Models](../../examples/example_basic_ode_solver_applied.cpp)

### Newton's Law of Cooling
* **Tested:** Calculating the temperature of a cooling cup of tea over time using the Heun method.
* **Equation:** $\frac{dT}{dt} = -k(T - T_{\text{room}})$
* **Values:** Cooling constant $k = 0.08$, Room temperature $T_{\text{room}} = 22.0$ °C, Initial temp $T(0) = 90.0$ °C.
* **Interval:** $t = 0$ to $30$ minutes with $h = 0.5$.
* **Expected Result:** The numerical output closely tracks the exact analytical solution $T(t) = 22.0 + 68.0e^{-0.08t}$.

### Logistic Population Growth
* **Tested:** Modeling population growth approaching a carrying capacity using both Euler and Heun methods.
* **Equation:** $\frac{dP}{dt} = rP(1 - \frac{P}{K})$
* **Values:** Growth rate $r = 0.4$, Carrying capacity $K = 5000.0$, Initial population $P(0) = 100.0$.
* **Evaluation Point:** $t = 20.0$
* **Tested Step Sizes ($h$):** $0.1$ (Euler/Heun) and $0.01$ (Heun).
* **Expected Result:** Results align with the exact logistic function $P(t) = \frac{K}{1 + (\frac{K}{P_0} - 1)e^{-rt}}$.