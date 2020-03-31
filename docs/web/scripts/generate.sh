#! /usr/bin/env bash

URL_BASE=http://tfel.sourceforge.net
AUTHOR="Thomas Helfer, Jérémy Hure, Mohamed Shokeir"
DATE="24/03/2020"
CRITERION=Green1972
UPPERCASE_CRITERION=GREEN_1972

# clean-up
rm -fr src include tests

# creating the directories
mkdir -p include/TFEL/Material/
mkdir -p mfront/include/MFront/BehaviourBrick
mkdir -p mfront/src
mkdir -p tests/test1
mkdir -p tests/test2

# downloading the template files
wget $URL_BASE/cxx/include/TFEL/Material/StressCriterionTemplate.hxx \
     -P include/TFEL/Material/
wget $URL_BASE/cxx/include/TFEL/Material/StressCriterionTemplate.ixx \
     -P include/TFEL/Material/
wget $URL_BASE/mfront/StressCriterionTest.mfront                     \
     -P tests/test1
wget $URL_BASE/mfront/StressCriterionTest_NumericalJacobian.mfront   \
     -P tests/test1
wget $URL_BASE/cxx/mfront/include/MFront/BehaviourBrick/StressCriterionTemplate.hxx \
     -P mfront/include/MFront/BehaviourBrick/
wget $URL_BASE/cxx/mfront/src/StressCriterionTemplate.cxx \
     -P mfront/src/

# renaming files
mv tests/test1/StressCriterionTest.mfront             \
   tests/test1/${CRITERION}PerfectPlasticity.mfront
mv tests/test1/StressCriterionTest_NumericalJacobian.mfront \
   tests/test1/${CRITERION}PerfectPlasticity_NumericalJacobian.mfront
mv include/TFEL/Material/StressCriterionTemplate.hxx \
   include/TFEL/Material/${CRITERION}StressCriterion.hxx
mv include/TFEL/Material/StressCriterionTemplate.ixx \
   include/TFEL/Material/${CRITERION}StressCriterion.ixx
mv mfront/include/MFront/BehaviourBrick/StressCriterionTemplate.hxx     \
   mfront/include/MFront/BehaviourBrick/${CRITERION}StressCriterion.hxx
mv mfront/src/StressCriterionTemplate.cxx     \
   mfront/src/${CRITERION}StressCriterion.cxx

# replace template values
for f in $(find . -type f);                          \
do sed -i                                            \
  -e "s|__Author__|$AUTHOR|g;"    \
  -e "s|__Date__|$DATE|g;"                      \
  -e "s|__StressCriterionName__|$CRITERION|g;"        \
  -e "s|__STRESS_CRITERION_NAME__|$UPPERCASE_CRITERION|g" $f ; \
done
