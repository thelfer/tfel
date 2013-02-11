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
      using std::vector;
      typedef FactorizedKriging<1u,1u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<1u,double> > > MyFactorizedKriging;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n0 = KrigingUtilities::normalize(vx);
      this->a0 = n0.first;
      this->b0 = n0.second;
      pair<double,double> n1 = KrigingUtilities::normalize(vy);
      this->a1 = n1.first;
      this->b1 = n1.second;
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	MyFactorizedKriging::addValue(this->a0*(*px)+this->b0,
				    this->a1*(*py)+this->b1,*pz);
      }
      MyFactorizedKriging::buildInterpolation();
    }

    FactorizedKriging1D1D::FactorizedKriging1D1D(const tfel::math::vector<double>& vx,
						 const tfel::math::vector<double>& vy,
						 const tfel::math::vector<double>& vz)
    {
      using namespace std;
      using namespace tfel::math;
      using tfel::math::vector;
      typedef FactorizedKriging<1u,1u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<1u,double> > > MyFactorizedKriging;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n0 = KrigingUtilities::normalize(vx);
      this->a0 = n0.first;
      this->b0 = n0.second;
      pair<double,double> n1 = KrigingUtilities::normalize(vy);
      this->a1 = n1.first;
      this->b1 = n1.second;
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	MyFactorizedKriging::addValue(this->a0*(*px)+this->b0,
				    this->a1*(*py)+this->b1,*pz);
      }
      MyFactorizedKriging::buildInterpolation();
    }
  
    double
    FactorizedKriging1D1D::operator()(const double vx,
				      const double vy) const
    {
      using namespace tfel::math;
      typedef FactorizedKriging<1u,1u,double,
	KrigingPieceWiseLinearModel1D<double>,
	KrigingModelAdaptator<KrigingDefaultModel<1u,double> > > MyFactorizedKriging;
      return MyFactorizedKriging::operator()(this->a0*(vx)+this->b0,
					   this->a1*(vy)+this->b1);
    } // end of FactorizedKriging1D1D::operator()
      
    FactorizedKriging1D1D::~FactorizedKriging1D1D()
    {} // end of FactorizedKriging1D1D::~FactorizedKriging1D1D

  } // end of namespace math

} // end of namespace tfel

