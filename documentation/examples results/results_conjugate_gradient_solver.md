# Conjugate Gradient Solver Examples Documentation

## 1. Standard Iterative Solving (`example_conjugate_gradient_solver.cpp`)

### 3x3 Symmetric Positive Definite (SPD) System Baseline
* **Tested:** Solving a standard $3 \times 3$ SPD system of linear equations.
* **Matrix ($A$):** 
  $$ \begin{pmatrix} 4 & 1 & 0 \\ 1 & 3 & 1 \\ 0 & 1 & 2 \end{pmatrix} $$
* **Vector ($b$):** $\{6, 10, 8\}$
* **Parameters:** Tolerance = $10^{-12}$, Max Iterations = $100$
* **Expected Result:** Solution vector $x = \{1, 2, 3\}$.

### Tridiagonal System Convergence Rate
* **Tested:** Finding the minimum number of iterations required to reach a specific tolerance for scaling problem sizes.
* **Matrix ($A$):** Tridiagonal with $2.0$ on the main diagonal and $-1.0$ on the sub/super-diagonals.
* **Vector ($b$):** All elements equal to $1.0$.
* **Tested Sizes ($n$):** $2, 5, 10, 20, 40$ unknowns.
* **Parameters:** Tolerance = $10^{-10}$
* **Expected Result:** The algorithm successfully converges in $\le n$ iterations, proving the theoretical limit for CG methods.

### Comparison with Direct Gaussian Elimination
* **Tested:** Verifying the accuracy of the iterative CG solver against an exact direct solver.
* **Matrix ($A$):** 
  $$ \begin{pmatrix} 10 & -1 & 2 & 0 \\ -1 & 11 & -1 & 3 \\ 2 & -1 & 10 & -1 \\ 0 & 3 & -1 & 8 \end{pmatrix} $$
* **Vector ($b$):** $\{6, 25, -11, 15\}$
* **Parameters:** Tolerance = $10^{-12}$
* **Expected Result:** The numerical output from the CG solver matches the output of the standard Gaussian Elimination solver.

### Invalid Input Exceptions
* **Tested (Symmetry):** Rejection of non-symmetric matrices (e.g., modifying off-diagonal elements so $A \neq A^T$).
* **Tested (Definiteness):** Rejection of symmetric but non-positive-definite matrices (e.g., $A = \begin{pmatrix} 1 & 2 \\ 2 & 1 \end{pmatrix}$, which has a negative eigenvalue).
* **Tested (Tolerance):** Rejection of a tolerance value of $0.0$.

---

## 2. Applied SPD Systems (`example_conjugate_gradient_solver_applied.cpp`)

### Spring Chain Displacements (Stiffness Matrix)
* **Tested:** Calculating the displacements of a chain of three masses connected by springs.
* **Matrix ($K$, Stiffness):**
  $$ \begin{pmatrix} 300 & -200 & 0 \\ -200 & 400 & -200 \\ 0 & -200 & 300 \end{pmatrix} $$
* **Vector ($u$, Load):** $\{10, 20, 10\}$ N
* **Parameters:** Tolerance = $10^{-12}$, Max Iterations = $50$
* **Expected Result:** Calculates the displacement (in meters) for each mass. Mass 1 and Mass 3 displace by the exact same amount due to symmetry.

### Heated Rod (Steady Temperature Distribution)
* **Tested:** Solving a 1D heat equation using a finite difference method across $99$ inner points.
* **Equation:** $-T'' = 1$
* **Matrix ($A$):** Tridiagonal system reflecting the finite difference stencil on a grid of $n=99$ points ($h = \frac{1}{100}$).
* **Vector ($b$):** Elements scaled to $h^2$.
* **Expected Result:** The numerical output matches the exact analytical solution $T(x) = \frac{x(1 - x)}{2}$ at various points along the rod, converging in 99 iterations or fewer.

### Line of Best Fit (Linear Regression)
* **Tested:** Finding the line of best fit for a set of measured data points by solving the normal equations.
* **Equation:** $y = c_0 + c_1x$
* **Data points $(x, y)$:** $(0, 1.1), (1, 2.9), (2, 5.2), (3, 6.8), (4, 9.1)$
* **Method:** Transforms data into the SPD normal equations $(X^T X)c = X^T y$.
* **Parameters:** Tolerance = $10^{-12}$, Max Iterations = $10$
* **Expected Result:** Successfully extracts the intercept $c_0$ and slope $c_1$ defining the line of best fit.