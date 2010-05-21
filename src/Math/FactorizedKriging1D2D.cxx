/*! 
 * \file  FactorizedKriging1D2D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 mai 2010
 */

#include"TFEL/Math/FactorizedKriging1D2D.hxx"

namespace tfel
{

  namespace math
  {

    FactorizedKriging1D2D::FactorizedKriging1D2D(const std::vector<double>& vx,
						 const std::vector<double>& vy,
						 const std::vector<double>& vz,
						 const std::vector<double>& vv)
    {
      using namespace std;
      using namespace tfel::math;
      typedef FactorizedKriging<1u,2u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<2u,double> > > FactorizedKriging;
      using std::vector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	tvector<2u> v;
	v(0) = *py;
	v(1) = *pz;
	FactorizedKriging::addValue(*px,v,*pv);
      }
      FactorizedKriging::buildInterpolation();
    }

    FactorizedKriging1D2D::FactorizedKriging1D2D(const tfel::math::vector<double>& vx,
						 const tfel::math::vector<double>& vy,
						 const tfel::math::vector<double>& vz,
						 const tfel::math::vector<double>& vv)
    {
      using namespace tfel::math;
      typedef FactorizedKriging<1u,2u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<2u,double> > > FactorizedKriging;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	tvector<2u> v;
	v(0) = *py;
	v(1) = *pz;
	FactorizedKriging::addValue(*px,v,*pv);
      }
      FactorizedKriging::buildInterpolation();
    }
  
    double
    FactorizedKriging1D2D::operator()(const double vx,
				      const double vy,
				      const double vz) const
    {
      using namespace tfel::math;
      typedef FactorizedKriging<1u,2u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<2u,double> > > FactorizedKriging;
      tvector<2u> v;
      v(0) = vy;
      v(1) = vz;
      return FactorizedKriging::operator()(vx,v);
    } // end of FactorizedKriging1D2D::operator()

    FactorizedKriging1D2D::~FactorizedKriging1D2D()
    {} // end of FactorizedKriging1D2D::~FactorizedKriging1D2D

  } // end of namespace math

} // end of namespace tfel

