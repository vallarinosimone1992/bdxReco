/*
 * CalorimeterFitHelper.cc
 *
 *  Created on: Feb 18, 2016
 *      Author: celentan
 */

#include <JANA/JStreamLog.h>
#include "CalorimeterFitHelper.h"

CalorimeterFitHelper::CalorimeterFitHelper():
y(0),x(0),m_f(0),m_minimizer(0),N(0),m_verbosity(0)
{
	// TODO Auto-generated constructor stub

	STEP=0.01;

}

CalorimeterFitHelper::~CalorimeterFitHelper() {
	// TODO Auto-generated destructor stub
	if (m_minimizer) delete m_minimizer;
}

double CalorimeterFitHelper::DoEval(const double *x) const{
	double ret=0;
	m_f->SetParameters(x);

	for (int ipoint=0;ipoint<N;ipoint++){
		ret+=(this->y[ipoint]-m_f->Eval(this->x[ipoint]))*(this->y[ipoint]-m_f->Eval(this->x[ipoint]));
	//	jout<<ipoint<<" :: "<<this->x[ipoint]<<" "<<this->y[ipoint]<<" "<<m_f->Eval(this->x[ipoint])<<std::endl;
	}

	return ret;

}

unsigned int CalorimeterFitHelper::NDim() const{
	return m_f->GetNpar();
}

ROOT::Math::IBaseFunctionMultiDim* CalorimeterFitHelper::Clone() const{
	return 0;
}

void CalorimeterFitHelper::configMinimizer(){
	//if (!m_minimizer) m_minimizer=new TMinuitMinimizer();
	if (!m_minimizer) m_minimizer= new ROOT::Minuit2::Minuit2Minimizer();
//			ROOT::Math::Factory::CreateMinimizer("Minuit2","Migrad"); //minimizer name: Minuit, Minuit2 //algo: Migrad, Simplex, Combined, Scan.

	m_minimizer->SetFunction(*this); /*This is very important*/
	for (int ipar=0;ipar<NDim();ipar++){
		m_minimizer->SetVariable(ipar,m_f->GetParName(ipar),m_f->GetParameter(ipar),STEP);
	}

	m_minimizer->SetPrintLevel(m_verbosity);
	//m_minimizer->SetMaxFunctionCalls(1000000);
	//m_minimizer->SetMaxIterations(100000);
	//m_minimizer->SetTolerance(0.01);//The minimization will stop when the estimated distance to the minimum is less than 0.001*tolerance (from ROOT FORUM)
	m_minimizer->SetPrecision(0); //find it automatically
}

void CalorimeterFitHelper::doFit(){
	this->configMinimizer();
	//jout<<"cazzo "<<N<<std::endl;
	//for (int ii=0;ii<N;ii++) jout<<x[ii]<<" "<<y[ii]<<std::endl;
	m_minimizer->SetStrategy(2);
	m_minimizer->Minimize();
	//jout<<"Done"<<std::endl;
	m_f->SetParameters(m_minimizer->X());
}


