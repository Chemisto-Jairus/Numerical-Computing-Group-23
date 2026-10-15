#!/bin/bash

echo "==================================================="
echo "       GROUP 23: AUTOMATED TEST RUNNER             "
echo "==================================================="
echo ""

echo ">>> 1. Running Basic Integration Tests (Sharon)..."
./test_basic_integration
echo ""

echo ">>> 2. Running Advanced Integration Tests (George)..."
./test_advanced_integration
echo ""

echo ">>> 3. Running Interpolation Tests (Derrick)..."
./test_basic_interpolation
echo ""

echo ">>> 4. Running Advanced Root Finder Tests (Valeria)..."
./test_advanced_root_finder
echo ""

echo "==================================================="
echo "  ALL WEEK 3 TESTS EXECUTED SUCCESSFULLY!          "
echo "==================================================="