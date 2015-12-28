/******************************************************************************
*
* IppGeometry.h
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
******************************************************************************/

#pragma once

#include "IppImage.h"

void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy);

void IppResizeNearest(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh);
void IppResizeBilinear(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh);
void IppResizeCubic(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh);
double cubic_interpolation(double v1, double v2, double v3, double v4, double d);

void IppRotate(IppByteImage& imgSrc, IppByteImage& imgDst, double angle);
void IppRotate90(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppRotate180(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppRotate270(IppByteImage& imgSrc, IppByteImage& imgDst);

void IppMirror(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppFlip(IppByteImage& imgSrc, IppByteImage& imgDst);
