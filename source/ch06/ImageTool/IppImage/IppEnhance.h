/******************************************************************************
 *
 * IppEnhance.h
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#pragma once

#include "IppImage.h"

void IppInverse(IppByteImage& img);
void IppBrightness(IppByteImage& img, int n);
void IppContrast(IppByteImage& img, int n);
void IppGammaCorrection(IppByteImage& img, float gamma);
void IppHistogram(IppByteImage& img, float histo[256]);
void IppHistogramStretching(IppByteImage& img);
void IppHistogramEqualization(IppByteImage& img);
