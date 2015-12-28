/******************************************************************************
 *
 * RGBBYTE.cpp
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/
 
#pragma once

typedef unsigned char BYTE;

class RGBBYTE
{
public:
	BYTE b;
	BYTE g;
	BYTE r;

public:
	RGBBYTE() : b(0), g(0), r(0) {}
	RGBBYTE(BYTE gray) : b(gray), g(gray), r(gray) {}
	RGBBYTE(const BYTE& _r, const BYTE& _g, const BYTE& _b) : b(_b), g(_g), r(_r) {}
};

