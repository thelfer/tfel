/*! 
 * \file  Kriging3D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 */

#include"TFEL/Math/Kriging3D.hxx"

namespace tfel
{

  namespace math
  {

    Kriging3D::Kriging3D(const std::vector<double>& vx,
			 const std::vector<double>& vy,
			 const std::vector<double>& vz,
			 const std::vector<double>& vv)
    {
      using namespace std;
      using tfel::math::tvector;
      Kriging<3u,double>::buildInterpolation();
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      tvector<3u,double> v;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	v(0)=*px;
	v(1)=*py;
	v(2)=*pz;
	Kriging<3u,double>::addValue(v,*pv);
      }
      Kriging<3u,double>::buildInterpolation();
    }

    Kriging3D::Kriging3D(const tfel::math::vector<double>& vx,
			 const tfel::math::vector<double>& vy,
			 const tfel::math::vector<double>& vz,
			 const tfel::math::vector<double>& vv)
    {
      using namespace tfel::math;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      tvector<3u,double> v;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	v(0)=*px;
	v(1)=*py;
	v(2)=*pz;
	Kriging<3u,double>::addValue(v,*pv);
      }
      Kriging<3u,double>::buildInterpolation();
    }
  
    double
    Kriging3D::operator()(const double vx,
			  const double vy,
			  const double vz) const
    {
      using namespace tfel::math;
      tvector<3u,double> v;
      v(0)=vx;
      v(1)=vy;
      v(2)=vz;
      return Kriging<3u,double>::operator()(v);
    } // end of Kriging3D::operator()
      
    Kriging3D::~Kriging3D()
    {} // end of Kriging3D::~Kriging3D

  } // end of namespace math

} // end of namespace tfel
