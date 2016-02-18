/*
 * CalorimeterFitHelper.h
 *
 *  Created on: Feb 18, 2016
 *      Author: celentan
 */

#ifndef SRC_LIBRARIES_CALORIMETER_CALORIMETERFITHELPER_H_
#define SRC_LIBRARIES_CALORIMETER_CALORIMETERFITHELPER_H_

#include "Fit/ParameterSettings.h"
#include "Math/Minimizer.h"
#include "Math/IFunction.h"
#include "Math/Factory.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "TMinuitMinimizer.h"
#include "TF1.h"

class CalorimeterFitHelper : public ROOT::Math::IBaseFunctionMultiDim{
public:
	CalorimeterFitHelper();
	virtual ~CalorimeterFitHelper();

	/*These are inherited from IBaseFunctionMultiDim*/
	virtual double DoEval(const double*) const;
	virtual unsigned int NDim() const;
	virtual ROOT::Math::IBaseFunctionMultiDim* Clone() const;

	ROOT::Minuit2::Minuit2Minimizer* getMinimizer(){return m_minimizer;}
	//TMinuitMinimizer* getMinimizer(){return m_minimizer;}
	void doFit();

	TF1* getF() const {
		return m_f;
	}

	void setF(TF1*& f) {
		m_f = f;
	}


	int getVerbosity() const {
		return m_verbosity;
	}

	void setVerbosity(int verbosity) {
		m_verbosity = verbosity;
	}

	int getN() const {
		return N;
	}

	void setN(int n) {
		N = n;
	}

	double* getX() const {
		return x;
	}

	void setX(double* x) {
		this->x = x;
	}

	double* getY() const {
		return y;
	}

	void setY(double* y) {
		this->y = y;
	}

private:
	double STEP;
	double *x,*y;
	int N;
	int m_verbosity;
	ROOT::Minuit2::Minuit2Minimizer*  m_minimizer;
	//TMinuitMinimizer* m_minimizer;
	TF1 *m_f;

	void configMinimizer();
};

#endif /* SRC_LIBRARIES_CALORIMETER_CALORIMETERFITHELPER_H_ */
