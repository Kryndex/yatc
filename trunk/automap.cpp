//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include "automap.h"
#include "sprite.h"
#include "engine.h"
#include "util.h"
#include "gamecontent/globalvars.h"

MiniMapArea::MiniMapArea(int x, int y, int z)
{
	m_basepos.x = x & 0xFF00;
	m_basepos.y = y & 0xFF00;
	m_basepos.z = z & 0xFF;
	oRGBA color; //default color
	m_bitmap = g_engine->createSprite(256, 256, color);
	load();
}

MiniMapArea::~MiniMapArea()
{
	save();
	delete m_bitmap;
}


bool MiniMapArea::save()
{
	std::stringstream x, y, z, minimapfnss;
	//get the name of the file
	x << setw(3) << setfill('0') << m_basepos.x / 256;
    y << setw(3) << setfill('0') << m_basepos.y / 256;
    z << setw(2) << setfill('0') << m_basepos.z;
    minimapfnss << x.str() << y.str() << z.str() << ".map";
    FILE* f = yatc_fopen(minimapfnss.str().c_str(), "wb");
    if(!f){
    	return false;
    }
    yatc_fwrite(m_color, 1, 256*256, f);
    yatc_fwrite(m_speed, 1, 256*256, f);

    //save map marks if any
    for(std::list<MapMark*>::iterator it = m_marks.begin(); it != m_marks.end(); ++it){
    	yatc_fwrite(&((*it)->x), 1, 4, f);
    	yatc_fwrite(&((*it)->y), 1, 4, f);
    	yatc_fwrite(&((*it)->type), 1, 4, f);
		uint16_t length = (*it)->text.size();
    	yatc_fwrite(&length, 1, 2, f);
    	yatc_fwrite(const_cast<char*>((*it)->text.c_str()), 1, length, f);
    }


    fclose(f);
    return true;
}

bool MiniMapArea::load()
{
	//get the name of the file
	std::stringstream x, y, z, minimapfnss;
	x << setw(3) << setfill('0') << m_basepos.x / 256;
    y << setw(3) << setfill('0') << m_basepos.y / 256;
    z << setw(2) << setfill('0') << m_basepos.z;
    minimapfnss << x.str() << y.str() << z.str() << ".map";
    FILE* f = yatc_fopen(minimapfnss.str().c_str(), "rb");
    if(!f){
    	memset(m_color, 0, 256*256);
    	memset(m_speed, 255, 256*256);
    }
    else{

		yatc_fread(m_color, 1, 256*256, f);
		yatc_fread(m_speed, 1, 256*256, f);

		if(!feof(f)){ //there are map marks
			int32_t marksCount = 0;
			yatc_fread(&marksCount, 4, 1, f);
			if(marksCount > 100) marksCount = 100;
			for(int i = 0; i < marksCount; ++i){
				uint32_t x, y, type;
				uint16_t length;
				char *text;

				yatc_fread(&x, 4, 1, f);
				yatc_fread(&y, 4, 1, f);
				yatc_fread(&type, 4, 1, f);
				yatc_fread(&length, 2, 1, f);
				text = new char[length + 1];
				yatc_fread(text, length, 1, f);
				text[length] = 0;

				MapMark* mark = new MapMark(x, y, type, text);
				m_marks.push_back(mark);

				delete[] text;
			}
		}
		fclose(f);
    }

    //Create the sprite
	SDL_Surface* s = m_bitmap->lockSurface();
	for(int i = 0; i < 256; i++){
		for(int j = 0; j < 256; j++){
			uint8_t r, g, b;
			getRGB(m_color[i][j], r, g, b);
			m_bitmap->putPixel(i, j, SDL_MapRGB(s->format, r, g, b) ,s);
		}
	}
	m_bitmap->unlockSurface();

    return true;
}

void MiniMapArea::setTileColor(uint16_t x, uint16_t y, uint8_t color, uint8_t speedindex)
{
	//update arrays
	x &= 0xFF; y &= 0xFF;
	m_color[x][y] = color;
	m_speed[x][y] = speedindex;
	//update the srpite
	SDL_Surface* s = m_bitmap->lockSurface();
	uint8_t r, g, b;
	getRGB(color, r, g, b);
	m_bitmap->putPixel(x, y, SDL_MapRGB(s->format, r, g, b) ,s);
	m_bitmap->unlockSurface();
}

void MiniMapArea::getRGB(uint8_t color, uint8_t& r, uint8_t& g, uint8_t& b)
{
	b = uint8_t((color % 6) / 5. * 255);
	g = uint8_t(((color / 6) % 6) / 5. * 255);
	r = uint8_t((color / 36.) / 6. * 255);
}

///////////////////////////////////////////////

Automap::Automap()
{
	//
}

Automap::~Automap()
{
	for(std::map<uint32_t, MiniMapArea*>::iterator it = m_areas.begin(); it != m_areas.end(); ++it){
		delete it->second;
	}
}

MiniMapArea* Automap::getArea(int x, int y, int z)
{
	uint32_t posindex = (z & 0xFF) | ((y & 0xFF00) << 8) | ((x & 0xFF00) << 16);
	std::map<uint32_t, MiniMapArea*>::iterator it = m_areas.find(posindex);
	if(it != m_areas.end()){
		return it->second;
	}
	else{
		MiniMapArea* area = new MiniMapArea(x, y, z);
		uint32_t posindex = (z & 0xFF) | ((y & 0xFF00) << 8) | ((x & 0xFF00) << 16);
		m_areas[posindex] = area;
		return area;
	}
}

void Automap::setTileColor(int x, int y, int z, uint8_t color, uint8_t speedindex)
{
	MiniMapArea* area = getArea(x, y, z);
	if(area){
		area->setTileColor(x & 0xFF, y & 0xFF, color, speedindex);
	}
}

void Automap::renderSelf(int x, int y, int w, int h, const Position& centerPos, int zoom)
// parameters specify where on the screen it should be painted
{
	//FIXME. zoom is not working because Blit doesnt scale properly the image

	//draw the minimap
	int x1 = centerPos.x - (w/2)/zoom;
	int y1 = centerPos.y - (h/2)/zoom;

	//background
	g_engine->drawRectangle(x, y, w, h, oRGBA(0,0,0,1));

	int i, j;
	//Decide which areas and where should be drawn
	if(x1 < 0) i = -x1*zoom;
	else i = -(x1 & 0xFF)*zoom;
	for(;i < w; i += 256*zoom){
		int current_x = x1 + i/zoom;
		int destx = i, srcx = 0;
		int srcw = 256, destw = 256*zoom;
		if(destx < 0){
			srcx = -destx/zoom;
			srcw -= srcx;
			destx = 0;
			destw = srcw*zoom;
		}
		if(destx + destw > w){
			destw = w - destx;
			srcw = destw/zoom;
		}

		if(y1 < 0) j = -y1*zoom;
		else j = -(y1 & 0xFF)*zoom;
		for(;j < h; j += 256*zoom){
			int current_y = y1 + j/zoom;
			int desty = j, srcy = 0;
			int srch = 256, desth = 256*zoom;

			if(desty < 0){
				srcy = -desty/zoom;
				srch -= srcy;
				desty = 0;
				desth = srch*zoom;
			}
			if(desty + desth > h){
				desth = h - desty;
				srch = desth/zoom;
			}

			MiniMapArea* area = getArea(current_x, current_y, centerPos.z);
			if(area){
				//area->getSprite()->Blit(x + destx, y + desty, srcx, srcy, srcw, srch);
				area->getSprite()->Blit(x + destx, y + desty, srcx, srcy, srcw, srch, destw, desth); //<-- Does not work!
			}
		}
	}

    //mark where is the player
    g_engine->drawRectangle(x + w/2 -1, y + h/2-1, 3, 3, oRGBA(1,1,1,1));
}