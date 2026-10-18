# Week 3 Progress Report

## Completed
**Member 1 (Chemisto Jairus - Lead):** 
- I finalized the week 2 report. 
- I helped and helping Bridget write the main.cpp file which combines the source files for the modules the other members implemented.
- I fixed logic of basic root finder module and some of the committed last week and fixed some arrnagement issues caused by other members.
- Renamed basic root finder


**Member 2 (Batte Derrick Calvin):** 
- I completed tests for Interpolation feature which Pauline is building an automatic tester for.
- I debugged interpolation module
- I renamed the interpolation files to interpolator.cpp

**Member 3 (Aogon Sharon):** 
- I created some tests for my module of basic integration.
- I debugged and renamed basic integration to basic interpolator so that the repo is uniform and easy to test all modules as requested by Jairus and Pauline

**Member 4 (Bakalu Valeria):** 
- I added tests for my advanced root finder and looked for errors in my code for the module I uploaded last week.

**Member 5 (Wasswa George Mpanga):** 
- I finalized the tests for Advanced Integration.
- I debugged the module
- Jairus helped me rename my advanced integration files 

**Member 6 (Mwanje Ian):** 
- I implemented the basic ODE module, debugged basic ODE solvers, and renamed the files for repo uniformity.
- **Class Structure & Implementation (`BasicODESolver`):**
- I created the `BasicODESolver` class inside the `numcomp` namespace (`basic_ode.hpp` / `basic_ode.cpp`).
- I created it to organize basic first-order Ordinary Differential Equation (ODE) solving algorithms into a clean, reusable object that separates step-by-step ODE integration from the rest of the mathematical modules.
- The class is designed as a stateless solver (it has no persistent class-level data variables). Instead, the data—the differential equation `f(t, y)` (`std::function<double(double, double)>`), initial time `t0`, initial value `y0`, end time `t_end`, and step size `h`—are passed as parameters into the function, while the computed trajectory points `(t, y)` are stored locally in a `std::vector<std::pair<double, double>>`.
- **4. What functions operate on that data?** The member function `euler(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)` operates on the inputs by stepping from `t0` to `t_end` in increments of `h`, updating `y` using Euler's method, and returning the vector of `(t, y)` pairs.
- The `euler` function is `public` so external files can call it directly, and no `private` data members are needed because keeping the class stateless prevents values from a previous calculation from corrupting future ODE runs. The rest of the program (`src/main.cpp` and `tests/test_basic_ode.cpp`) includes `numerical-computing/basic_ode.hpp`, creates an object `numcomp::BasicODESolver solver;`, calls `solver.euler(...)`, and loops through the returned `(t, y)` vector to display or test the results.

**Member 7 (Muwanguzi Samuel):** 
- I implemented the advanced ODE solvers feature, renamed the files, and debugged the module.
- **Class Structure & Implementation (`AdvancedODESolver`):**
- I created the `AdvancedODESolver` class inside the `numcomp` namespace (`advanced_ode.hpp` / `advanced_ode.cpp`).
  - **2. Why?** I created it to encapsulate higher-order, multi-stage Runge-Kutta ODE methods into a dedicated class that provides high-precision ODE solving while keeping a uniform structure with the rest of the library.
- Like `BasicODESolver`, `AdvancedODESolver` has no persistent class-level data members. It takes the derivative function `f(t, y)` (`std::function<double(double, double)>`), initial time `t0`, initial value `y0`, final time `t_end`, and step size `h` as function inputs, and uses local variables to hold the intermediate Runge-Kutta slopes ($k_1, k_2, k_3, k_4$) and a `std::vector<std::pair<double, double>>` to hold the resulting `(t, y)` solution curve.
- Two member functions operate on the data: `rk4(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)` (4th-Order Runge-Kutta) and `rk3(std::function<double(double, double)> f, double t0, double y0, double t_end, double h)` (3rd-Order Runge-Kutta), which calculate weighted intermediate slopes at each time step and return the full vector of `(t, y)` pairs.
- Both `rk4` and `rk3` are `public` so the user interface and test runner can call them directly, while omitting `private` state variables ensures the solver is self-contained and can solve multiple ODEs back-to-back without state conflicts. The rest of the program (`src/main.cpp` and `tests/test_advanced_ode.cpp`) includes `numerical-computing/advanced_ode.hpp`, instantiates `numcomp::AdvancedODESolver solver;`, calls `solver.rk4(...)` or `solver.rk3(...)`, and reads the returned vector of `(t, y)` coordinate pairs.

**Member 8 (Kyeyune Praise Pauline):** 
- I built automatic testers for the modules that were implemented last week
- I debugged the testers I built and checked if the current build modules were working

**Member 9 (Nabakooza Bridget Maria):** 

## NB
We decided to properly arrange the repo and we agreed to make some changes to the names given to the files. It helps us all debug easily and the project be more organized. I (Jairus) encouraged everyone to rename their files. There were multiple conflicts in multiple files so we came up with this revolution. 

## NEW NAMES
- root_finding changed to basic_root_finder
- adv_roots changed to advanced_root_finder
- interpolation changed to interpolator
- basic_integration changed to basic_integrator
- adv_integration changed to advanced_integrator

## In Progress
**Member 1 (Chemisto Jairus - Lead):**  
- Me and Bridget are still writing the main.cpp file 

**Member 2 (Batte Derrick Calvin):** 
- I am writing the code for matrix solvers. I am also checking my code for logic errors.

**Member 3 (Aogon Sharon):** 
- Writing more examples for basic integration module.

**Member 4 (Bakalu Valeria):** 
- Testing if my module works properly and making final touches on it. Getting ready to write examples.

**Member 5 (Wasswa George Mpanga):** 
- I am testing the code to see if it works and checking for errors.
- I am writing examples for the advanced integrator

**Member 6 (Mwanje Ian):** 
- I'm writing tests for my basic ode module and looking for errors

**Member 7 (Muwanguzi Samuel):** 
- I am checking my code for logic errors and building tests for it  

**Member 8 (Kyeyune Praise Pauline):** 
- I am building testers for Samuel and Ian's modules while checking if there are errors.

**Member 9 (Nabakooza Bridget Maria):** 


## Challenges/Blockers
**Member 8 (Kyeyune Praise Pauline):**  I found errors in some of the members code on running the testers. I am urging them to fix it
They were giving wrong answers
**Member 9 (Nabakooza Bridget Maria):** I suffered with the logic of the main source file. It is very buggy.


## Next Week
**Member 1 (Chemisto Jairus - Lead):** 
- I have to build the test for basic root  finding module as well as assist Calvin on his heavy load next week so Bridget will handle main.cpp construction next week.

**Member 2 (Batte Derrick Calvin):** 
- Implementing matrix solvers and cubic spline interpolation modules

**Member 3 (Aogon Sharon):** 
- I am debugging my branch and organizing the code and finalizing the module.

**Member 4 (Bakalu Valeria):** 

**Member 5 (Wasswa George Mpanga):** 
- Documenting my module and writing final examples

**Member 6 (Mwanje Ian):**
- Building tests for basic ode solver

**Member 7 (Muwanguzi Samuel):** 

**Member 8 (Kyeyune Praise Pauline):** 

**Member 9 (Nabakooza Bridget Maria):**  
-Jairus has to build the tests for the basic root finder module so i will take over constructing the main source file next week and ask for help where i need it.

## AI Use
**Member 3 (Aogon Sharon):**
- Tool: Claude
- Purpose: Debugging
- Reason: Due to time constraints and failure to debug efficiently

**Member 8 (Kyeyune Praise Pauline):** 
- Tool: Gemini
- Purpose: Teaching me how to build the auto tester
- Purpose:  I didn't know how to build an auto tester

**Member 2 (Batte Derrick Calvin):**
- Tool: Claude
- Purpose: Learning cubic spline and logic of matrix solvers
- Reason: I didn't know about these methods so i had to research on them first 

**Member 9 (Nabakooza Bridget Maria):**
- Tool: Claude
- Purpose: To help understand logic of main source file
- Reason: I couldnt easily understand the logic so i consulted the AI

**Member 7 (Muwanguzi Samuel):** 
- Tool: Claude
- Purpose: Debugging
- Reason: I had failed to debugg it effectively