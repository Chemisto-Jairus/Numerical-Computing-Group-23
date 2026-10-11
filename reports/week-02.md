# Week 2 Progress Report

## Current Status & Objectives
We focused on getting Interpolation, Basic Integration, and Advanced Root Finding working this week. Most of the code was finished early, but we had to spend a couple of days standardizing the architecture after realizing we weren't fully meeting the Object-Oriented Programming requirement.

## Completed
Everyone pushed their assigned modules. To fix our architecture problem, we agreed on a shared design pattern for the library. Because these are purely mathematical operations that only need runtime inputs, none of our classes hold private data. All member functions are public. This lets the main program just set up a class and immediately call its methods without dealing with getters or setters.

-   **Member 1 (Jairus):** I created a ruleset on the repo which prevents anyone, including myself from pushing to the main branch without using a pull request and it being permitted by other members. This ensured every change to the main branch had to be done through creation of a new branch I went back and refactored my Week 1 code into a `BasicRootFinder` class using another branch and pushed the classes implentation to the main branch. We needed this to group the foundational algorithms in one place. It handles the `bisection` and `newton_raphson` functions. I later deleted the repos i used to make changes to the main.
-   **Member 2 (Derrick):** I created the `Interpolator` class to manage all our data point estimation. It runs the `linear_interpolate` and `chebyshev_nodes` methods on whatever vectors are passed to it. This was done after one of us suggested we use classes so i adjusted my code from the previous commits to have classes. I later created the pull request and waited for Jairus the leader to accept it.
-   **Member 3 (Sharon):** Built `BasicIntegrator`. The purpose of this class is to handle standard area calculations, specifically `trapezoidal_rule` and `midpoint_rule`. Created a pull request once I  was done
-   **Member 4 (Valeria):** I wrote the `AdvancedRootFinder` class. This separates the complex root-finding methods (which don't need strict mathematical derivatives) from my basic ones. It includes `secant_method` and `fixed_point_iteration`. I recreated my branch after my mistake of creating a branch while i was checked out in Shon's branch and i deleted the branch i made from Shin's branch.
-   **Member 5 (George):** I added `AdvancedIntegrator` for highly accurate curve calculations, wrapping up the `simpsons_rule` and `romberg_integration` functions. I suffered with creating a branch but was successful after. I finalized my part by creating the pull request.

## Challenges Encountered & Solutions
-   **The OOP pivot:** Early in the week, we were mostly writing standalone C++ functions. When someone pushed their part using classes, we realized we were completely ignoring the OOP requirement. I had everyone pause, and we went back to refactor all the raw code into classes (even my Week 1 code on `main`). It was extra work, but the library makes a lot more sense now.
-   **Git branching mistakes:** Valeria accidentally started her feature branch while checked out on Sharon's branch instead of `main`. We caught it before it tangled the commit history. The fix was just deleting that local branch, running `git checkout main`, pulling the base code, and starting over. Good reminder for us to run `git status`.
-   **More members joining:** As more members had joined the group we had to restructure the work division and this prompted us to edit the original README file on how we divided the work in that particular section thus the commits from Derrick and George Wasswa to update theirs. All the other members had not yet created their branches so they did not do this.


## AI Use
To fix the classes issue quickly, we asked ChatGPT how we can convert our existing functional C++ code into a class-based structure. After getting the correct syntax for splitting class declarations into headers and implementations into source files, we then wrote our individual parts. This let us fix the architecture across all the branches without having to rewrite any of the actual math logic.

Valeria asked Gemini how to undo and delete her mistake cloning Shon's branch and make a new one.

Jairus used Gemini to know how to create the ruleset on the repo to prevent people from accidentally pushing to the main before we confirm their code.


## Next Week
-   **Merge and Resolve (Jairus):** I need to review all these Week 2 Pull Requests, check the `CMakeLists.txt` for conflicts, and get everything merged into `main`.
-   **Begin ODEs (Ian & Samuel):** They are going to start work on the Ordinary Differential Equation solvers (Euler and Advanced methods).
-   **Setup Testing (Pauline):** She is going to begin setting up the unit testing framework so we can verify all the math modules we just merged actually work.