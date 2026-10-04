# Week 1 Progress Report

## Completed
- Created the shared GitHub repository and set up the main folder structure (include, src, tests, examples, reports).
- Wrote the initial CMakeLists.txt file so the project can be built.
- Divided the 18 numerical computing algorithms and testing work among all 9 group members. Everyone started their initial prep:
  - **Member 1 (Chemisto Jairus - Lead):** created the repo, pushed the empty folders with dummy files so git would track them, and wrote the initial C++ code for Bisection and Newton-Raphson.
  - **Member 2 (Batte Derrick Calvin):** successfully cloned the repo locally and researched the formulas for linear interpolation and chebyshev nodes from the lecture slides.
  - **Member 3 (Aogon Sharon):** set up git on their laptop and started mapping out the logic for the basic integration methods (trapezoidal and midpoint rule).
  - **Member 4 (Bakalu Valeria):** researched the secant method and fixed-point iteration to see how they need to handle the math differently than newton-raphson.
  - **Member 5 (Wasswa George Mpanga):** looked into simpson's rule and romberg integration to figure out the grid spacing requirements.
  - **Member 6 (Mwanje Ian):** watched some tutorials on how to structure the loops for the basic differential equation solvers (Euler and finite difference).
  - **Member 7 (Muwanguzi Samuel)):** started breaking down the math for the multi-step ODEs (runge-kutta-fehlberg and the adams methods).
  - **Member 8 (Kyeyune Praise Pauline):** looked into how we are going to write the unit tests and researched the conjugate gradient math.
  - **Member 9 (Nabakooza Bridget Maria):** started drafting the outline for the final README and sketched out what the examples folder should look like to show off the library.

## In Progress
- Getting everyone to test the CMake build on their own machines to make sure it compiles.
- Members 2, 3, 4, and 5 are currently writing the actual C++ code for their assigned math functions.

## Challenges/Blockers
- We had a really annoying issue where Git was ignoring our empty folders. Member 1 had to figure out how to force git to upload the folder structure before anyone else could clone it.
- A few people in the group had errors trying to run CMake in VS Code because their MinGW compiler paths weren't set up right in their environment variables. We spent a lot of time just fixing C++ environments.

## Next Week
- Have Members 2, 3, 4, and 5 put their finished code into separate branches.
- Open our first pull requests and review each other's code before merging it into main.
- Start working on the differential equations code.

## AI Use
- Tool: ChatGPT
- Purpose: Troubleshooting CMake and compiler path errors, and fixing git folder uploads.
- Reason: When some members tried to build the starter code in VS Code, they got compiler path errors. We used AI to figure out how to correctly set the MinGW-w64 path so CMake could find it. We also asked it why our folders weren't showing up on GitHub and learned about git tracking rules.