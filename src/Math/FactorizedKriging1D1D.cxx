/*! 
 * \file  FactorizedKriging1D1D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 mai 2010
 */

#include"TFEL/Math/FactorizedKriging1D1D.hxx"

namespace tfel
{

  namespace math
  {

    FactorizedKriging1D1D::FactorizedKriging1D1D(const std::vector<double>& vx,
						 const std::vector<double>& vy,
						 const std::vector<double>& vz)
    {
      using namespace std;
      using namespace tfel::math;
      typedef FactorizedKriging<1u,1u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<1u,double> > > FactorizedKriging;
      using std::vector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	FactorizedKriging::addValue(*px,*py,*pz);
      }
      FactorizedKriging::buildInterpolation();
    }

    FactorizedKriging1D1D::FactorizedKriging1D1D(const tfel::math::vector<double>& vx,
						 const tfel::math::vector<double>& vy,
						 const tfel::math::vector<double>& vz)
    {
      using namespace tfel::math;
      typedef FactorizedKriging<1u,1u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<1u,double> > > FactorizedKriging;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	FactorizedKriging::addValue(*px,*py,*pz);
      }
      FactorizedKriging::buildInterpolation();
    }
  
    double
    FactorizedKriging1D1D::operator()(const double vx,
				      const double vy) const
    {
      using namespace tfel::math;
      typedef FactorizedKriging<1u,1u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<1u,double> > > FactorizedKriging;
      return FactorizedKriging::operator()(vx,vy);
    } // end of FactorizedKriging1D1D::operator()
      
    FactorizedKriging1D1D::~FactorizedKriging1D1D()
    {} // end of FactorizedKriging1D1D::~FactorizedKriging1D1D

  } // end of namespace math

} // end of namespace tfel

