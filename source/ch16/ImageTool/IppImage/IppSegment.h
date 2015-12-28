/******************************************************************************
*
* IppSegment.h
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
******************************************************************************/

#pragma once

#include <vector>
#include "IppImage.h"
#include "IppFeature.h"

class IppLabelInfo
{
public:
	std::vector<IppPoint> pixels;
	int cx, cy;
	int minx, miny, maxx, maxy;

public:
	IppLabelInfo() : cx(0), cy(0), minx(9999), miny(9999), maxx(0), maxy(0)
	{
		pixels.clear();
	}
};

void IppBinarization(IppByteImage& imgSrc, IppByteImage& imgDst, int threshold);
int  IppBinarizationIterative(IppByteImage& imgSrc);

int  IppLabeling(IppByteImage& imgSrc, IppIntImage& imgDst, std::vector<IppLabelInfo>& labels);

void IppContourTracing(IppByteImage& imgSrc, int sx, int sy, std::vector<IppPoint>& cp);

void IppMorphologyErosion(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppMorphologyDilation(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppMorphologyOpening(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppMorphologyClosing(IppByteImage& imgSrc, IppByteImage& imgDst);

void IppMorphologyGrayErosion(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppMorphologyGrayDilation(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppMorphologyGrayOpening(IppByteImage& imgSrc, IppByteImage& imgDst);
void IppMorphologyGrayClosing(IppByteImage& imgSrc, IppByteImage& imgDst);

void IppFourierDescriptor(IppByteImage& img, int sx, int sy, int percent, std::vector<IppPoint>& cp);

void IppInvariantMoments(IppByteImage& img, double m[7]);
double IppGeometricMoment(IppByteImage& img, int p, int q);

bool IppZernikeMoments(IppByteImage& img, int n, int m, double& zr, double& zi);

IppPoint IppTemplateMatching(IppByteImage& imgSrc, IppByteImage& imgTmpl, IppIntImage& imgMap);
