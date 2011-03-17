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

#ifndef APP_WX_IMAGE_H
#define APP_WX_IMAGE_H

#ifdef USE_WX
#include "LfnIcImage.h"

//
// Implements LfnIc::Image, using a wxImage to load, store, and save the data.
//
class AppImageWx : public LfnIc::Image
{
public:
	// LfnIc::Image interface
	virtual bool LoadAndValidate(const std::string& imagePath);
	virtual void Save();
	virtual bool Init(int width, int height);
	virtual bool IsValid() const;
	virtual const std::string& GetFilePath() const;
	virtual Pixel* GetData();
	virtual const Pixel* GetData() const;
	virtual int GetWidth() const;
	virtual int GetHeight() const;

private:
	// Internal data
	wxImage m_wxImage;
};

#endif // USE_WX
#endif
