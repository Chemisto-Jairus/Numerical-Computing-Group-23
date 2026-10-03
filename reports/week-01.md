Completed
•	Created the shared GitHub repository and set up the main folder structure (include, src, tests, examples, reports, data).
•	Wrote the initial CMakeLists.txt file so the project can be built.
•	Divided the 18 numerical computing algorithms among the 7 group members so everyone has clear tasks.
•	Created the basic header files for the Root Finding module to establish our coding style.

In Progress
•	The group leader is writing the actual C++ implementation for the Bisection and Newton-Raphson methods.
•	The rest of the group is researching the math formulas for their assigned modules (like Interpolation and Integration) before writing the code.

Challenges/Blockers
•	We had some trouble getting the CMake build system to work properly on everyone's different laptops. A few members had issues linking their MinGW GCC compilers in VS Code, so we had to spend time troubleshooting our environment paths.

Next Week
•	Finish and push the C++ code for the Root Finding and Interpolation modules.
•	Make sure everyone successfully clones the repo and makes at least one visible commit to prove their environment is working.

AI Use
•	Tool: ChatGPT
•	Purpose: Troubleshooting CMake and compiler path errors.
•	Reason: When some members tried to build the starter code in VS Code, they got compiler path errors. We used AI to figure out how to correctly set the MinGW-w64 path in the environment variables so CMake could find it.
