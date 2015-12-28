/******************************************************************************
*
* IppDibJpg.cpp
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
*****************************************************************************/

#pragma warning(disable:4099)

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <setjmp.h>
#include <algorithm>

#include "IppDib.h"
#include "jpeglib.h"

struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */

	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

BOOL IppDib::LoadJPG(const char* filename)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;

	FILE * infile;      // 입력 파일
	JSAMPARRAY buffer;  // 라인 단위 출력 버퍼
	int row_stride;     // 출력 버퍼 크기

	BOOL ret = FALSE;

	fopen_s(&infile, filename, "rb"); // 입력 파일 열기
	if (!infile)
		return FALSE;

	// Step 1: JPEG 압축 해제를 위한 객체 생성 및 초기화
	cinfo.err = jpeg_std_error(&jerr.pub); // JPEG 라이브러리 에러 처리 설정
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return FALSE;
	}

	jpeg_create_decompress(&cinfo);

	// Step 2: 입력 파일 지정
	jpeg_stdio_src(&cinfo, infile);

	// Step 3: 파일 헤더 읽기
	(void)jpeg_read_header(&cinfo, TRUE);

	// Step 4: decompression 파라미터 설정
	// 기본 값을 그대로 사용할 것이므로 추가 코드 없음.

	// Step 5: JPEG 압축 해제 시작
	(void)jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	// Step 6: 라인 단위로 JPEG 파일의 픽셀 데이터를 읽어와서 IppDib 픽셀 값 설정
	// 그레이스케일 영상과 트루컬러 영상을 구분하여 처리함.

	if (cinfo.out_color_space == JCS_GRAYSCALE)
	{
		// 그레이스케일 영상
		CreateGrayBitmap(cinfo.image_width, cinfo.image_height);
		BYTE* pixels = GetDIBitsAddr();
		int ws = (cinfo.image_width + 3) & ~3;

		while (cinfo.output_scanline < cinfo.output_height) {
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);
			memcpy(&pixels[(cinfo.image_height - cinfo.output_scanline) * ws], buffer[0], row_stride);
		}
	}
	else if (cinfo.out_color_space == JCS_RGB)
	{
		// 트루컬러 영상
		CreateRgbBitmap(cinfo.image_width, cinfo.image_height);
		BYTE* pixels = GetDIBitsAddr();
		int ws = (cinfo.image_width * 3 + 3) & ~3;

		while (cinfo.output_scanline < cinfo.output_height) {
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);

			// RGB -> BGR
			for (int i = 0; i < row_stride; i += 3) {
				std::swap(buffer[0][i], buffer[0][i + 2]);
			}

			memcpy(&pixels[(cinfo.image_height - cinfo.output_scanline) * ws], buffer[0], row_stride);
		}
	}

	// Step 7: JPEG 압축 해제 종료
	(void)jpeg_finish_decompress(&cinfo);

	// Step 8: JPEG 압축 해제를 위한 객체 해제
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	return TRUE;
}

BOOL IppDib::SaveJPG(const char* filename)
{
	if (GetBitCount() == 8 || GetBitCount() == 24)
		return FALSE;

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	FILE * outfile;             // 출력 파일
	JSAMPROW row_pointer[1];    // 라인 단위 버퍼 주소
	int row_stride;             // 버퍼 크기

								// 비트맵 객체가 그레이스케일 또는 트루컬러 영상이 아니면 종료
	if (GetBitCount() == 8 || GetBitCount() == 24)
		return FALSE;

	// Step 1: JPEG 압축을 위한 객체 생성 및 초기화
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	// Step 2: 출력 파일 지정
	fopen_s(&outfile, filename, "wb");
	if (!outfile)
	{
		jpeg_destroy_compress(&cinfo);
		return FALSE;
	}

	jpeg_stdio_dest(&cinfo, outfile);

	// Step 3: 압축을 위한 파라미터 설정
	cinfo.image_width = GetWidth();
	cinfo.image_height = GetHeight();

	if (GetBitCount() == 8) // 그레이스케일 영상
	{
		cinfo.input_components = 1;
		cinfo.in_color_space = JCS_GRAYSCALE;
	}
	else  // 트루컬러 영상
	{
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
	}

	int quality = 95;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	// Step 4: 압축 시작
	jpeg_start_compress(&cinfo, TRUE);

	// Step 5: 라인 단위로 DIB 픽셀 데이터를 읽어와서 JPEG 파일로 저장
	// 그레이스케일 영상과 트루컬러 영상을 구분하여 처리함.

	if (GetBitCount() == 8) // 그레이스케일 영상
	{
		row_stride = cinfo.image_width;
		BYTE* pixels = GetDIBitsAddr();
		int ws = (cinfo.image_width + 3) & ~3;

		while (cinfo.next_scanline < cinfo.image_height) {
			row_pointer[0] = &pixels[(cinfo.image_height - cinfo.next_scanline - 1)*ws];
			(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
	}
	else // 트루컬러 영상
	{
		row_stride = cinfo.image_width * 3;
		BYTE* pixels = GetDIBitsAddr();
		BYTE* buffer = new BYTE[row_stride];
		int ws = (cinfo.image_width * 3 + 3) & ~3;

		while (cinfo.next_scanline < cinfo.image_height) {
			memcpy(buffer, &pixels[(cinfo.image_height - cinfo.next_scanline - 1)*ws], row_stride);

			// BGR -> RGB
			for (int i = 0; i < row_stride; i += 3) {
				std::swap(buffer[i], buffer[i + 2]);
			}

			row_pointer[0] = buffer;
			(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
		delete[] buffer;
	}

	// Step 6: JPEG 압축 종료
	jpeg_finish_compress(&cinfo);
	fclose(outfile);

	// Step 7: JPEG 압축을 위한 객체 해제
	jpeg_destroy_compress(&cinfo);

	return TRUE;
}
