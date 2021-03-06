//
// Copyright 2010, Darren Lafreniere
// <http://www.lafarren.com/image-completer/>
//
// This file is part of lafarren.com's Image Completer.
//
// Image Completer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Image Completer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Image Completer, named License.txt. If not, see
// <http://www.gnu.org/licenses/>.
//

#include "Pch.h"

#ifdef USE_ITK
#include "AppITKMask.h"

#include "tech/MathUtils.h"
#include "tech/DbgMem.h"

#include "itkImageFileReader.h"
#include "itkImageRegionConstIterator.h"

AppITKMask::AppITKMask() : m_offsetX(0), m_offsetY(0)
{
}

bool AppITKMask::LoadAndValidate(const std::string& imagePath, int offsetX, int offsetY)
{
	typedef itk::ImageFileReader<MaskImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(imagePath);
	reader->Update();

#if 0
	if(reader->GetOutput()->GetNumberOfComponentsPerPixel() != static_cast<unsigned char>(LfnIc::Image::Pixel::NUM_CHANNELS))
	{
		std::cerr << "Input image has " << reader->GetOutput()->GetNumberOfComponentsPerPixel() << " components but we are expecting "
		<< LfnIc::Image::Pixel::NUM_CHANNELS << "!" << std::endl;
		exit(-1);
	}
#endif

	m_mask = MaskImageType::New();
	m_mask->Graft(reader->GetOutput());

	m_offsetX = offsetX;
	m_offsetY = offsetY;

	return true;
}

LfnIc::Mask::Value AppITKMask::GetValue(int x, int y) const
{
	const int xMaskSpace = x - m_offsetX;
	const int yMaskSpace = y - m_offsetY;

	const int width = m_mask->GetLargestPossibleRegion().GetSize()[0];
	const int height = m_mask->GetLargestPossibleRegion().GetSize()[1];

	Value value = Mask::KNOWN;
	if (xMaskSpace >= 0 && yMaskSpace >= 0 && xMaskSpace < width && yMaskSpace < height)
	{
		itk::Index<2> index;
		index[0] = x;
		index[1] = y;
		value = ByteToMaskValue(m_mask->GetPixel(index));
	}

	return value;
}

LfnIc::Mask::Value AppITKMask::ByteToMaskValue(unsigned char byte) const
{
	// Declare the ideal values of the different types of mask pixels.
	const int byteUnknown = 0;
	const int byteIgnored = 128;
	const int byteKnown   = 255;

	// Determine which one of the ideal values the input is closest to.
	const int byteIgnoredDiff = abs(byte - byteIgnored);
	if ((byte - byteUnknown) <= byteIgnoredDiff)
	{
		return UNKNOWN;
	}
	else
	{
		if (byteIgnoredDiff <= (byteKnown - byte))
		{
			return IGNORED;
		}
		else
		{
			return KNOWN;
		}
	}
}

#endif // USE_ITK
