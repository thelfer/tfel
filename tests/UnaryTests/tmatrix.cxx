#include<iostream>
#include<cstdlib>
#include<cmath>

#include "Math/qt.hxx"
#include "Math/functions.hxx"
#include "Math/tmatrix.hxx"

#include "function_unary_tests.hxx"

const double prec = 1.e-12;

int main(void){
  
  using namespace tfel::math;

  typedef tmatrix<2,2,double> matrix1;
  typedef tmatrix<2,2,unsigned short> matrix2;
  USING_TFEL_FUNCTIONS;

  matrix1 v1;
  matrix2 v2;

  v2(0,0)=2;
  v2(0,1)=1;
  v2(1,0)=5;
  v2(1,1)=3;

  v1(0,0)=4.;
  v1(0,1)=1.;
  v1(1,0)=25.;
  v1(1,1)=9.;

  function(v1               , 4.,1., 25., 9.,prec,1);
  function(2*v1             , 8.,2., 50.,18.,prec,2);
  function(v1*2             , 8.,2., 50.,18.,prec,3);
  function(v2               , 2 ,1 ,  5 , 3 ,0   ,4);
  function(v1+v2            , 6.,2., 30.,12.,prec,5);
  function(v1-v2            , 2.,0., 20., 6.,prec,6);
  function(v1+v2+v1         ,10.,3., 55.,21.,prec,7);
  function(v1+v2-v1         , 2.,1.,  5., 3.,prec,8);
  function(2.*v1+v2         ,10.,3., 55.,21.,prec,9);
  function(v1+(2.*(v1+v2))  ,16.,5., 85.,33.,prec,10);
  function(3*v1+(2.*(v1+v2)),24.,7.,135.,51.,prec,11);

  v1(0,0)=0.1;
  v1(0,1)=1.234;
  v1(1,0)=2.123;
  v1(1,1)=4.529;

  v2(0,0)=0;
  v2(0,1)=0;
  v2(1,0)=3;
  v2(1,1)=1;

  function(sin(v1+v2),std::sin(0.1),std::sin(1.234),std::sin(5.123),std::sin(5.529),prec,12);
  function(cos(v1+v2),std::cos(0.1),std::cos(1.234),std::cos(5.123),std::cos(5.529),prec,13);
  function(tan(v1+v2),std::tan(0.1),std::tan(1.234),std::tan(5.123),std::tan(5.529),prec,14);
  function(sinh(v1+v2),std::sinh(0.1),std::sinh(1.234),std::sinh(5.123),std::sinh(5.529),prec,15);
  function(cosh(v1+v2),std::cosh(0.1),std::cosh(1.234),std::cosh(5.123),std::cosh(5.529),prec,16);
  function(tanh(v1+v2),std::tanh(0.1),std::tanh(1.234),std::tanh(5.123),std::tanh(5.529),prec,17);
  function(exp(v1+v2),std::exp(0.1),std::exp(1.234),std::exp(5.123),std::exp(5.529),prec,18);
  function(log(v1+v2),std::log(0.1),std::log(1.234),std::log(5.123),std::log(5.529),prec,19);
  function(log10(v1+v2),std::log10(0.1),std::log10(1.234),std::log10(5.123),std::log10(5.529),prec,20);
  function(sqrt(v1+v2),std::sqrt(0.1),std::sqrt(1.234),std::sqrt(5.123),std::sqrt(5.529),prec,21);

  v1(0,0)=0.;
  v1(0,1)=0.6785;
  v1(1,0)=0.3234;
  v1(1,1)=0.2976;

  function(asin(v1),std::asin(0.),std::asin(0.6785),std::asin(0.3234),std::asin(0.2976),prec,22);
  function(acos(v1),std::acos(0.),std::acos(0.6785),std::acos(0.3234),std::acos(0.2976),prec,23);
  function(atan(v1),std::atan(0.),std::atan(0.6785),std::atan(0.3234),std::atan(0.2976),prec,24);

  std::cout << "success" << std::endl;

  return EXIT_SUCCESS;

}
