#pragma once

#ifndef Tr2PngHandler_h
#define Tr2PngHandler_h

#include "Tr2ImageHandler.h"
#include "png.h"

class Tr2PngHandler : public Tr2ImageHandler
{
public:
	Tr2PngHandler( const wchar_t* sourceName );
	virtual ~Tr2PngHandler();

	virtual bool SetStream( IBlueStream* src );	
	virtual bool ReadHeader();
	virtual bool ReadImage();

	virtual bool IsSupported();
	virtual Tr2RenderContextEnum::PixelFormat GetFormat() const;

	virtual unsigned GetBlockByteSize() const;

	bool IsValid() const;

	virtual unsigned GetOffset( unsigned mipLevel, unsigned face ) const;

	virtual bool IsSaveSupported( const Tr2BitmapDimensions& bd );
	virtual bool Save( const Tr2HostBitmap& image, IBlueStream* output );
    
private:
	bool DoReadHeader();
	bool DoReadImage( png_bytep* rowPtrs );
	bool DoSaveImage( png_structp png, png_infop info, int transforms );

	unsigned GetColorType() const	{ return IsValid() ? png_get_color_type  ( m_png, m_info ) : 0; }

	png_structp	m_png;
	png_infop	m_info;
	
	unsigned	m_channels;

	jmp_buf		m_jmpBuf;
	bool		m_haveError;

	void ReadData   ( png_bytep data, png_size_t length );
	void UserError  ( png_const_charp errorMsg);
    void UserWarning( png_const_charp warningMsg);

	static void PNGAPI ReadData_thunk( png_structp pngPtr, png_bytep data, png_size_t length );
	static void PNGAPI UserError_thunk  ( png_structp pngPtr, png_const_charp errorMsg);
    static void PNGAPI UserWarning_thunk( png_structp pngPtr, png_const_charp warningMsg);

	static png_voidp MemoryAlloc( png_structp, png_alloc_size_t );
	static void MemoryFree( png_structp, png_voidp );

	Tr2PngHandler( const Tr2PngHandler & );
	Tr2PngHandler& operator=( const Tr2PngHandler &);
};

#endif // Tr2PngHandler_h
