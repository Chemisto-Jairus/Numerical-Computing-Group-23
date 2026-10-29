# Matrix Solver Tests Documentation

## 1. Unit Tests (`test_matrix_solver.cpp`)

### Accuracy on Standard Problems
*(Note: All valid systems are tested simultaneously against both Gaussian Elimination and LU Decomposition to ensure parity).*

* **Tested (2x2 System):** $2x + y = 5$ and $x + 3y = 10$.
  * **Expected Value:** $x = [1.0, 3.0]^T$.

* **Tested (3x3 System):** Standard dense 3x3 textbook system.
  * **Expected Value:** $x = [2.0, 3.0, -1.0]^T$.

* **Tested (4x4 System):** Dense 4x4 system where the right-hand side $b$ is mathematically pre-calculated from a known solution vector $x$.
  * **Expected Value:** Recovers the exact known vector $x = [1.0, -2.0, 3.0, 0.5]^T$.

* **Tested (Solver Parity):** Evaluates an arbitrary 3x3 system through both solvers independently.
  * **Expected Result:** Gaussian elimination and LU decomposition yield identically matching solution vectors (within floating-point tolerance).

### Boundaries and Edge Cases

* **Tested (1x1 System):** A single equation $4x = 8$.
  * **Expected Value:** $2.0$.

* **Tested (Zero First Pivot):** A 3x3 system where the top-left element $A_{0,0} = 0.0$.
  * **Expected Result:** Successfully identifies the zero pivot, performs the necessary row swaps (partial pivoting) to avoid division by zero, and returns the exact correct answer $x = [1.0, 2.0, 3.0]^T$.

* **Tested (Identity Matrix):** Solving $I x = b$.
  * **Expected Result:** Evaluates perfectly and returns the exact $b$ vector unmodified.

* **Tested (Input Immutability):** Matrix $A$ and vector $b$ passed into the solvers.
  * **Expected Result:** Asserts that neither solver mutates the caller's original matrix or vector data during the elimination/decomposition processes.

### Invalid Input and Singularity Exceptions

* **Tested (Invalid Dimensions & Shapes):** Rejection of empty matrices, non-square matrices (e.g., 2x3), ragged/uneven rows, and $b$ vectors that do not match the matrix size.
  * **Expected Result:** Throws `std::invalid_argument`.

* **Tested (Singular Matrices):** A matrix where one row is a scalar multiple of another (e.g., row 2 is exactly $2 \times$ row 1), meaning no unique solution exists.
  * **Expected Result:** Intercepted by the solver during decomposition/elimination and throws `std::runtime_error`.

* **Tested (All-Zero Matrix):** A square matrix filled entirely with zeros.
  * **Expected Result:** Throws `std::runtime_error`.