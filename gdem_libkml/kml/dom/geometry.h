// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the declarations for the abstract Geometry element
// and concrete coordinates, Point, LineString, LinearRing, Polygon,
// outerBoundaryIs, and innerBoundaryIs elements.

// In addition to classes for the abstract and concrete elements in the
// KML standard there are internal convenience classes used here to hold
// common code.  Each such class is named *GeometryCommon and follows
// this general pattern: constructor is protected, implements set,get,has,clear
// for the field it owns, and parses that field (implements AddElement).
// Each concrete element owns serialization of all fields for itself as per
// the order the KML standard.  The KML standard does not specify the common
// simple elements in an order that maps well to a type hierarchy hence
// the more typical pattern of abstract types serializing their own
// fields is not followed here.
//
// Here is a quick summary of the type hierarchy used and what fields
// are associated with each type:
//
// class Geometry : public Object
//   AbstractGeometryGroup in the KML standard.  No child elements.
// class AltitudeGeometryCommon : public Geometry
//   Geometry with <altitudeMode>
// class ExtrudeGeometryCommon : public AltitudeGeometryCommon
//   Geometry with <altitudeMode> + <extrude>
// class CoordinatesGeometryCommon : public ExtrudeGeometryCommon
//   Geometry with <altitudeMode> + <extrude> + <coordinates>
// class Point : public CoordinatesGeometryCommon
//   <Point> has <altitudeMode> + <extrude> + <coordinates>
// class LineCommon : public CoordinatesGeometryCommon
//   LineCommon has <altitudeMode> + <extrude> + <coordinates> + <tessellate>
// class LineString : public LineCommon
//   <LineString> is an instantiation of LineCommon
// class LinearRing : public LineCommon
//   <LinearRing> is an instantiation of LineCommon
// class BoundaryCommon : public Element
//   BoundaryCommon has <LinearRing>
// class OuterBoundaryIs : public BoundaryCommon
//  <outerBoundaryIs> is an instantiation of BoundaryCommon
// class InnerBoundaryIs : public BoundaryCommon
//  <innerBoundaryIs> is an instantiation of BoundaryCommon
// class Polygon : public ExtrudeGeometryCommon
//   <Polygon> has <altitudeMode> + <extrude> + <tessellate> +
//      <outerBoundaryIs> and N x <innerBoundaryIs>
// class MultiGeometry : public Geometry
// Note: class Model : public AltitudeGeometryCommon

#ifndef KML_DOM_GEOMETRY_H__
#define KML_DOM_GEOMETRY_H__

#include <string>
#include <vector>
#include "kml/dom/kml22.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/object.h"
#include "kml/base/util.h"
#include "kml/base/vec3.h"

namespace kmldom {

class Serializer;

// <coordinates>
class Coordinates : public BasicElement<Type_coordinates> {
 public:
  virtual ~Coordinates();

  // The main KML-specific API
  void add_latlngalt(double latitude, double longitude, double altitude) {
    coordinates_array_.push_back(kmlbase::Vec3(longitude, latitude, altitude));
  }

  void add_latlng(double latitude, double longitude) {
    coordinates_array_.push_back(kmlbase::Vec3(longitude, latitude, 0.0));
  }

  void add_vec3(const kmlbase::Vec3& vec3) {
    coordinates_array_.push_back(vec3);
  }

  size_t get_coordinates_array_size() const {
    return coordinates_array_.size();
  }

  const kmlbase::Vec3 get_coordinates_array_at(size_t index) const {
    return coordinates_array_[index];
  }

  // Internal methods used in parser.  Public for unittest purposes.
  // See .cc for more details.
  void Parse(const std::string& char_data);
  static bool ParseVec3(const char* coords, char** nextp, kmlbase::Vec3* vec);

  // This clears the internal coordinates array.
  void Clear() {
    coordinates_array_.clear();
  }

 private:
  friend class KmlFactory;
  Coordinates();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;

  std::vector<kmlbase::Vec3> coordinates_array_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Coordinates);
};

// OGC KML 2.2 Standard: 10.1 kml:AbstractGeometryGroup
// OGC KML 2.2 XSD: <element name="AbstractGeometryGroup"...
class Geometry : public Object {
 public:
  virtual ~Geometry();
  virtual KmlDomType Type() const { return Type_Geometry; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Geometry || Object::IsA(type);
  }

 protected:
  // Geometry is abstract.
  Geometry();

 private:
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Geometry);
};

// Internal convenience class for any Geometry with <altitudeMode>.
// This is not in the KML standard, hence there is no type info.
class AltitudeGeometryCommon : public Geometry {
 public:
  virtual ~AltitudeGeometryCommon();

 protected:
  AltitudeGeometryCommon();

 public:
  // <altitudeMode>
  int get_altitudemode() const { return altitudemode_; }
  bool has_altitudemode() const { return has_altitudemode_; }
  void set_altitudemode(int value) {
    altitudemode_ = value;
    has_altitudemode_ = true;
  }
  void clear_altitudemode() {
    altitudemode_ = ALTITUDEMODE_CLAMPTOGROUND;
    has_altitudemode_ = false;
  }

  // <gx:altitudeMode>
  int get_gx_altitudemode() const { return gx_altitudemode_; }
  bool has_gx_altitudemode() const { return has_gx_altitudemode_; }
  void set_gx_altitudemode(int value) {
    gx_altitudemode_ = value;
    has_gx_altitudemode_ = true;
  }
  void clear_gx_altitudemode() {
    gx_altitudemode_ = GX_ALTITUDEMODE_CLAMPTOSEAFLOOR;
    has_gx_altitudemode_ = false;
  }

  virtual void AddElement(const ElementPtr& element);

 private:
  int altitudemode_;
  bool has_altitudemode_;
  int gx_altitudemode_;
  bool has_gx_altitudemode_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(AltitudeGeometryCommon);
};

// Internal convenience class for any Geometry with <altitudeMode> + <extrude>
// This is not in the KML standard, hence there is no type info.
class ExtrudeGeometryCommon : public AltitudeGeometryCommon {
 public:
  virtual ~ExtrudeGeometryCommon();

  // <extrude>
  bool get_extrude() const { return extrude_; }
  bool has_extrude() const { return has_extrude_; }
  void set_extrude(bool value) {
    extrude_ = value;
    has_extrude_ = true;
  }
  void clear_extrude() {
    extrude_ = false;
    has_extrude_ = false;
  }

 protected:
  ExtrudeGeometryCommon();
  virtual void AddElement(const ElementPtr& element);

 private:
  bool extrude_;
  bool has_extrude_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(ExtrudeGeometryCommon);
};

// Internal convenience class for any Geometry with
// <altitudeMode> + <extrude> + <coordinates>.
// This is not in the KML standard, hence there is no type info.
class CoordinatesGeometryCommon : public ExtrudeGeometryCommon {
 public:
  virtual ~CoordinatesGeometryCommon();

 public:
  // <coordinates>
  const CoordinatesPtr get_coordinates() const { return coordinates_; }
  bool has_coordinates() const { return coordinates_ != NULL; }
  void set_coordinates(const CoordinatesPtr& coordinates) {
    SetComplexChild(coordinates, &coordinates_);
  }
  void clear_coordinates() {
    set_coordinates(NULL);
  }

 protected:
  CoordinatesGeometryCommon();
  // Parser support
  virtual void AddElement(const ElementPtr& element);

 private:
  CoordinatesPtr coordinates_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(CoordinatesGeometryCommon);
};

// <Point>
class Point : public CoordinatesGeometryCommon {
 public:
  virtual ~Point();
  virtual KmlDomType Type() const { return Type_Point; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Point || Geometry::IsA(type);
  }

 private:
  friend class KmlFactory;
  Point();
  friend class Serializer;
  void Serialize(Serializer& serializer) const;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Point);
};

// Internal convenience class for code common to LineString and LinearRing.
// This is not in the KML standard, hence there is no type info.
class LineCommon : public CoordinatesGeometryCommon {
 public:
  virtual ~LineCommon();

 public:
  // <tessellate>
  bool get_tessellate() const { return tessellate_; }
  bool has_tessellate() const { return has_tessellate_; }
  void set_tessellate(bool value) {
    tessellate_ = value;
    has_tessellate_ = true;
  }
  void clear_tessellate() {
    tessellate_ = false;
    has_tessellate_ = false;
  }

 protected:
  LineCommon();
  // Parser support
  virtual void AddElement(const ElementPtr& element);

 private:
  friend class Serializer;
  void Serialize(Serializer& serializer) const;
  bool tessellate_;
  bool has_tessellate_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(LineCommon);
};

// <LineString>
class LineString : public LineCommon {
 public:
  virtual ~LineString();
  virtual KmlDomType Type() const { return Type_LineString; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_LineString || Geometry::IsA(type);
  }

 private:
  friend class KmlFactory;
  LineString();
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(LineString);
};

// <LinearRing>
class LinearRing : public LineCommon {
 public:
  virtual ~LinearRing();
  virtual KmlDomType Type() const { return Type_LinearRing; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_LinearRing || Geometry::IsA(type);
  }

 private:
  friend class KmlFactory;
  LinearRing();
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(LinearRing);
};

// Internal class for code common to OuterBoundaryIs and InnerBoundaryIs.
// This is not in the KML standard, hence there is no type info.
class BoundaryCommon : public Element {
 public:
  virtual ~BoundaryCommon();

 public:
  const LinearRingPtr get_linearring() const { return linearring_; }
  bool has_linearring() const { return linearring_ != NULL; }
  void set_linearring(const LinearRingPtr& linearring) {
    SetComplexChild(linearring, &linearring_);
  }
  void clear_linearring() {
    set_linearring(NULL);
  }

  // Parser support
  virtual void AddElement(const ElementPtr& element);

 protected:
  BoundaryCommon();
  virtual void Serialize(Serializer& serializer) const;

 private:
  LinearRingPtr linearring_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(BoundaryCommon);
};

// <outerBoundaryIs>
class OuterBoundaryIs : public BoundaryCommon {
 public:
  virtual ~OuterBoundaryIs();
  virtual KmlDomType Type() const { return Type_outerBoundaryIs; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_outerBoundaryIs;
  }

 private:
  friend class KmlFactory;
  OuterBoundaryIs();
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(OuterBoundaryIs);
};

// <innerBoundaryIs>
class InnerBoundaryIs : public BoundaryCommon {
 public:
  virtual ~InnerBoundaryIs();
  virtual KmlDomType Type() const { return Type_innerBoundaryIs; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_innerBoundaryIs;
  }

 private:
  friend class KmlFactory;
  InnerBoundaryIs();
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(InnerBoundaryIs);
};

// <Polygon>
class Polygon : public ExtrudeGeometryCommon {
 public:
  virtual ~Polygon();
  virtual KmlDomType Type() const { return Type_Polygon; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Polygon || Geometry::IsA(type);
  }

  // <tessellate>
  bool get_tessellate() const { return tessellate_; }
  bool has_tessellate() const { return has_tessellate_; }
  void set_tessellate(bool value) {
    tessellate_ = value;
    has_tessellate_ = true;
  }
  void clear_tessellate() {
    tessellate_ = false;
    has_tessellate_ = false;
  }

  // <outerBoundaryIs>
  const OuterBoundaryIsPtr get_outerboundaryis() const {
    return outerboundaryis_;
  }
  bool has_outerboundaryis() const { return outerboundaryis_ != NULL; }
  void set_outerboundaryis(const OuterBoundaryIsPtr& outerboundaryis) {
    SetComplexChild(outerboundaryis, &outerboundaryis_);
  }
  void clear_outerboundaryis() {
    set_outerboundaryis(NULL);
  }

  // <innerBoundaryIs>
  void add_innerboundaryis(const InnerBoundaryIsPtr& innerboundaryis) {
    AddComplexChild(innerboundaryis, &innerboundaryis_array_);
  }

  size_t get_innerboundaryis_array_size() const {
    return innerboundaryis_array_.size();
  }

  const InnerBoundaryIsPtr& get_innerboundaryis_array_at(size_t index) {
    return innerboundaryis_array_[index];
  }

 private:
  friend class KmlFactory;
  Polygon();

  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);

  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;

  bool tessellate_;
  bool has_tessellate_;
  OuterBoundaryIsPtr outerboundaryis_;
  std::vector<InnerBoundaryIsPtr> innerboundaryis_array_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Polygon);
};

// <MultiGeometry>
class MultiGeometry : public Geometry {
 public:
  virtual ~MultiGeometry();
  virtual KmlDomType Type() const { return Type_MultiGeometry; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_MultiGeometry || Geometry::IsA(type);
  }

  // The main KML-specific API
  void add_geometry(const GeometryPtr& geometry);

  size_t get_geometry_array_size() const {
    return geometry_array_.size();
  }

  const GeometryPtr& get_geometry_array_at(size_t index) const {
    return geometry_array_[index];
  }

 private:
  friend class KmlFactory;
  MultiGeometry();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<GeometryPtr> geometry_array_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(MultiGeometry);
};

}  // namespace kmldom

#endif  // KML_DOM_GEOMETRY_H__
