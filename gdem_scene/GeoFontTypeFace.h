#pragma once

#include <QString>
#include "IVideoDriver.h"
#include "../gdem_freetypelib/include/VecMath.hpp"
using namespace geo;

typedef Vector2<size_t> Vec2size_t;

struct TypeFace_Data;
struct TypeFace_CacheEntry;
struct FT_GlyphRec_;
struct FT_BitmapGlyphRec_;

class CGeoFontTypeFace
{
	friend class CGeoFont;
public:
	explicit CGeoFontTypeFace( const QString& aFileName, size_t aPointSize = 12, size_t aResolution = 96 );
	virtual ~CGeoFontTypeFace();
public:
	// Get/Set Point Size of the font.
	size_t		getPointSize() const;
	void		setPointSize( size_t aPointSize );

	// Get/Set Resolution of the font in DPI
	size_t		getResolution() const;
	void		setResolution( size_t aResolution );

	// Renders the passed in aString at the passed in position
	void		Render( const QString& aString, const Vec2f& aPosition, bool upsideDown=false, video::SColor aColor = video::SColor(255, 255, 255, 255));

	float		getAscent() const;
	float		getDescent() const;

	// Returns the height of a line rendered in this type face.
	float		getLineHeight() const;

	// Returns the total width of the passed in text in pixels, assuming the text is rendered on one line.
	float		getWidgth( const QString& aString );
	float		getCharWidth(const unsigned short uStr);

	// Returns the index of the character at the position determined by the passed in offset from the beginning of the string.
	int			hitCharacterIndex( const QString& aString, float anOffset );

	Vec2size_t	getMaximumGlyphSize() const;
	
	TypeFace_Data* getTypeFaceData() {return data_;}

private:

	// Renders the glyph with the passed in index.
	Vec2f		renderGlyph( size_t aGlyphIndex, const Vec2f& aPosition, unsigned int afontflag = 0);

	// Add a new texture to the cache textures.
	void		addNewTexture( const Vec2size_t& aGlyphSize );

	// Puts a new entry into the glyph cache.
	void		addCacheEntry( FT_GlyphRec_* aGlyph, FT_BitmapGlyphRec_* aBitmapGlyph, size_t aGlyphIndex, const Vec2size_t& aGlyphSize );

	// Checks that a cache texture is available, adding a new one if necessary.
	void		ensureTextureIsAvailable();

	// Returns the cache entry for the glyph with the passed in index.
	TypeFace_CacheEntry& cachedGlyph( size_t aGlyphIndex, unsigned int afontflag = 0);

	// Returns the kerning vector for the passed in pair of glyphs.
	Vec2f		kerning( size_t leftGlyphIndex, size_t rightGlyphIndex ) const;

	// Renders the glyphs of the passed in string.
	void		renderGlyphs( const QString& aString, unsigned int afontflag = 0);

	// binds the passed in cache entry's texture.
	void		bindTexture( const TypeFace_CacheEntry& aCacheEntry ) const;

	// Caches the glyph with the passed in index.
	void		cacheGlyph( size_t aGlyphIndex, unsigned int afontflag = 0);

	// Flushes the string and character cache.
	void		flushCache();

	// All member data pertaining to the font is encapsulated an instance of \c Data
	TypeFace_Data* data_; 

	bool m_bDrawing;

};
