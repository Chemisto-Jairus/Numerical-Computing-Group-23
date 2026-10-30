# Adams-Bashforth Solver Examples Documentation

## 1. Standard Explicit Multistep Methods (`example_adams_bashforth_solver.cpp`)

### Exponential Decay Baseline
* **Tested:** Accuracy of AB2 (2-step) and AB4 (4-step) methods against an exact decay curve.
* **Equation:** $\frac{dy}{dt} = -2y$
* **Initial Condition:** $y(0) = 1.0$
* **Interval:** $[0, 1]$
* **Step Size ($h$):** $0.1$
* **Expected Result:** Numerical approximations closely match the exact solution $y(t) = e^{-2t}$.

### Order of Accuracy
* **Tested:** Convergence rates (error reduction) of AB2 and AB4 at $t = 1.0$ when step size is progressively halved.
* **Equation:** $\frac{dy}{dt} = -2y$
* **Tested Step Sizes ($h$):** $0.1, 0.05, 0.025, 0.0125$
* **Expected Result:** Halving $h$ divides the AB2 error by roughly $4$ (second-order accuracy) and divides the AB4 error by roughly $16$ (fourth-order accuracy).

### Computational Cost Comparison (AB4 vs. RK4)
* **Tested:** Efficiency based on the number of function evaluations needed to achieve fourth-order accuracy.
* **Equation:** $\frac{dy}{dt} = y - t$
* **Initial Condition:** $y(0) = 2.0$
* **Interval:** $[0, 2]$
* **Step Size ($h$):** $0.01$
* **Expected Result:** Both AB4 and RK4 calculate the answer ($e^2 + 3.0$), but AB4 requires about one-quarter of the work (evaluations of $f$) because it reuses previously calculated slopes instead of recalculating intermediate stages.

### Invalid Input Exceptions
* **Tested (Step Size):** Rejection of negative step sizes ($h = -0.1$).
* **Tested (Time Limits):** Rejection of backward intervals where $t_{\text{end}} < t_0$.
* **Tested (Singularities):** Rejection and safety handling of solutions that blow up to infinity (e.g., $\frac{dy}{dt} = y^2$ integrated past its asymptote at $t = 1.0$).

---

## 2. Applied Models (`example_adams_bashforth_solver_applied.cpp`)

### Skydiver with Air Resistance
* **Tested:** Modeling velocity over time including quadratic drag to find terminal velocity using AB4.
* **Equation:** $\frac{dv}{dt} = g - kv^2$
* **Values:** Gravity $g = 9.81$ m/s², Drag coefficient $k = 0.0039$, Initial velocity $v(0) = 0.0$ m/s.
* **Interval:** $t = 0$ to $30$ seconds with $h = 0.1$.
* **Expected Result:** Matches the exact curve $v(t) = v_{\text{terminal}} \tanh(\sqrt{gk}t)$ and approaches the terminal velocity $\sqrt{g/k}$.

### Bacterial Growth (Logistic Model)
* **Tested:** Comparing AB2 and AB4 accuracies on a logistic population model approaching carrying capacity.
* **Equation:** $\frac{dN}{dt} = 0.8N(1 - \frac{N}{1000})$
* **Initial Condition:** $N(0) = 1.0$ (thousands of cells)
* **Evaluation Point:** $t = 12.0$ hours with $h = 0.05$.
* **Expected Result:** Both methods approximate the exact solution $N(t) = \frac{1000}{1 + 999e^{-0.8t}}$, but the higher-order AB4 method produces a result far closer to the exact value for the same step size.