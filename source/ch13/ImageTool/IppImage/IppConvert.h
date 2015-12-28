/******************************************************************************
 *
 * IppConvert.h
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#pragma once

#include "IppImage.h"
#include "IppDib.h"

void IppDibToImage(IppDib& dib, IppByteImage& img);
void IppDibToImage(IppDib& dib, IppRgbImage& img);

void IppImageToDib(IppByteImage& img, IppDib& dib);
void IppImageToDib(IppFloatImage& img, IppDib& dib);
void IppImageToDib(IppRgbImage& img, IppDib& dib);
