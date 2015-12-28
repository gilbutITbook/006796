/******************************************************************************
*
* IppColor.h
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
******************************************************************************/

#pragma once

#include "IppImage.h"

void IppInverse(IppRgbImage& img);

void RGB_TO_HSI(double R, double G, double B, double& H, double& S, double& I);
void HSI_TO_RGB(double H, double S, double I, double& R, double& G, double& B);

void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V);
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B);

void IppColorSplitRGB(IppRgbImage& imgColor, IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB);
void IppColorSplitHSI(IppRgbImage& imgColor, IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI);
void IppColorSplitYUV(IppRgbImage& imgColor, IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV);

bool IppColorCombineRGB(IppByteImage& imgR, IppByteImage& imgG, IppByteImage& imgB, IppRgbImage& imgColor);
bool IppColorCombineHSI(IppByteImage& imgH, IppByteImage& imgS, IppByteImage& imgI, IppRgbImage& imgColor);
bool IppColorCombineYUV(IppByteImage& imgY, IppByteImage& imgU, IppByteImage& imgV, IppRgbImage& imgColor);

void IppColorEdge(IppRgbImage& imgSrc, IppByteImage& imgEdge);

void IppColorHistoEqual(IppRgbImage& imgSrc, IppRgbImage& imgDst);
