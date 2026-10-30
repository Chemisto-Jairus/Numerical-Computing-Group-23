# Final Week Progress Report

## Completed

**Member 1 (Chemisto Jairus - Lead):** 

- I implemented part of the Numerical differentiator module.
- I uploaded the updated Helpers report for all the modules. Find it at [shared_helpers](../documentation/modules/shared_helpers.md)
- I wrote the numcomp file that includes all our header files. Find it at [numcomp](../include/numerical-computing/numcomp.hpp)
- I added the multi step helper functions to be used by the Adams-Bashforth and Adams-Moulton methods. Find them at [multistep](../src/internal)
- I added the trigiagonal helper functions to be used for tridiagonal linear systems. Find them at [text](../src/internal)
- I documented my modules 
- I added the examples and tests as well as their results for Numerical differentiator.

**Member 2 (Batte Derrick Calvin):** 

I made the following changes to my modules

- CubicSpline now uses the shared tridiagonal solver instead of its own copy.
- Added Chebyshev approximation to Interpolator.
- Add full Doxygen documentation to Interpolator, CubicSpline and MatrixSolver.
- Sources, tests and examples: file headers and comments that explain the algorithm steps.
- I helped Pauline check the build on macOS as these were not easily available. (macOS machines)
- I built the Doxyfile that easily documents the project for other programmers to understand.


**Member 3 (Aogon Sharon):** 

- Implemented finite difference solver module.
- I implemented the examples for both my modules.
- I documented all the tests and examples and their results.
- I checked if the examples and tesrs work.


**Member 4 (Bakalu Valeria):** 

- I implemented the Conjugate gradient module.
- I uploaded some tests for that module as well.
- I added comments to the conjugate gradient and advanced root finder.
- I finished documenting all my modules.


**Member 5 (Wasswa George Mpanga):** 

- I implemented the first part of Runge-Kutta-Fehlberg-Solver.
- I documented all my modules.
- I implemented the tests and examples of the RungeKuttaFehlbergSolver.


**Member 6 (Mwanje Ian):** 

- I added comments to the ode stepper helpers
- I implemented the Adam's bashforth module
- I documented Basic ode solver  and Adams bashforth modules
- I wrote their examples and tests
- I am finalizing my modules and documenting my contributions

**Member 7 (Muwanguzi Samuel):** 

- I implemented the first part of adams_moulton_solver

**Member 8 (Kyeyune Praise Pauline):** 

- I updated the auto tester to include the missing modules soon to be added.
- I documented the testing framework.
- I documented the test results.

**Member 9 (Nabakooza Bridget Maria):** 

- I wrote the examples list the members are going to create, some were already completed.
- I started upgrading the main app, rather a new version of the library using helpers.
- I completed the numcomp_app that uses our library.


## In Progress

**Member 9 (Nabakooza Bridget Maria):** 

- Still building the UI of the app. (Done)

**Member 1 (Chemisto Jairus - Lead):** 

- Finalizing the Numerical Differentiator.
- Documenting my modules.

**Member 2 (Batte Derrick Calvin):** 

- Helping other members who need a hand and making sure everything is running efficiently.

**Member 4 (Bakalu Valeria):** 

- I am building the examples and handling the documentation of all my Modules

**Member 5 (Wasswa George Mpanga):** 

- I am writing examples and tests for the Runge-Kutta-Fehlberg-Solver. (done)
- I am documenting all my modules properly and checking if they run efficiently. (done)

**Member 3 (Aogon Sharon):**

- Writing tests and examples for finte difference solver module.
- Documenting all my implemented modules.
- Ensuring all the modules' test run efficiemtly.

## Challenges/Blockers

**Member 6 (Mwanje Ian):**

- My pc has issues so my friend Samuel helped me push the work, but it is now fixed


## AI Use

**Member 9 (Nabakooza Bridget Maria):** 

- Tool: Claude 
- Purpose: To learn how to add helpers to the main.cpp file i had built in the previous weeks.
- Reason: I did not know how to efficiently do this and i needed to get it done in time.

**Member 7 (Muwanguzi Samuel):** 

- Tool: Claude
- Purpose:  Helped me get the basic idea of the adams moulton method and identify and fix bugs
- Reason: To understand the solver and to help find out the cause of the errors

**Member 2 (Batte Derrick Calvin):** 

- Tool: Claude
- Purpose: Documenting using Doxy
- Reason: I asked claude how to efficiently document the project besides what we have done in regards to the rule of documenting, and it reccomended using the Doxyfile. So it taught me how to use it and made the count of how many doxy comments were made.

**Member 4 (Bakalu Valeria):** 

- Tool: Claude
- Purpose: Utilizing Doxygen commenting
- Reason: I wanted to learn how to effectively use the comments