/*! 
 * \file  Kriging2D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 */

#include"TFEL/Math/Kriging2D.hxx"

namespace tfel
{

  namespace math
  {

    Kriging2D::Kriging2D(const std::vector<double>& vx,
			 const std::vector<double>& vy,
			 const std::vector<double>& vz)
    {
      using namespace std;
      using tfel::math::tvector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      tvector<2u,double> v;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	v(0)=*px;
	v(1)=*py;
	Kriging<2u,double>::addValue(v,*pz);
      }
      Kriging<2u,double>::buildInterpolation();
    }

    Kriging2D::Kriging2D(const tfel::math::vector<double>& vx,
			 const tfel::math::vector<double>& vy,
			 const tfel::math::vector<double>& vz)
    {
      using namespace tfel::math;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      tvector<2u,double> v;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	v(0)=*px;
	v(1)=*py;
	Kriging<2u,double>::addValue(v,*pz);
      }
      Kriging<2u,double>::buildInterpolation();
    }
  
    double
    Kriging2D::operator()(const double vx,
			  const double vy) const
    {
      using namespace tfel::math;
      tvector<2u,double> v;
      v(0)=vx;
      v(1)=vy;
      return Kriging<2u,double>::operator()(v);
    } // end of Kriging2D::operator()
      
    Kriging2D::~Kriging2D()
    {} // end of Kriging2D::~Kriging2D

  } // end of namespace math

} // end of namespace tfel
