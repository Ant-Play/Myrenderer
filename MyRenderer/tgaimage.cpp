#include <iostream>
#include <cstring>
#include "tgaimage.h"

//TGAImage::TGAImage(const int w, const int h, const int bpp) :
//	w(w),
//	h(h),
//	bpp(bpp),
//	data(w* h* bpp, 0) {}
//
//bool TGAImage::read_tga_file(const std::string filename)
//{
//	std::ifstream in;
//	in.open(filename, std::ios::binary);
//	if (!in.is_open())
//	{
//		std::cerr << "cant open file" << filename << std::endl;
//		in.close();
//		return false;
//	}
//	TGAHeader header;
//	in.read(reinterpret_cast<char*>(&header), sizeof(header));
//	if (!in.good())
//	{
//		in.close();
//		std::cerr << "an error occurred while reading the header" << std::endl;
//		return false;
//	}
//	w = header.width;
//	h = header.height;
//	bpp = header.bitsPerPixel >> 3;
//	if (w <= 0 || h <= 0 || (bpp != GRAYSCALE && bpp != RGB && bpp != RGBA))
//	{
//		in.close();
//		std::cerr << "bad bpp (or width/height) value" << std::endl;
//		return false;
//	}
//	size_t nbytes = bpp * w * h;
//	data = std::vector<std::uint8_t>(nbytes, 0);
//	if (3 == header.dataTypeCode || 2 == header.dataTypeCode)
//	{
//		in.read(reinterpret_cast<char*>(data.data()), nbytes);
//		if (!in.good())
//		{
//			in.close();
//			std::cerr << "an error occurred while reading the data" << std::endl;
//			return false;
//		}
//	}
//	else if (10 == header.dataTypeCode || 11 == header.dataTypeCode)
//	{
//		if (!load_rle_data(in))
//		{
//			in.close();
//			std::cerr << "an error occurred while reading the data" << std::endl;
//			return false;
//		}
//	}
//	else
//	{
//		in.close();
//		std::cerr << "unknown file format" << (int)header.dataTypeCode << std::endl;
//		return false;
//	}
//	if (!(header.imageDescriptor & 0x20))
//		flip_vertically();
//	if (header.imageDescriptor & 0x10)
//		flip_horizontally();
//	std::cerr << w << "x" << h << "/" << bpp * 8 << std::endl;
//	in.close();
//	return true;
//}
//
//bool TGAImage::load_rle_data(std::ifstream& in)
//{
//	size_t pixelCount = w * h;
//	size_t currentPixel = 0;
//	size_t currentByte = 0;
//	TGAColor colorBuffer;
//	do 
//	{
//		std::uint8_t chunkHeader = 0;
//		chunkHeader = in.get();
//		if (!in.good())
//		{
//			std::cerr << "an error occurred while reading the data" << std::endl;
//			return false;
//		}
//		if (chunkHeader < 128)
//		{
//			chunkHeader++;
//			for (int i = 0; i < chunkHeader; i++)
//			{
//				in.read(reinterpret_cast<char*>(colorBuffer.bgra), bpp);
//				if (!in.good())
//				{
//					std::cerr << "an error occurred while reading the header" << std::endl;
//					return false;
//				}
//				for (int t = 0; t < bpp; t++)
//				{
//					data[currentByte++] = colorBuffer.bgra[t];
//				}
//				currentPixel++;
//				if (currentPixel > pixelCount)
//				{
//					std::cerr << "Too many pixels read" << std::endl;
//					return false;
//				}
//			}
//		}
//		else
//		{
//			chunkHeader -= 127;
//			in.read(reinterpret_cast<char*>(colorBuffer.bgra), bpp);
//			if (!in.good())
//			{
//				std::cerr << "an error occurred while reading the header" << std::endl;
//				return false;
//			}
//			for (int i = 0; i < chunkHeader; i++)
//			{
//				for (int t = 0; t < bpp; t++)
//				{
//					data[currentByte++] = colorBuffer.bgra[t];
//				}
//				currentPixel++;
//				if (currentPixel > pixelCount)
//				{
//					std::cerr << "Too many pixels read" << std::endl;
//					return false;
//				}
//			}
//		}
//	} while (currentPixel < pixelCount);
//	return true;
//}
//
//bool TGAImage::write_tga_file(const std::string filename, const bool vflip /* = true */, const bool rle /* = true */) const
//{
//	constexpr std::uint8_t developer_area_ref[4] = { 0,0,0,0 };
//	constexpr std::uint8_t extension_area_ref[4] = { 0,0,0,0 };
//	constexpr std::uint8_t footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','\0' };
//	std::ofstream out;
//	out.open(filename, std::ios::binary);
//	if (!out.is_open())
//	{
//		std::cerr << "can't open file" << filename << std::endl;
//		out.close();
//		return false;
//	}
//	TGAHeader header = {};
//	header.bitsPerPixel = bpp << 3;
//	header.width = w;
//	header.height = h;
//	header.dataTypeCode = (bpp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
//	header.imageDescriptor = vflip ? 0x00 : 0x20; // top-left or bottom-left origin
//	out.write(reinterpret_cast<char*>(&header), sizeof(header));
//	if (!out.good())
//	{
//		out.close();
//		std::cerr << "can't dump the tga file" << std::endl;
//		return false;
//	}
//	if (!rle)
//	{
//		out.write(reinterpret_cast<const char*>(data.data()), w * h * bpp);
//		if (!out.good())
//		{
//			std::cerr << "can't unload raw data" << std::endl;
//			out.close();
//			return false;
//		}
//	}
//	else if (!unload_rle_data(out))
//	{
//		out.close();
//		std::cerr << "can't unload the rle file" << std::endl;
//		return false;
//	}
//	out.write(reinterpret_cast<const char*>(developer_area_ref), sizeof(developer_area_ref));
//	if (!out.good())
//	{
//		out.close();
//		std::cerr << "can't dump tga data" << std::endl;
//		return false;
//	}
//	out.write(reinterpret_cast<const char*>(extension_area_ref), sizeof(extension_area_ref));
//	if (!out.good())
//	{
//		out.close();
//		std::cerr << "can't dump tga data" << std::endl;
//		return false;
//	}
//	out.write(reinterpret_cast<const char*>(footer), sizeof(footer));
//	if (!out.good())
//	{
//		out.close();
//		std::cerr << "can't dump the tga file" << std::endl;
//		return false;
//	}
//	out.close();
//	return true;
//}
//
//// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
//bool TGAImage::unload_rle_data(std::ofstream& out) const
//{
//	const std::uint8_t max_chunk_length = 128;
//	size_t npixels = w * h;
//	size_t curpix = 0;
//	while (curpix < npixels)
//	{
//		size_t chunkstart = curpix * bpp;
//		size_t curbyte = curpix * bpp;
//		std::uint8_t run_length = 1;
//		bool raw = true;
//		while (curpix + run_length < npixels && run_length < max_chunk_length)
//		{
//			bool succ_eq = true;
//			for (int t = 0; succ_eq && t < bpp; t++)
//			{
//				succ_eq = (data[curbyte + t] == data[curbyte + t + bpp]);
//			}
//			curbyte += bpp;
//			if (1 == run_length)
//			{
//				raw = !succ_eq;
//			}
//			if (raw && succ_eq)
//			{
//				run_length--;
//				break;
//			}
//			if (!raw && !succ_eq)
//			{
//				break;
//			}
//			run_length++;
//		}
//		curpix += run_length;
//		out.put(raw ? run_length - 1 : run_length + 127);
//		if (!out.good())
//		{
//			std::cerr << "can't dump the tga file" << std::endl;
//			return false;
//		}
//		out.write(reinterpret_cast<const char*>(data.data() + chunkstart), (raw ? run_length * bpp : bpp));
//		if (!out.good())
//		{
//			std::cerr<< "can't dump the tga file" << std::endl;
//			return false;
//		}
//	}
//	return true;
//}
//
//TGAColor TGAImage::get(const int x, const int y) const
//{
//	if (!data.size() || x < 0 || y < 0 || x >= w || y >= h)
//	{
//		return {};
//	}
//	TGAColor ret = { 0,0,0,0,bpp };
//	const std::uint8_t* p = data.data() + (x + y * w) * bpp;
//	for (int i = bpp; i--; ret.bgra[i] = p[i]);
//	return ret;
//}
//
//void TGAImage::set(const int x, const int y, TGAColor& c)
//{
//	if (!data.size() || x < 0 || y < 0 || x >= w || y >= h) return;
//	memcpy(data.data() + (x + y * w) * bpp, c.bgra, bpp);
//}
//
//void TGAImage::flip_horizontally()
//{
//	int half = w >> 1;
//	for (int i = 0; i < half; i++)
//	{
//		for (int j = 0; j < h; j++)
//		{
//			for (int b = 0; b < bpp; b++)
//			{
//				std::swap(data[(i + j * w) * bpp + b], data[(w - 1 - i + j * w) * bpp + b]);
//			}
//		}
//	}
//}
//
//void TGAImage::flip_vertically()
//{
//	int half = h >> 1;
//	for (int i = 0; i < w; i++)
//	{
//		for (int j = 0; j < half; j++)
//		{
//			for (int b = 0; b < bpp; b++)
//			{
//				std::swap(data[(i + j * w) * bpp + b], data[(i + (h - 1 - j) * w) * bpp + b]);
//			}
//		}
//	}
//}
//
//int TGAImage::width() const
//{
//	return w;
//}
//
//int TGAImage::height() const
//{
//	return h;
//}

TGAImage::TGAImage() : data(NULL), width(0), height(0), bytespp(0) {
}

TGAImage::TGAImage(int w, int h, int bpp) : data(NULL), width(w), height(h), bytespp(bpp) {
	unsigned long nbytes = width * height * bytespp;
	data = new unsigned char[nbytes];
	memset(data, 0, nbytes);
}

TGAImage::TGAImage(const TGAImage& img) {
	width = img.width;
	height = img.height;
	bytespp = img.bytespp;
	unsigned long nbytes = width * height * bytespp;
	data = new unsigned char[nbytes];
	memcpy(data, img.data, nbytes);
}

TGAImage::~TGAImage() {
	if (data) delete[] data;
}

TGAImage& TGAImage::operator =(const TGAImage& img) {
	if (this != &img) {
		if (data) delete[] data;
		width = img.width;
		height = img.height;
		bytespp = img.bytespp;
		unsigned long nbytes = width * height * bytespp;
		data = new unsigned char[nbytes];
		memcpy(data, img.data, nbytes);
	}
	return *this;
}

bool TGAImage::read_tga_file(const char* filename) {
	if (data) delete[] data;
	data = NULL;
	std::ifstream in;
	in.open(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		in.close();
		return false;
	}
	TGA_Header header;
	in.read((char*)&header, sizeof(header));
	if (!in.good()) {
		in.close();
		std::cerr << "an error occured while reading the header\n";
		return false;
	}
	width = header.width;
	height = header.height;
	bytespp = header.bitsperpixel >> 3;
	if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA)) {
		in.close();
		std::cerr << "bad bpp (or width/height) value\n";
		return false;
	}
	unsigned long nbytes = bytespp * width * height;
	data = new unsigned char[nbytes];
	if (3 == header.datatypecode || 2 == header.datatypecode) {
		in.read((char*)data, nbytes);
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else if (10 == header.datatypecode || 11 == header.datatypecode) {
		if (!load_rle_data(in)) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else {
		in.close();
		std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
		return false;
	}
	if (!(header.imagedescriptor & 0x20)) {
		flip_vertically();
	}
	if (header.imagedescriptor & 0x10) {
		flip_horizontally();
	}
	std::cerr << width << "x" << height << "/" << bytespp * 8 << "\n";
	in.close();
	return true;
}

bool TGAImage::load_rle_data(std::ifstream& in) {
	unsigned long pixelcount = width * height;
	unsigned long currentpixel = 0;
	unsigned long currentbyte = 0;
	TGAColor colorbuffer;
	do {
		unsigned char chunkheader = 0;
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader < 128) {
			chunkheader++;
			for (int i = 0; i < chunkheader; i++) {
				in.read((char*)colorbuffer.raw, bytespp);
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int t = 0; t < bytespp; t++)
					data[currentbyte++] = colorbuffer.raw[t];
				currentpixel++;
				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
		else {
			chunkheader -= 127;
			in.read((char*)colorbuffer.raw, bytespp);
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}
			for (int i = 0; i < chunkheader; i++) {
				for (int t = 0; t < bytespp; t++)
					data[currentbyte++] = colorbuffer.raw[t];
				currentpixel++;
				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);
	return true;
}

bool TGAImage::write_tga_file(const char* filename, bool rle) {
	unsigned char developer_area_ref[4] = { 0, 0, 0, 0 };
	unsigned char extension_area_ref[4] = { 0, 0, 0, 0 };
	unsigned char footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };
	std::ofstream out;
	out.open(filename, std::ios::binary);
	if (!out.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		out.close();
		return false;
	}
	TGA_Header header;
	memset((void*)&header, 0, sizeof(header));
	header.bitsperpixel = bytespp << 3;
	header.width = width;
	header.height = height;
	header.datatypecode = (bytespp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
	header.imagedescriptor = 0x20; // top-left origin
	out.write((char*)&header, sizeof(header));
	if (!out.good()) {
		out.close();
		std::cerr << "can't dump the tga file\n";
		return false;
	}
	if (!rle) {
		out.write((char*)data, width * height * bytespp);
		if (!out.good()) {
			std::cerr << "can't unload raw data\n";
			out.close();
			return false;
		}
	}
	else {
		if (!unload_rle_data(out)) {
			out.close();
			std::cerr << "can't unload rle data\n";
			return false;
		}
	}
	out.write((char*)developer_area_ref, sizeof(developer_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char*)extension_area_ref, sizeof(extension_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char*)footer, sizeof(footer));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.close();
	return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
bool TGAImage::unload_rle_data(std::ofstream& out) {
	const unsigned char max_chunk_length = 128;
	unsigned long npixels = width * height;
	unsigned long curpix = 0;
	while (curpix < npixels) {
		unsigned long chunkstart = curpix * bytespp;
		unsigned long curbyte = curpix * bytespp;
		unsigned char run_length = 1;
		bool raw = true;
		while (curpix + run_length < npixels && run_length < max_chunk_length) {
			bool succ_eq = true;
			for (int t = 0; succ_eq && t < bytespp; t++) {
				succ_eq = (data[curbyte + t] == data[curbyte + t + bytespp]);
			}
			curbyte += bytespp;
			if (1 == run_length) {
				raw = !succ_eq;
			}
			if (raw && succ_eq) {
				run_length--;
				break;
			}
			if (!raw && !succ_eq) {
				break;
			}
			run_length++;
		}
		curpix += run_length;
		out.put(raw ? run_length - 1 : run_length + 127);
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
		out.write((char*)(data + chunkstart), (raw ? run_length * bytespp : bytespp));
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
	}
	return true;
}

TGAColor TGAImage::get(int x, int y) {
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		return TGAColor();
	}
	return TGAColor(data + (x + y * width) * bytespp, bytespp);
}

bool TGAImage::set(int x, int y, TGAColor c) {
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		return false;
	}
	memcpy(data + (x + y * width) * bytespp, c.raw, bytespp);
	return true;
}

int TGAImage::get_bytespp() {
	return bytespp;
}

int TGAImage::get_width() {
	return width;
}

int TGAImage::get_height() {
	return height;
}

bool TGAImage::flip_horizontally() {
	if (!data) return false;
	int half = width >> 1;
	for (int i = 0; i < half; i++) {
		for (int j = 0; j < height; j++) {
			TGAColor c1 = get(i, j);
			TGAColor c2 = get(width - 1 - i, j);
			set(i, j, c2);
			set(width - 1 - i, j, c1);
		}
	}
	return true;
}

bool TGAImage::flip_vertically() {
	if (!data) return false;
	unsigned long bytes_per_line = width * bytespp;
	unsigned char* line = new unsigned char[bytes_per_line];
	int half = height >> 1;
	for (int j = 0; j < half; j++) {
		unsigned long l1 = j * bytes_per_line;
		unsigned long l2 = (height - 1 - j) * bytes_per_line;
		memmove((void*)line, (void*)(data + l1), bytes_per_line);
		memmove((void*)(data + l1), (void*)(data + l2), bytes_per_line);
		memmove((void*)(data + l2), (void*)line, bytes_per_line);
	}
	delete[] line;
	return true;
}

unsigned char* TGAImage::buffer() {
	return data;
}

void TGAImage::clear() {
	memset((void*)data, 0, width * height * bytespp);
}

bool TGAImage::scale(int w, int h) {
	if (w <= 0 || h <= 0 || !data) return false;
	unsigned char* tdata = new unsigned char[w * h * bytespp];
	int nscanline = 0;
	int oscanline = 0;
	int erry = 0;
	unsigned long nlinebytes = w * bytespp;
	unsigned long olinebytes = width * bytespp;
	for (int j = 0; j < height; j++) {
		int errx = width - w;
		int nx = -bytespp;
		int ox = -bytespp;
		for (int i = 0; i < width; i++) {
			ox += bytespp;
			errx += w;
			while (errx >= (int)width) {
				errx -= width;
				nx += bytespp;
				memcpy(tdata + nscanline + nx, data + oscanline + ox, bytespp);
			}
		}
		erry += h;
		oscanline += olinebytes;
		while (erry >= (int)height) {
			if (erry >= (int)height << 1) // it means we jump over a scanline
				memcpy(tdata + nscanline + nlinebytes, tdata + nscanline, nlinebytes);
			erry -= height;
			nscanline += nlinebytes;
		}
	}
	delete[] data;
	data = tdata;
	width = w;
	height = h;
	return true;
}

