---
title: TFELTests - Unit Testing Framework
author: Thomas Helfer
date: 10/04/2010
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Overview

`TFELTests` is a lightweight and flexible unit testing framework
designed for C++ applications. It provides a comprehensive set of tools
for writing, organizing, and executing unit tests, with support for test
suites, test proxies, and multiple output formats including standard
streams and XML (JUnit format).

The framework is particularly well-suited for scientific computing
applications and is integrated with the TFEL ecosystem.

# Key Features

## Test Organization

The framework supports several levels of test organization:

- **Test Case**: A single test that can contain multiple assertions
- **Test Suite**: A collection of tests that can be executed together
- **Test Manager**: A singleton that manages multiple test suites and outputs

## Assertion Macros

The framework provides a rich set of assertion macros for testing various conditions:

- `TFEL_TESTS_ASSERT(X)`: Assert that a condition is true
- `TFEL_TESTS_STATIC_ASSERT(X)`: Static assertion that is evaluated at compile time
- `TFEL_TESTS_CHECK_THROW(X, Y)`: Check that code X throws exception of type Y
- `TFEL_TESTS_CALLMETHOD(X)`: Call a method and register its result
- `TFEL_TESTS_CHECK_EQUAL(X, Y)`: Check that X equals Y
- `TFEL_TESTS_CHECK_GT(X, Y)`: Check that X is greater than Y
- `TFEL_TESTS_CHECK_GE(X, Y)`: Check that X is greater than or equal to Y
- `TFEL_TESTS_CHECK_LT(X, Y)`: Check that X is less than Y
- `TFEL_TESTS_CHECK_LE(X, Y)`: Check that X is less than or equal to Y

## Output Formats

Test results can be output to:

- Standard streams (console, files)
- XML files in JUnit format for integration with continuous integration
  systems
- Multiple outputs simultaneously

## Automatic Test Registration

The framework provides mechanisms for automatic test registration using
proxy classes, reducing boilerplate code.

# Usage

## Basic Test Case

The simplest way to create a test is to inherit from the `TestCase` class:

~~~~{.cpp}
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"

struct MyTest final : public tfel::tests::TestCase {
  MyTest() : TestCase("MyGroup", "MyTest") {}
  tfel::tests::TestResult execute() override {
    TFEL_TESTS_ASSERT(true);
    TFEL_TESTS_ASSERT(1 != 2);
    TFEL_TESTS_CHECK_EQUAL(std::string("test"), "test");
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(MyTest, "MyTestSuite");
~~~~

## Test Function Wrapper

For simple test functions, the `TestFunctionWrapper` can be used:

~~~~{.cpp}
#include "TFEL/Tests/TestFunctionWrapper.hxx"

bool myTestFunction() {
  return true;
}

int main() {
  using namespace tfel::tests;
  using Wrapper = TestFunctionWrapper<myTestFunction>;
  auto test = std::make_shared<Wrapper>("MyGroup", "myTestFunction");
  TestSuite suite("MySuite");
  suite.add(test);
  return suite.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
~~~~

## Test Suite

Tests can be grouped into suites for better organization:

~~~~{.cpp}
#include "TFEL/Tests/TestSuite.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"

bool test1() { return true; }
bool test2() { return false; }

int main() {
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<test1>;
  using Wrapper2 = TestFunctionWrapper<test2>;
  
  auto a = std::make_shared<Wrapper1>("test1");
  auto b = std::make_shared<Wrapper2>("test2");
  
  TestSuite suite("MySuite");
  suite.add(a);
  suite.add(b);
  
  return suite.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
~~~~

## Test Manager

The `TestManager` singleton provides centralized management of tests and outputs:

~~~~{.cpp}
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"

bool test1() { return true; }

int main() {
  using namespace tfel::tests;
  using Wrapper = TestFunctionWrapper<test1>;
  
  auto& m = TestManager::getTestManager();
  auto test = std::make_shared<Wrapper>("test1");
  
  m.addTest("MySuite", test);
  m.addTestOutput("test-output.txt");
  m.addXMLTestOutput("test-results.xml");
  
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
~~~~

## Output Configuration

The framework supports various output configurations:

### Standard Stream Output

~~~~{.cpp}
#include "TFEL/Tests/StdStreamTestOutput.hxx"

// Redirect output to a file
auto output = std::make_shared<tfel::tests::StdStreamTestOutput>("output.txt");

// Redirect output to std::cout with colors
auto console = std::make_shared<tfel::tests::StdStreamTestOutput>(std::cout, true);
~~~~

### XML Output (JUnit Format)

~~~~{.cpp}
#include "TFEL/Tests/XMLTestOutput.hxx"

auto xml_output = std::make_shared<tfel::tests::XMLTestOutput>("results.xml");
~~~~

### Multiple Outputs

~~~~{.cpp}
#include "TFEL/Tests/MultipleTestOutputs.hxx"

tfel::tests::MultipleTestOutputs outputs;
outputs.addTestOutput(std::make_shared<tfel::tests::StdStreamTestOutput>("file1.txt"));
outputs.addTestOutput(std::make_shared<tfel::tests::XMLTestOutput>("results.xml"));
~~~~

# API Reference

## Core Classes

### Test

Base class for all unit tests. Provides the interface that all tests must implement.

- `virtual std::string name() const = 0`: Return the name of the test
- `virtual std::string classname() const = 0`: Return the group/class of the test
- `virtual TestResult execute() = 0`: Execute the test and return the result

### TestCase

A concrete test class that can contain multiple assertions.

- `TestCase(const std::string&)`: Constructor with test name
- `TestCase(const std::string&, const std::string&)`: Constructor with group and test name
- `virtual void registerResult(const std::string_view, const bool, const std::string_view = {})`: Register a test result
- `TestResult result`: The result of the test

### TestSuite

A collection of tests that can be executed together.

- `TestSuite()`: Default constructor
- `TestSuite(const std::string&)`: Constructor with suite name
- `void add(const TestPtr&)`: Add a test to the suite
- `TestResult execute(TestOutput&)`: Execute the suite with a specific output
- `TestResult execute()`: Execute the suite with default output

### TestManager

Singleton class for managing tests and outputs.

- `static TestManager& getTestManager()`: Get the singleton instance
- `void addTestOutput(TestOutputPtr)`: Add a test output
- `void addTestOutput(std::ostream&, const bool = true)`: Add a stream output
- `void addTestOutput(const std::string&)`: Add a file output
- `void addXMLTestOutput(const std::string&)`: Add an XML output
- `void addTestOutput(const std::string&, TestOutputPtr)`: Add output for a specific suite
- `void addTest(const std::string&, TestPtr)`: Add a test to a suite
- `TestResult execute()`: Execute all registered tests

### TestFunctionWrapper

Template class for wrapping simple test functions.

- `TestFunctionWrapper(const std::string&, const std::string&)`: Constructor with group and function name
- `TestFunctionWrapper(const char* const)`: Constructor with function name

### TestProxy

Template class for automatic test registration.

- `template <typename... Arguments> TestProxy(const std::string&, Arguments&&...)`: Constructor

## Output Classes

### TestOutput

Base class for test outputs.

- `virtual void beginTestSuite(const std::string&) = 0`: Called when a test suite begins
- `virtual void addTest(const std::string&, const std::string&, const TestResult&) = 0`: Add a test result
- `virtual void endTestSuite(const TestResult&) = 0`: Called when a test suite ends

### StdStreamTestOutput

Output to standard streams.

- `StdStreamTestOutput(const std::string&)`: Constructor with file name
- `StdStreamTestOutput(std::ostream&, const bool = true)`: Constructor with stream and color option
- `StdStreamTestOutput(const std::shared_ptr<std::ostream>&, const bool = false)`: Constructor with shared stream

### XMLTestOutput

Output to XML file in JUnit format.

- `XMLTestOutput(const std::string&)`: Constructor with file name

### MultipleTestOutputs

Aggregate multiple outputs.

- `void addTestOutput(const TestOutputPtr&)`: Add an output

## Result Classes

### TestResult

Structure describing the result of a test or test suite.

- `TestResult()`: Default constructor
- `TestResult(const bool)`: Constructor with success status
- `TestResult(const bool, const std::string_view, const std::string_view = {})`: Constructor with details
- `bool success() const`: Check if the test was successful
- `const std::string& getDescription() const`: Get the test description
- `const std::string& getFailureDescription() const`: Get the failure description
- `void append(const TestResult&)`: Append a result
- `void setTestDuration(const double)`: Set the test duration
- `double duration() const`: Get the test duration

# Macros

## Test Registration

- `TFEL_TESTS_GENERATE_PROXY(X, Y)`: Generate a proxy for class X in suite Y
- `TFEL_TESTS_GENERATE_PROXY2(X, X2, Y)`: Generate a proxy with custom name
- `TFEL_TESTS_GENERATE_PROXY3(X, X2, Y, Y2)`: Generate a proxy with constructor argument
- `TFEL_TESTS_FUNCTION(X, G, Y)`: Generate a test function with group G in suite Y

## Assertions

- `TFEL_TESTS_ASSERT(X)`: Assert condition X
- `TFEL_TESTS_STATIC_ASSERT(X)`: Static assertion
- `TFEL_TESTS_CHECK_THROW(X, Y)`: Check exception
- `TFEL_TESTS_CALLMETHOD(X)`: Call method and register result
- `TFEL_TESTS_CHECK_EQUAL(X, Y)`: Check equality
- `TFEL_TESTS_CHECK_GT(X, Y)`: Check greater than
- `TFEL_TESTS_CHECK_GE(X, Y)`: Check greater or equal
- `TFEL_TESTS_CHECK_LT(X, Y)`: Check less than
- `TFEL_TESTS_CHECK_LE(X, Y)`: Check less or equal

# Integration with Build Systems

The TFELTests library is typically used in CMake-based projects. Tests can be added to the build system using the standard CMake testing infrastructure or custom test runners.

# Examples

The TFEL source code includes several examples of unit tests in the `tests/Tests/` directory:

- `testcase.cxx`: Example of using the TestCase class
- `testsuite.cxx`: Example of using the TestSuite class
- `testmanager.cxx`: Example of using the TestManager class
- `testproxy.cxx`: Example of using test proxies
- `multipletestoutputs.cxx`: Example of using multiple outputs
- `stdstreamtestoutput.cxx`: Example of using standard stream output

# Best Practices

1. **Test Isolation**: Each test should be independent and not rely on the state of other tests.

2. **Descriptive Names**: Use descriptive names for tests and test suites to make it clear what is being tested.

3. **Single Responsibility**: Each test should verify a single behavior or property.

4. **Use Assertions Wisely**: Use the appropriate assertion macro for the condition being tested.

5. **Test Coverage**: Aim for comprehensive test coverage of your code, including edge cases and error conditions.

6. **Performance**: For performance-critical code, consider the overhead of test execution and use appropriate test strategies.

# Continuous Integration

The XML output format follows the JUnit standard, making it compatible with most continuous integration systems including Jenkins, GitLab CI, and GitHub Actions. This allows for easy integration of test results into your CI pipeline.

<!-- Local IspellDict: english -->
