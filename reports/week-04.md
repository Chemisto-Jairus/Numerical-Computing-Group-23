# Week 4 Progress Report

## NB

- Bridget reminded us about some modules we had completely forgotten to implement. We are going to work together in the remaining period to implement them in the remaining time.
- We are now restructing the entire repository and we divided the missing modules amongst ourselves and this time we are to all work together and help each other on all of them so as to meet the deadline
- All files that need updating in the repository are going to be updated.
- Our apologies if things seem messy at the moment (TRUST THE PROCESS)

 The missing modules are the following (to be implemented next week by all of us together)
 
- Numerical Differentiation (to be worked upon mainly by Jairus(Leader)) 
- Finite Difference Method (to be worked upon mainly by Sharon(Member 3)) 
- Runge-Kutta-Fehlberg (RKF45) (to be worked upon mainly by George(Member 5)) 
- Conjugate Gradient (to be worked upon mainly by Valeria(Member 4)) 
- Adams-Bashforth (AB2, AB4) (to be worked upon mainly by Ian(Memer 6))
- Adams-Moulton (AM2, AM4) (to be worked upon mainly by Samuel(Member 7)) 
- Chebyshev Approximation updates (some missing methods) (to be worked upon mainly by Calvin(member 2)) 

- We agreed to implement helper functions to prevent code repeatition so all the next code will be shorter and more efficient

## Completed

**Member 1 (Chemisto Jairus - Lead):** 

- I added professional comments to basic root finder module to make it easy to understand.
- I added catch and throw functions to the basic root finder code.
- I added the internal helper functions for input checks to avoid repetition and urged the other members to implement them in their modules.
- I added the examples and tests for the basic root finder module,

**Member 2 (Batte Derrick Calvin):** 

- Fixed logic for interpolator module.
- Added Doxygen style comments to header files.
- I implemented cubic spline module.
- I wrote the test for Cubic spline.
- I wrote  the matrix solver module.
- I added the helper functions to all my modules.
- I wrote examples for the cubic spline and matrix solver modules.
- I documented my modules for the previous weeks

**Member 3 (Aogon Sharon):** 

- I added professional comments to basic integrator
- Debugged basic integrator module. (no warnings were being given if a or b were infinity)
- I added the helper functions to my basic integrator
- I documented the basic Integrator module.

**Member 4 (Bakalu Valeria):** 

- I added Doxygen comments to my header file.
- I fixed the logic of the advanced root finder. NaN is now rejected (the check is now: not (tol > 0), which also catches NaN)
- I added more tests for the advanced root finder.
- I implemented helpers in my module
- I added examples for the module
- I documented the advanced root finder module.

**Member 5 (Wasswa George Mpanga):** 

- I added comments to the header file and then fixed the advanced integrator logic.
- I updated tests for advanced integrator.
- I completed the examples for the advanced integrator and used the helper functions

**Member 6 (Mwanje Ian):** 

- I added comments to the basic ode header file
- I debugged the logic of the ode solver
- I added helpers to my module and implemented its examples

**Member 7 (Muwanguzi Samuel):** 

- I added Doxygen comments to my header file
- I debugged advanced ode solver
- I built examples and tests for advanced ode solver
- I implemented the helpers for the examples
- I documented advanced ode solver


**Member 8 (Kyeyune Praise Pauline):** 

- I updated the auto tester to include more modules soon to be done by other members
- I fixed its logic so that it also shows if the tests failed
- I updated the auto tester
- I finalized the auto tester for the current modules
- I helped Calvin create the test for his matrix solver module.

**Member 9 (Nabakooza Bridget Maria):** 

- I updated the main source file. It currently has errors because Calvin has not yet pushed the missing modules. (Calvin finished pushing the modules so now no errors)
I updated the main source file again.
- I included the helpers in the main source file.

## NB

- I (Sharon) noticed our code is very repetitive. I have urged Jairus the leader and other members that we have to fix this by week's end.
- Noting Sharon's observation we are going to implement helper functions to avoid uneccesary repetition. (Calvin)

## In Progress

**Member 2 (Batte Derrick Calvin):** 

- I am writing examples for cubic spline interpolation module and interpolator. (done)
- I am writing the matrix solver module. (DONE)
- I am writing tests for the matrix solver module as well as examples. (done)
- I am documenting my modules.

**Member 3 (Aogon Sharon):** 

- Writing examples files for the advanced integration module.
- Documenting the module (done)

**Member 8 (Kyeyune Praise Pauline):** 

- Checking if the auto tester works efficiently

**Member 1 (Chemisto Jairus - Lead):** 

- I am trying to help Calvin with his heavy load this week and also write my tests and examples for basic root finder module.
- I am checking to see if Bridget is building the main source file well.

**Member 4 (Bakalu Valeria):** 

- I am writing examples for my module and finalizing documenting it. (done)
- I am seeing if i can improve efficiency of the code
- I am researching the missing modules and seeing which one i will contribute to next week.

**Member 8 (Kyeyune Praise Pauline):** 

- I am helping Calvin write the tester for the matrix solver module he is soon uploading (finished)


**Member 6 (Mwanje Ian):**

- I am building tests and examples for basic ode solver


## Challenges/Blockers

**Member 8 (Kyeyune Praise Pauline):** 

- VS code had issues so i committed the code twice. It's now been fixed.

**ALL**

- Restructure the work division to meet deadline due to miscounting required modules earlier.



## Next Week

 **Member 1 (Chemisto Jairus - Lead):** 

- Numerical Differentiation

**Member 2 (Batte Derrick Calvin):** 

- Chebyshev Approximation updates

**Member 3 (Aogon Sharon):** 

- Finite Difference Method 

**Member 4 (Bakalu Valeria):** 

- Conjugate Gradient

**Member 5 (Wasswa George Mpanga):** 

- Runge-Kutta-Fehlberg

**Member 6 (Mwanje Ian):** 

- Adams-Bashforth

**Member 7 (Muwanguzi Samuel):** 

- Adams-Moulton (AM2, AM4)

**Member 8 (Kyeyune Praise Pauline):** 

- Updating the auto tester and seeing whethr all the modules are functioning well

**Member 9 (Nabakooza Bridget Maria):** 

 - Finalizing the UI for the main source file and including the remaining modules in the main.

## AI Use

**Member 2 (Batte Derrick Calvin):**
- Tool: Claude
- Purpose: Presentation of profession code and Debugging
- Reason: Claude reccomended i add the comments so that it has professional aesthetic and helped me debug my module which Pauline reported to fail some tests.

**Member 8 (Kyeyune Praise Pauline):** 
- Tool: Gemini
- Purpose: To get the auto tester working well.
- Reason: I didn't know the syntax for writing the auto tester.

**Member 1 (Chemisto Jairus - Lead):** 
- Tool: Claude
- Purpose: Debugging
- Reason: I couldn't figure out why my code was failing some work so i used claude to help debug it.

**Member 5 (Wasswa George Mpanga):** 
- Tool: Claude
- Purpose: Debugging
- Reason: I  added comments to the header file and then fixed the advanced integrator logic to ensure that it accounted for scenarios it had missed before.

**Member 4 (Bakalu Valeria):** 

- Tool: Chatgpt
- Purpose: Help with Debugging
- Reason: I needed to ensure my code worked perfectly so i asked chatgpt to review it and it gave me patches which i have implemented.

**Member 7 (Muwanguzi Samuel):**
- Tool: Claude
- Purpose: Helped identify and fix bugs in my advanced ODE solver implementation
- Reason: To help identify why the solver was failing, and to understand the cause of the errors

**Member 6 (Mwanje Ian):**
- Tool: Claude
- Purpose: Debugging
- Reason: I  added comments to the header file and then fixed the advanced integrator logic to ensure that it accounted for scenarios it had missed before.

**Member 9 (Nabakooza Bridget Maria):** 
- Tool: Chatpgt
- Purpose: Debugging the main source file
- Reason: It is a lot of code so i got lost and i needed its help.