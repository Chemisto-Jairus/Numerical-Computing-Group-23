# Conjugate Gradient Solver Tests Documentation

## 1. Unit Tests (`test_conjugate_gradient_solver.cpp`)

### Accuracy on Standard Problems

* **Tested (2x2 System):** Symmetric Positive Definite (SPD) matrix $A = \begin{bmatrix} 4 & 1 \\ 1 & 3 \end{bmatrix}$, $b = \begin{bmatrix} 1 \\ 2 \end{bmatrix}$
  * **Expected Value:** Converges to exact solution $x = \begin{bmatrix} 1/11 \\ 7/11 \end{bmatrix}$.

* **Tested (3x3 System):** SPD matrix $A = \begin{bmatrix} 4 & 1 & 0 \\ 1 & 3 & 1 \\ 0 & 1 & 2 \end{bmatrix}$, $b = \begin{bmatrix} 6 \\ 10 \\ 8 \end{bmatrix}$
  * **Expected Value:** Converges to exact solution $x = \begin{bmatrix} 1 \\ 2 \\ 3 \end{bmatrix}$.

* **Tested (Diagonal Matrix):** $A = \text{diag}(2, 4, 8)$, $b = \begin{bmatrix} 2 \\ 2 \\ 2 \end{bmatrix}$
  * **Expected Value:** Solves directly as $x_i = b_i / A_{ii}$, yielding $x = \begin{bmatrix} 1 \\ 0.5 \\ 0.25 \end{bmatrix}$.

* **Tested (Large Sparse System):** $50 \times 50$ tridiagonal second-difference matrix with $b = \begin{bmatrix} 1 & 0 & \dots & 0 & 1 \end{bmatrix}^T$.
  * **Expected Result:** Successfully converges to $x = \begin{bmatrix} 1 & 1 & \dots & 1 \end{bmatrix}^T$ within exactly $n=50$ iterations, matching theoretical CG behavior.

### Initial Guess Behaviors (`solve_from`)

* **Tested (Exact Initial Guess):** Providing the exact solution as $x_0$.
  * **Expected Result:** Returns immediately without taking any iteration steps.

* **Tested (Rough Initial Guess):** Providing a very poor guess $x_0 = \begin{bmatrix} 10 \\ -10 \end{bmatrix}$ for the 2x2 system.
  * **Expected Result:** Overcomes the poor start and successfully converges to the correct answer.

### Boundaries and Edge Cases

* **Tested (1x1 System):** $5x = 10$.
  * **Expected Value:** $2.0$.

* **Tested (Zero Vector):** System with a zero right-hand side ($b = 0$).
  * **Expected Value:** Immediately returns the zero vector ($x = 0$).

* **Tested (Input Immutability):** Matrix $A$ and vector $b$ passed into the solver.
  * **Expected Result:** Asserts that the solver does not alter the caller's original matrix or vector data during iteration.

### Invalid Input and Divergence Exceptions

* **Tested (Invalid Dimensions):** Rejection of empty matrices, non-square matrices, mismatched $b$ vector sizes, and mismatched $x_0$ guess sizes.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Invalid Properties & Parameters):** Non-symmetric matrices, $tol = 0$, $tol = \text{NaN}$, and $max\_iter = 0$.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Definite Requirements):** Providing a symmetric but indefinite matrix (e.g., $\begin{bmatrix} 1 & 2 \\ 2 & 1 \end{bmatrix}$) or a negative definite matrix.
  * **Expected Result:** Throws `std::runtime_error` as Conjugate Gradient mathematically requires a positive definite matrix to converge.

* **Tested (Failure to Converge):** Running the 50-variable system with a cap of $max\_iter = 2$.
  * **Expected Result:** Throws `std::runtime_error` because it cannot converge in so few steps.