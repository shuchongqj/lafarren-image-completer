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
#include "AppData.h"

#include "CommandLineOptions.h"
#include "SettingsText.h"

#include "tech/DbgMem.h"


AppData::AppData(const CommandLineOptions& options, Image& inputImage, Mask& mask, Image& outputImage)
	: m_inputImage(inputImage)
	, m_mask(mask)
	, m_outputImage(outputImage)
	, m_isValid(false)
{
	LfnIc::SettingsConstruct(m_settings, m_inputImage);

	ApplyCommandLineOptionsToSettings(options);
	SettingsText::PrintInvalidMembers settingsTextPrintInvalidMembers(options);

	if (LfnIc::AreSettingsValid(m_settings, &settingsTextPrintInvalidMembers))
	{
		m_isValid = true;

		if (options.ShouldShowSettings())
		{
			// Nothing more to construct for simply displaying the settings.
		}

		if (options.ShouldRunImageCompletion())
		{
			m_inputImage.SetFilePath(options.GetInputImagePath());
			m_outputImage.SetFilePath(options.GetOutputImagePath());

#if ENABLE_PATCHES_INPUT_OUTPUT
			if (options.HasInputPatchesPath())
			{
				m_patchesIstream.reset(new std::ifstream(options.GetInputPatchesPath().c_str(), std::ios::binary));
			}

			if (options.HasOutputPatchesPath())
			{
				m_patchesOstream.reset(new std::ofstream());
				m_patchesOstream->open(options.GetOutputPatchesPath().c_str(), std::ios::binary);
			}
#endif // ENABLE_PATCHES_INPUT_OUTPUT

		}
	}
}

bool AppData::IsValid() const
{
	return m_isValid;
}

const LfnIc::Settings& AppData::GetSettings()
{
	return m_settings;
}

const AppData::Image& AppData::GetInputImage()
{
	return m_inputImage;
}

const AppData::Mask& AppData::GetMask()
{
	return m_mask;
}

AppData::Image& AppData::GetOutputImage()
{
	return m_outputImage;
}

const AppData::Image& AppData::GetOutputImage() const
{
	return m_outputImage;
}

std::istream* AppData::GetPatchesIstream()
{
	return m_patchesIstream.get();
}

std::ostream* AppData::GetPatchesOstream()
{
	return m_patchesOstream.get();
}

void AppData::ApplyCommandLineOptionsToSettings(const CommandLineOptions& options)
{
	// If the command line overrides either (or both) of the lattice gap
	// parameters, then reconstruct the settings based on the new gap size.
	if (options.HasLatticeGapX() || options.HasLatticeGapY())
	{
		SettingsConstruct(
			m_settings,
			options.HasLatticeGapX() ? options.GetLatticeGapX() : m_settings.latticeGapX,
			options.HasLatticeGapY() ? options.GetLatticeGapY() : m_settings.latticeGapY);
	}

	if (options.DebugLowResolutionPasses())
	{
		m_settings.debugLowResolutionPasses = true;
	}
	if (options.HasLowResolutionPassesMax())
	{
		m_settings.lowResolutionPassesMax = options.GetLowResolutionPassesMax();
	}
	if (options.HasNumIterations())
	{
		m_settings.numIterations = options.GetNumIterations();
	}
	if (options.HasPostPruneLabelsMin())
	{
		m_settings.postPruneLabelsMin = options.GetPostPruneLabelsMin();
	}
	if (options.HasPostPruneLabelsMax())
	{
		m_settings.postPruneLabelsMax = options.GetPostPruneLabelsMax();
	}
	if (options.HasCompositorPatchType())
	{
		m_settings.compositorPatchType = options.GetCompositorPatchType();
	}
	if (options.HasCompositorPatchBlender())
	{
		m_settings.compositorPatchBlender = options.GetCompositorPatchBlender();
	}
}
