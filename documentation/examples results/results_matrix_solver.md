# Matrix Solver Examples Documentation

## 1.  [Standard Linear Systems](examples/example_matrix_solver.cpp)

### 3x3 System Baseline
* **Tested:** Solving a standard 3x3 system using both Gaussian Elimination and LU Decomposition.
* **Coefficient Matrix ($A$):** 
  $A = \begin{bmatrix} 2 & 1 & -1 \\ -3 & -1 & 2 \\ -2 & 1 & 2 \end{bmatrix}$
* **Constants Vector ($b$):** 
  $b = \begin{bmatrix} 8 \\ -11 \\ -3 \end{bmatrix}$
* **Expected Solution ($x$):** $x_1 = 2, x_2 = 3, x_3 = -1$

### Zero Pivot Handling (Automatic Row Swapping)
* **Tested:** Algorithm's ability to handle a $0$ in the first pivot position during Gaussian Elimination.
* **Coefficient Matrix ($A$):** 
  $A = \begin{bmatrix} 0 & 2 & 1 \\ 1 & 1 & 1 \\ 2 & 1 & 0 \end{bmatrix}$
* **Constants Vector ($b$):** 
  $b = \begin{bmatrix} 7 \\ 6 \\ 4 \end{bmatrix}$
* **Expected Solution ($x$):** $x_1 = 1, x_2 = 2, x_3 = 3$

### 4x4 System Reversing
* **Tested:** Solving a larger 4x4 system using LU Decomposition where the target vector $b$ is dynamically generated from a known exact answer.
* **Coefficient Matrix ($A$):** 
  $A = \begin{bmatrix} 4 & -1 & 0 & 2 \\ 1 & 5 & -2 & 0 \\ 0 & 2 & 6 & -1 \\ 3 & 0 & 1 & 7 \end{bmatrix}$
* **Known Exact Solution ($x$):** $1.0, -2.0, 3.0, 0.5$
* **Expected Result:** LU solver successfully recovers the known exact solution $x$.

### Invalid Input Exceptions
* **Tested (Singular Matrix):** Rejection of matrices with linearly dependent rows (e.g., Row 2 is exactly $2 \times$ Row 1).
* **Tested (Dimensions):** Rejection of non-square matrices (e.g., $2 \times 3$ matrix).
* **Tested (Mismatch):** Rejection of inputs where vector $b$ size does not match matrix $A$ size.

---

## 2.  [Applied Linear Systems](examples/example_matrix_solver_applied.cpp)

### Kirchhoff's Voltage Law (3-Loop Circuit)
* **Tested:** Finding loop currents ($I_1, I_2, I_3$) using LU Decomposition.
* **Resistance Matrix ($R$ in Ohms):** 
  $R = \begin{bmatrix} 15 & -5 & 0 \\ -5 & 20 & -10 \\ 0 & -10 & 25 \end{bmatrix}$
* **Voltage Vector ($V$ in Volts):** 
  $V = \begin{bmatrix} 10 \\ 0 \\ 5 \end{bmatrix}$
* **Expected Result:** Outputs the correct loop currents in Amperes.

### Curve Fitting (Parabola through 3 points)
* **Tested:** Finding coefficients $a, b, c$ for the parabola $y = a + bx + cx^2$ using Gaussian Elimination.
* **Given Points:** $(1, 4), (2, 3), (4, 9)$
* **Vandermonde Matrix ($V$):** 
  $V = \begin{bmatrix} 1 & 1 & 1 \\ 1 & 2 & 4 \\ 1 & 4 & 16 \end{bmatrix}$
* **Constants Vector ($y$):** 
  $y = \begin{bmatrix} 4 \\ 3 \\ 9 \end{bmatrix}$
* **Expected Result:** Outputs coefficients that perfectly reconstruct $y(4) = 9.0$.

### Alloy Mixing Problem
* **Tested:** Calculating the required litres of three different alloys to reach a specific target composition using Gaussian Elimination.
* **Alloy Compositions (% Copper / Zinc / Tin):**
  * Alloy A: $70\% / 20\% / 10\%$
  * Alloy B: $50\% / 40\% / 10\%$
  * Alloy C: $40\% / 40\% / 20\%$
* **Composition Matrix ($A$):** 
  $A = \begin{bmatrix} 0.70 & 0.50 & 0.40 \\ 0.20 & 0.40 & 0.40 \\ 0.10 & 0.10 & 0.20 \end{bmatrix}$
* **Target Volume ($b$):** $100$ Litres total ($55$L Copper, $32$L Zinc, $13$L Tin)
* **Expected Result:** Outputs exact litres of each alloy needed, verifying that the sum equals exactly $100$L.