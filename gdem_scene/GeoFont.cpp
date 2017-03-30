
#include "GeoFont.h"
#include <QFile>
#include <QChar>
#include <QString>
#include "GLee.h"
#include "gdemclientbase.h"

CGeoFont::CGeoFont(float sizeI, const QString& ttfFileName)
        : m_typeFace(ttfFileName, (size_t)(sizeI), 84)
{
}

void CGeoFont::printOutlinedString(float x, float y, const QString& s, bool speedOptimize, float xshift, float yshift, 
								   video::SColor aColor, unsigned int afontflag, float scale) const
{
	float charLen;
	GLint mMode;

	glGetIntegerv(GL_MATRIX_MODE, &mMode);
	glMatrixMode(GL_MODELVIEW);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

	glTranslatef(x,y,0.f);
	glTranslatef(xshift, -yshift, 0.f);
	glScalef(scale, scale, 0.f);
	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);

	GLfloat current_color[3];
	glGetFloatv(GL_CURRENT_COLOR, current_color);	 

	glColor3f((float) aColor.getRed() / 255.f, (float) aColor.getGreen() / 255.f, (float) aColor.getBlue() / 255.f);

	for (int i = 0; i < s.length(); i++)
	{
		if ( !speedOptimize )
		{
			printCharOutlined(s[i], afontflag);			
		}
		else
		{
			printChar(s[i]);
		}

		//if(i == 0)
		charLen = getStrLen(s.mid(i,1));

		glTranslatef(charLen * 1.05f, 0.f, 0.f);

	}

	glColor3fv(current_color);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(mMode);

	if (afontflag & FONT_UNDERLINE)
		drawUnderline(x, y, s, aColor, true);
	if (afontflag & FONT_STRIKEOUT)
		drawUnderline(x, y, s, aColor, false);

}

void CGeoFont::printCharOutlined(const QChar c, unsigned int afontflag) const
{
 	GLfloat current_color[3];
 	glGetFloatv(GL_CURRENT_COLOR, current_color);	 
 	 
 	glColor3f(0.0f, 0.0f, 0.0f);
 	
 	glPushMatrix();
 	glTranslatef(1.0f, 1.0f, 0.0f);		
 	m_typeFace.renderGlyphs(c, afontflag);
 	glPopMatrix();
 	
 	glPushMatrix();
 	glTranslatef(-1.0f, -1.0f, 0.0f);		
 	m_typeFace.renderGlyphs(c, afontflag);
 	glPopMatrix();
 	
 	glPushMatrix();
 	glTranslatef(1.0f, -1.0f ,0.0f );		
 	m_typeFace.renderGlyphs(c, afontflag);
 	glPopMatrix();
 	
 	glPushMatrix();
 	glTranslatef(-1.0f, 1.0f, 0.0f);		
 	m_typeFace.renderGlyphs(c, afontflag);
 	glPopMatrix();
 	
 	glColor3fv(current_color);
	
	m_typeFace.renderGlyphs(c, afontflag);

}

 void CGeoFont::drawUnderline(float x, float y, const QString& s, video::SColor aColor, bool bUnderline) const
 {
	 glPushAttrib(GL_ALL_ATTRIB_BITS);
	 glPushMatrix();

	 if (bUnderline)
		 glTranslatef(x + 1.f, y + 5.f,0.f);
	 else
		 glTranslatef(x + 1.f, y - (getLineHeight() / 2.f - 5.f), 0.f);

	 glDisable(GL_BLEND);
	 glDisable(GL_TEXTURE_2D);
	 
	 glColor3f((float) aColor.getRed() / 255.f, (float) aColor.getGreen() / 255.f, (float) aColor.getBlue() / 255.f);

	 glBegin(GL_LINES);
	 {
		 glVertex2f(0.f, 0.f);
		 glVertex2f(getStrLen(s), 0.f);
	 }
	 glEnd();

	 glPopMatrix();
	 glPopAttrib();
 }
