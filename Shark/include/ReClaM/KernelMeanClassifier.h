//===========================================================================
/*!
 *  \file KernelMeanClassifier.h
 *
 *  \brief Kernel Mean Classifier
 *
 *  \author  T. Glasmachers
 *  \date    2006
 *
 *  \par Copyright (c) 1999-2006:
 *      Institut f&uuml;r Neuroinformatik<BR>
 *      Ruhr-Universit&auml;t Bochum<BR>
 *      D-44780 Bochum, Germany<BR>
 *      Phone: +49-234-32-25558<BR>
 *      Fax:   +49-234-32-14209<BR>
 *      eMail: Shark-admin@neuroinformatik.ruhr-uni-bochum.de<BR>
 *      www:   http://www.neuroinformatik.ruhr-uni-bochum.de<BR>
 *
 *  \par Project:
 *      ReClaM
 *
 *
 *  <BR>
 *
 *
 *  <BR><HR>
 *  This file is part of ReClaM. This library is free software;
 *  you can redistribute it and/or modify it under the terms of the
 *  GNU General Public License as published by the Free Software
 *  Foundation; either version 2, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//===========================================================================


#ifndef _KernelMeanClassifier_H_
#define _KernelMeanClassifier_H_


#include <ReClaM/Model.h>
#include <ReClaM/KernelFunction.h>


//! The kernel mean classifier is parameter free,
//! that is, it does not require training.
class KernelMeanClassifier : public Model
{
public:
	//! Constructor
	//!
	//! \param  k       kernel function
	KernelMeanClassifier(KernelFunction* k);

	//! Constructor
	//!
	//! \param  input   input patterns
	//! \param  target  input labels
	//! \param  k       kernel function
	KernelMeanClassifier(const Array<double>& input, const Array<double>& target, KernelFunction* k);

	//! Destructor
	~KernelMeanClassifier();


	//! Define a set of labeled points as a base for classification
	void SetPoints(const Array<double>& input, const Array<double>& target);

	//! Change a kernel parameter and recalc the coefficients
	void setParameter(unsigned int index, double value);

	//! The model method does the classification.
	void model(const Array<double>& input, Array<double>& output);

protected:
	void Recalc();

	Array<double> training_input;
	Array<double> training_target;
	KernelFunction* kernel;
	bool bMustRecalc;
	double coeff_plus;
	double coeff_minus;
	double bias;
};


#endif

