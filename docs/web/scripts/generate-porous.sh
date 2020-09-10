#! /usr/bin/env bash

URL_BASE=http://tfel.sourceforge.net
AUTHOR="Thomas Helfer, Jérémy Hure, Mohamed Shokeir"
DATE="20/07/2020"
CRITERION=MichelAndSuquet1992HollowSphereTest
UPPERCASE_CRITERION=MICHEL_SUQUET_1992_HOLLOW_SPHERE_TEST

# clean-up
rm -fr src include tests

# creating the directories
mkdir -p include/TFEL/Material/
mkdir -p mfront/include/MFront/BehaviourBrick
mkdir -p mfront/src
mkdir -p tests/test1
mkdir -p tests/test2

# downloading the template files
wget $URL_BASE/cxx/include/TFEL/Material/PorousStressCriterionTemplate.hxx \
     -P include/TFEL/Material/
wget $URL_BASE/cxx/include/TFEL/Material/PorousStressCriterionTemplate.ixx \
     -P include/TFEL/Material/
wget $URL_BASE/mfront/PorousStressCriterionTest.mfront                     \
     -P tests/test1
wget $URL_BASE/mfront/PorousStressCriterionTest_NumericalJacobian.mfront   \
     -P tests/test1
wget $URL_BASE/cxx/mfront/include/MFront/BehaviourBrick/PorousStressCriterionTemplate.hxx \
     -P mfront/include/MFront/BehaviourBrick/
wget $URL_BASE/cxx/mfront/src/PorousStressCriterionTemplate.cxx \
     -P mfront/src/

# renaming files
mv tests/test1/PorousStressCriterionTest.mfront             \
   tests/test1/${CRITERION}ViscoPlasticity.mfront
mv tests/test1/PorousStressCriterionTest_NumericalJacobian.mfront \
   tests/test1/${CRITERION}ViscoPlasticity_NumericalJacobian.mfront
mv include/TFEL/Material/PorousStressCriterionTemplate.hxx \
   include/TFEL/Material/${CRITERION}StressCriterion.hxx
mv include/TFEL/Material/PorousStressCriterionTemplate.ixx \
   include/TFEL/Material/${CRITERION}StressCriterion.ixx
mv mfront/include/MFront/BehaviourBrick/PorousStressCriterionTemplate.hxx     \
   mfront/include/MFront/BehaviourBrick/${CRITERION}StressCriterion.hxx
mv mfront/src/PorousStressCriterionTemplate.cxx     \
   mfront/src/${CRITERION}StressCriterion.cxx

# replace template values
for f in $(find . -type f);                          \
do sed -i                                            \
  -e "s|__Author__|$AUTHOR|g;"    \
  -e "s|__Date__|$DATE|g;"                      \
  -e "s|__StressCriterionName__|$CRITERION|g;"        \
  -e "s|__STRESS_CRITERION_NAME__|$UPPERCASE_CRITERION|g" $f ; \
done
