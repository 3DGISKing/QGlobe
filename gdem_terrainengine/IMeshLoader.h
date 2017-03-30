// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __I_MESH_LOADER_H_INCLUDED__
#define __I_MESH_LOADER_H_INCLUDED__

#include "IReferenceCounted.h"
//2013 2 18 added by ugi
#include "IVideoDriver.h"
//end
#include "path.h"

namespace geo
{
namespace io
{
	class IReadFile;
} // end namespace io
namespace scene
{
	class IAnimatedMesh;

//! Class which is able to load an animated mesh from a file.
/** If you want Geo be able to load meshes of
currently unsupported file formats (e.g. .cob), then implement
this and add your new Meshloader with
ISceneManager::addExternalMeshLoader() to the engine. */
class IMeshLoader : public virtual IReferenceCounted
{
//2013 2 17 added by ugi
protected:
	video::IVideoDriver* driver;
	//end
public:

	//! Destructor
	virtual ~IMeshLoader() {}

	//! Returns true if the class might be able to load this file.
	/** This decision should be based on the file extension (e.g. ".vrml")
	only.
	\param filename Name of the file to test.
	\return True if the extension is a recognised type. */
	virtual bool isALoadableFileExtension(const io::path& filename) const = 0;

	//! Creates/loads an animated mesh from the file.
	/** \param file File handler to load the file from.
	\return Pointer to the created mesh. Returns 0 if loading failed.
	If you no longer need the mesh, you should call IAnimatedMesh::drop().
	See IReferenceCounted::drop() for more information. */
	virtual IAnimatedMesh* createMesh(io::IReadFile* file) = 0;
	//2013 2 17 added by ugi
	void setDriver(video::IVideoDriver* driver){this->driver=driver;}
	//end

};


} // end namespace scene
} // end namespace geo

#endif

