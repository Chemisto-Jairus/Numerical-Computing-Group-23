#pragma once
/**
 * @file numcomp.hpp
 * @brief Umbrella header: includes every public module of the library.
 * @author Chemisto Jairus (Member 1, Repo Lead)
 *
 * A program that uses several modules can include this one file instead
 * of ten separate headers:
 * @code
 * #include "numerical-computing/numcomp.hpp"
 *
 * numcomp::BasicRootFinder finder;
 * double root = finder.bisection([](double x) { return x * x - 2.0; }, 0.0, 2.0, 1e-10, 100);
 * @endcode
 */

#include "numerical-computing/basic_root_finder.hpp"
#include "numerical-computing/advanced_root_finder.hpp"
#include "numerical-computing/interpolator.hpp"
#include "numerical-computing/cubic_spline.hpp"
#include "numerical-computing/basic_integrator.hpp"
#include "numerical-computing/advanced_integrator.hpp"
#include "numerical-computing/basic_ode_solver.hpp"
#include "numerical-computing/advanced_ode_solver.hpp"
#include "numerical-computing/matrix_solver.hpp"
#include "numerical-computing/numerical_differentiator.hpp"

/**
 * @namespace numcomp
 * @brief Everything in the Group 23 numerical computing library.
 *
 * The library is split into stateless classes covering root finding,
 * interpolation, numerical differentiation, integration, ODE solving and
 * linear systems.
 * Every class has a default constructor and only public member functions.
 * Invalid input is reported with standard exceptions:
 * - std::invalid_argument: a parameter is outside its allowed range.
 * - std::out_of_range: an interpolation point lies outside the data.
 * - std::runtime_error: the method itself fails (no convergence, singular or
 *   non-positive-definite matrix, a result that is not a finite number).
 */
