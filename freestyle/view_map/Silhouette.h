//
//  Filename         : Silhouette.h
//  Author(s)        : Stephane Grabli
//  Purpose          : Classes to define a silhouette structure
//  Date of creation : 25/03/2002
//
///////////////////////////////////////////////////////////////////////////////


//
//  Copyright (C) : Please refer to the COPYRIGHT file distributed 
//   with this source distribution. 
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef  SILHOUETTE_H
# define SILHOUETTE_H

# include <iostream>
# include <string>
# include <vector>
# include <set>
# include <float.h>
# include <assert.h>
# include "../system/FreestyleConfig.h"
# include "../geometry/Geom.h"
# include "../geometry/BBox.h"
# include "../scene_graph/Material.h"
# include "../geometry/Polygon.h"
# include "../system/Exception.h"
# include "Interface0D.h"
# include "Interface1D.h"
# include "../winged_edge/Curvature.h"

using namespace std;
using namespace Geometry;

class ViewShape;
class WFace;
class WEdge;
typedef vector<ViewShape*> occluder_container;


                  /**********************************/
                  /*                                */
                  /*                                */
                  /*             SVertex            */
                  /*                                */
                  /*                                */
                  /**********************************/

class FEdge;
class ViewVertex;
class SShape;

/*! Class to define a vertex of the embedding. */
class LIB_VIEW_MAP_EXPORT SVertex : public Interface0D
{
public: // Implementation of Interface0D

  /*! Returns the string "SVertex" .*/
  virtual string getExactTypeName() const {
    return "SVertex";
  }

  // Data access methods

  /*! Returns the 3D x coordinate of the vertex .*/
  virtual real getX() const {
    return _Point3D.x();
  }

  /*! Returns the 3D y coordinate of the vertex .*/
  virtual real getY() const {
    return _Point3D.y();
  }

  /*! Returns the 3D z coordinate of the vertex .*/
  virtual real getZ() const {
    return _Point3D.z();
  }

  /*!  Returns the 3D point. */ 
  virtual Vec3f getPoint3D() const {
    return _Point3D;
  }

  /*! Returns the projected 3D  x coordinate of the vertex .*/
  virtual real getProjectedX() const {
    return _Point2D.x();
  }

  /*! Returns the projected 3D  y coordinate of the vertex .*/
  virtual real getProjectedY() const {
    return _Point2D.y();
  }

  /*! Returns the projected 3D  z coordinate of the vertex .*/
  virtual real getProjectedZ() const {
    return _Point2D.z();
  }

  /*!  Returns the 2D point. */ 
  virtual Vec2f getPoint2D() const {
    return Vec2f((float)_Point2D.x(),(float)_Point2D.y());
  }

  /*! Returns the FEdge that lies between this Svertex and the
   *  Interface0D given as argument. */
  virtual FEdge* getFEdge(Interface0D&);

  /*! Returns the Id of the vertex .*/
  virtual Id getId() const {
    return _Id;
  }

  /*! Returns the nature of the vertex .*/
  virtual Nature::VertexNature getNature() const;

  /*! Cast the Interface0D in SVertex if it can be. */ 
  virtual SVertex * castToSVertex();

  /*! Cast the Interface0D in ViewVertex if it can be. */ 
  virtual ViewVertex * castToViewVertex();

  /*! Cast the Interface0D in NonTVertex if it can be. */ 
  virtual NonTVertex * castToNonTVertex();

  /*! Cast the Interface0D in TVertex if it can be. */ 
  virtual TVertex * castToTVertex();

public:

  typedef vector<FEdge*> fedges_container;

private:

  Id _Id;
  Vec3r _Point3D;
  Vec3r _Point2D;
  set<Vec3r> _Normals; 
  vector<FEdge*> _FEdges; // the edges containing this vertex
  SShape *_Shape;  // the shape to which belongs the vertex
  ViewVertex *_pViewVertex; // The associated viewvertex, in case there is one.
  real _curvatureFredo;
  Vec2r _directionFredo;
    CurvatureInfo* _curvature_info;
    bool _POProjected; // has this been projected to the punch-out surface?
    
    
    WEdge * _sourceEdge; // added by Aaron: for smooth curves, the mesh edge that this svertex lies on, if any
    WVertex * _sourceVertex;  // for sharp edges: mesh vertex this svertex came from

public:

  /*! A field that can be used by the user to store any data.
   *  This field must be reseted afterwards using ResetUserData().
   */
  void *userdata;

  
  /*! Default constructor.*/
  inline SVertex() {
    _Id = 0;
    userdata = NULL;
    _Shape = NULL;
    _pViewVertex = 0;
    _curvature_info = 0;
    _POProjected = false;
    _sourceEdge = NULL;
    _sourceVertex = NULL;
  }

  /*! Builds a SVertex from 3D coordinates and an Id. */
  inline SVertex(const Vec3r &iPoint3D, const Id& id) {
    _Point3D = iPoint3D;
    _Id=id;
    userdata = NULL;
    _Shape = NULL;
    _pViewVertex=0;
    _curvature_info = 0;
    _POProjected = false;
    _sourceEdge = NULL;
    _sourceVertex = NULL;
  }

  /*! Copy constructor. */
  inline SVertex(SVertex& iBrother) {
    _Id = iBrother._Id;
    _Point3D =  iBrother.point3D();
    _Point2D = iBrother.point2D();
    _Normals = iBrother._Normals;
    _FEdges = iBrother.fedges();
    _Shape = iBrother.shape();
    _pViewVertex = iBrother._pViewVertex;
    if (!(iBrother._curvature_info))
      _curvature_info = 0;
    else
      _curvature_info = new CurvatureInfo(*(iBrother._curvature_info));
    iBrother.userdata = this;
    userdata = 0;
    _POProjected = iBrother._POProjected;
    _sourceEdge = iBrother._sourceEdge;
    _sourceVertex = iBrother._sourceVertex;
  }

  /*! Destructor. */
  virtual ~SVertex() {
    if (_curvature_info)
      delete _curvature_info;
  }

  /*! Cloning method. */
  virtual SVertex * dupplicate() {
    SVertex *clone = new SVertex(*this);
    return clone;
  }

  /*! operator == */
  virtual bool operator==(const SVertex& iBrother) {
    return ((_Point2D == iBrother._Point2D) &&
	    (_Point3D == iBrother._Point3D));
  }

  /* accessors */
  inline const Vec3r& point3D() const {return _Point3D;}
  inline const Vec3r& point2D() const {return _Point2D;}
  /*! Returns the set of normals for this Vertex.
   *  In a smooth surface, a vertex has exactly one normal.
   *  In a sharp surface, a vertex can have any number of normals.
   */
  inline set<Vec3r> normals() {return _Normals;}
  /*! Returns the number of different normals for this vertex. */
  inline unsigned normalsSize() const {return _Normals.size();}
  inline const vector<FEdge*>& fedges() {return _FEdges;}
  inline fedges_container::iterator fedges_begin() {return _FEdges.begin();}
  inline fedges_container::iterator fedges_end() {return _FEdges.end();}
  inline SShape * shape() {return _Shape;}
  inline real z() const {return _Point2D[2];}
  /*! If this SVertex is also a ViewVertex, this method
   *  returns a pointer onto this ViewVertex. 0 is returned
   *  otherwise.
   */
  inline ViewVertex * viewvertex() {return _pViewVertex;}

  /*! modifiers */
  /*! Sets the 3D coordinates of the SVertex. */
  inline void SetPoint3D(const Vec3r &iPoint3D) {_Point3D = iPoint3D;}
  /*! Sets the 3D projected coordinates of the SVertex. */
  inline void SetPoint2D(const Vec3r &iPoint2D) {_Point2D = iPoint2D;}
  /*! Adds a normal to the Svertex's set of normals. If the same
   * normal is already in the set, nothing changes.
   */
  inline void AddNormal(const Vec3r& iNormal) 
  {
    _Normals.insert(iNormal); // if iNormal in the set already exists, nothing is done
  }

  void setCurvatureInfo(CurvatureInfo* ci) {
    _curvature_info = ci;
  }

  const CurvatureInfo* getCurvatureInfo() const {
    return _curvature_info;
  }

  real GetIsophoteDistance(real isovalue, int maxDistance) const;  // get it from mesh point this vertex came from
  Vec2r ImageSpaceNormal() const;

  /* Fredo's normal and curvature*/
  void setCurvatureFredo(real c) {_curvatureFredo=c;}
  void setDirectionFredo(Vec2r d) {_directionFredo=d;}
  real curvatureFredo () {return _curvatureFredo;}
  const Vec2r directionFredo () {return _directionFredo;}

  /*! Sets the Id */
  inline void SetId(const Id& id) {_Id = id;}
  inline void SetFEdges(const vector<FEdge*>& iFEdges) {_FEdges = iFEdges;}
  inline void SetShape(SShape *iShape) {_Shape = iShape;}
  inline void SetViewVertex(ViewVertex *iViewVertex) {_pViewVertex = iViewVertex;}
  /*! Add an FEdge to the list of edges emanating from this SVertex. */
  inline void AddFEdge(FEdge* iFEdge) {_FEdges.push_back(iFEdge);}
  /* replaces edge 1 by edge 2 in the list of edges */
  inline void Replace(FEdge *e1, FEdge *e2)
  {
    vector<FEdge*>::iterator insertedfe;
    for(vector<FEdge*>::iterator fe=_FEdges.begin(),fend=_FEdges.end();
        fe!=fend;
        fe++)
      {
        if((*fe) == e1)
          {
            insertedfe = _FEdges.insert(fe, e2);// inserts e2 before fe.
                                                // returns an iterator pointing toward e2. fe is invalidated.
            // we want to remove e1, but we can't use fe anymore:
            insertedfe++; // insertedfe points now to e1
            _FEdges.erase(insertedfe);
            return;
          }
      }
  }

  Vec3r getColorID();

    void SetSourceEdge(WEdge * edge) { _sourceEdge = edge; };
    WEdge * GetSourceEdge() const { return _sourceEdge; };
    void SetSourceVertex(WVertex * v) { _sourceVertex = v; };
    WVertex * GetSourceVertex() const { return _sourceVertex; }

public:

  /* Information access interface */

  FEdge *fedge() ; // for non T vertex
  inline const Vec3r& point2d() const {return point2D();}
  inline const Vec3r& point3d() const {return point3D();}
  inline Vec3r normal() const {if(_Normals.size() == 1) return (*(_Normals.begin())); Exception::raiseException(); return *(_Normals.begin());}
  //Material material() const ;
  Id shape_id() const ;
  const SShape* shape() const ;
  float shape_importance() const ;
  
  const int qi() const ;
  occluder_container::const_iterator occluders_begin() const ;
  occluder_container::const_iterator occluders_end() const ;
  bool occluders_empty() const ;
  int occluders_size() const ;
  const Polygon3r& occludee() const ;
  const SShape * occluded_shape() const ;
  const bool  occludee_empty() const ;
  real z_discontinuity() const ;
  //inline float local_average_depth() const ;
  //  inline float local_depth_variance() const ;
  //  inline real local_average_density(float sigma = 2.3f) const ;
  //inline Vec3r shaded_color() const ;
  //  inline Vec3r orientation2d() const ; 
  //  inline Vec3r orientation3d() const ;
  //  inline Vec3r curvature2d_as_vector() const ;
  /*! angle in radians */
  //  inline real curvature2d_as_angle() const ;

  bool POProjected() { return _POProjected; }
  void SetPOProjected() { _POProjected = true; }
  
};

                  /**********************************/
                  /*                                */
                  /*                                */
                  /*             FEdge              */
                  /*                                */
                  /*                                */
                  /**********************************/


class ViewEdge;
/*! Base Class for feature edges.
 *  This FEdge can represent a silhouette, a crease,
 *  a ridge/valley, a border or a suggestive contour.
 *  For silhouettes,  the FEdge is oriented
 *  such as, the visible face lies on the left of the edge.
 *  For borders, the FEdge is oriented
 *  such as, the face lies on the left of the edge.
 *  An FEdge can represent an initial edge of the mesh
 *  or runs accross a face of the initial mesh depending
 *  on the smoothness or sharpness of the mesh.
 *  This class is specialized into a smooth and a sharp
 *  version since their properties slightly vary from
 *  one to the other.
 */
class LIB_VIEW_MAP_EXPORT FEdge : public Interface1D
{
public: // Implementation of Interface0D

  /*! Returns the string "FEdge" . */
  virtual string getExactTypeName() const {
    return "FEdge";
  }

  // Data access methods

  /*! Returns the 2D length of the FEdge. */
  virtual real getLength2D() const {
    if (!_VertexA || !_VertexB)
      return 0;
    return (_VertexB->getPoint2D() - _VertexA->getPoint2D()).norm();
  }

  /*! Returns the Id of the FEdge. */
  virtual Id getId() const {
    return _Id;
  }

public:

  // An edge can only be of one kind (SILHOUETTE or BORDER, etc...)
  // For an multi-nature edge there must be several different FEdge.
   // DEBUG:
  //  Vec3r A;
  //  Vec3r u;
  //  vector<Polygon3r> _Occludees;
  //  Vec3r intersection;
  //  vector<Vec3i> _Cells;

protected:
  SVertex *_VertexA;
  SVertex *_VertexB;
  Id _Id;
  Nature::EdgeNature _Nature;
  //vector<Polygon3r> _Occluders; // visibility // NON GERE PAR LE COPY CONSTRUCTOR!!
  
  FEdge *_NextEdge; // next edge on the chain
  FEdge *_PreviousEdge;
  ViewEdge *_ViewEdge;
  // Sometimes we need to deport the visibility computation onto another 
  // edge. For example the exact edges use edges of the mesh to 
  // compute their visibility
  
  Polygon3r _aFace; // The occluded face which lies on the right of a silhouette edge
  Vec3r _occludeeIntersection;
  bool _occludeeEmpty;

  bool _isSmooth;

  WVertex * _visSource; // for debugging region-based visibility: the vertex that this fedge got it's
  // visibility vote from.

public:
  /*! A field that can be used by the user to store any data.
   *  This field must be reseted afterwards using ResetUserData().
   */
  void *userdata;
  /*! Default constructor */
  inline FEdge() {
    userdata = NULL;
    _Nature = Nature::NO_FEATURE;
    _NextEdge = NULL;
    _PreviousEdge = NULL;
    _ViewEdge = NULL;
    //_hasVisibilityPoint=false;
    _occludeeEmpty = true;
    _isSmooth = false;
    _visSource = NULL;
  }
  /*! Builds an FEdge going from vA to vB. */  
  inline FEdge(SVertex *vA, SVertex *vB) {
    userdata = NULL;
    _VertexA = vA;
    _VertexB = vB;
    _Nature = Nature::NO_FEATURE;
    _NextEdge=NULL;
    _PreviousEdge=NULL;
    _ViewEdge = NULL;
    //_hasVisibilityPoint=false;
    _occludeeEmpty = true;
    _isSmooth = false;
    _visSource = NULL;
  }
  /*! Copy constructor */
  inline FEdge(FEdge& iBrother)
  {
    _VertexA = iBrother.vertexA();
    _VertexB = iBrother.vertexB();
    _NextEdge = iBrother.nextEdge();
    _PreviousEdge = iBrother._PreviousEdge;
    _Nature = iBrother.getNature();
    _Id = iBrother._Id;
    _ViewEdge = iBrother._ViewEdge;
    //_hasVisibilityPoint = iBrother._hasVisibilityPoint;
    //_VisibilityPointA = iBrother._VisibilityPointA;
    //_VisibilityPointB = iBrother._VisibilityPointB;
    _aFace = iBrother._aFace;
    _occludeeEmpty = iBrother._occludeeEmpty;
    _isSmooth = iBrother._isSmooth;
    iBrother.userdata = this;
    userdata = 0;
    _visSource = iBrother._visSource;
  }
  /*! Destructor */
  virtual ~FEdge() {}
  /*! Cloning method. */
  virtual FEdge* dupplicate()
  {
    FEdge *clone = new FEdge(*this);
    return clone;
  }
  /* accessors */
  /*! Returns the first SVertex. */
  inline SVertex* vertexA() {return _VertexA;}
  /*! Returns the second SVertex. */
  inline SVertex* vertexB() {return _VertexB;}
  /*! Returns the first SVertex if i=0, the seccond SVertex
   *  if i=1. */
  inline SVertex* operator[](const unsigned short int& i) const{ 
      return i%2==0 ? _VertexA : _VertexB;
  }
  /*! Returns the nature of the FEdge. */
  inline Nature::EdgeNature getNature() const {return _Nature;}
  /*! Returns the FEdge following this one in the ViewEdge.
   *  If this FEdge is the last of the ViewEdge, 0 is returned.
   */
  inline FEdge * nextEdge() {return _NextEdge;}
  /*! Returns the Edge preceding this one in the ViewEdge.
   *  If this FEdge is the first one of the ViewEdge, 0 is returned.
   */
  inline FEdge * previousEdge() {return _PreviousEdge;}
  inline SShape * shape() {return _VertexA->shape();}
  //inline int invisibility() const {return _Occluders.size();}
  int invisibility() const ;
  //inline const vector<Polygon3r>& occluders() const {return _Occluders;}
  /*! Returns a pointer to the ViewEdge to which this FEdge belongs to. */
  inline ViewEdge * viewedge() const {return _ViewEdge;}
  inline Vec3r center3d() {return Vec3r((_VertexA->point3D()+_VertexB->point3D())/2.0);}
  inline Vec3r center2d() {return Vec3r((_VertexA->point2D()+_VertexB->point2D())/2.0);}
  //  inline bool hasVisibilityPoint() const {return _hasVisibilityPoint;}
  //  inline Vec3r visibilityPointA() const {return _VisibilityPointA;}
  //  inline Vec3r visibilityPointB() const {return _VisibilityPointB;}
  inline const Polygon3r& aFace() const {return _aFace;}
  inline const Vec3r& getOccludeeIntersection() { return _occludeeIntersection; }
  inline bool getOccludeeEmpty() { return _occludeeEmpty; }
  /*! Returns true if this FEdge is a smooth FEdge. */
  inline bool isSmooth() const {return _isSmooth;}
  inline WVertex * visSource() const { return _visSource; }

  /* modifiers */
  /*! Sets the first SVertex. */
  inline void SetVertexA(SVertex *vA) {_VertexA = vA;}
  /*! Sets the second SVertex. */
  inline void SetVertexB(SVertex *vB) {_VertexB = vB;}
  /*! Sets the FEdge Id . */
  inline void SetId(const Id& id) {_Id = id;}
  /*! Sets the pointer to the next FEdge. */
  inline void SetNextEdge(FEdge* iEdge) {_NextEdge = iEdge;}
  /*! Sets the pointer to the previous FEdge. */
  inline void SetPreviousEdge(FEdge *iEdge) {_PreviousEdge = iEdge;}
  /*! Sets the nature of this FEdge. */
  inline void SetNature(Nature::EdgeNature iNature) {_Nature = iNature;}
  //inline void AddOccluder(Polygon3r& iPolygon) {_Occluders.push_back(iPolygon);}
  /*! Sets the ViewEdge to which this FEdge belongs to. */
  inline void SetViewEdge(ViewEdge *iViewEdge) {_ViewEdge = iViewEdge;}
  //  inline void SetHasVisibilityPoint(bool iBool) {_hasVisibilityPoint = iBool;}
  //  inline void SetVisibilityPointA(const Vec3r& iPoint) {_VisibilityPointA = iPoint;}
  //  inline void SetVisibilityPointB(const Vec3r& iPoint) {_VisibilityPointB = iPoint;}
  inline void SetaFace(Polygon3r& iFace) {_aFace = iFace;}
  inline void SetOccludeeIntersection(const Vec3r& iPoint) {_occludeeIntersection = iPoint;}
  inline void SetOccludeeEmpty(bool iempty) {_occludeeEmpty = iempty;}
  /*! Sets the flag telling whether this FEdge is smooth or sharp.
   *  true for Smooth, false for Sharp.
   */
  inline void SetSmooth(bool iFlag) {_isSmooth = iFlag;}
  inline void SetVisSource(WVertex * vertex) { _visSource = vertex; }
  
  /* checks whether two FEdge have a common vertex.
   *  Returns a pointer on the common vertex if it exists, 
   *  NULL otherwise.
   */
  static inline SVertex* CommonVertex(FEdge *iEdge1, FEdge* iEdge2)
  {
    if((NULL == iEdge1) || (NULL == iEdge2))
      return NULL;

    SVertex *sv1 = iEdge1->vertexA();
    SVertex *sv2 = iEdge1->vertexB();
    SVertex *sv3 = iEdge2->vertexA();
    SVertex *sv4 = iEdge2->vertexB();

    if((sv1 == sv3) || (sv1 == sv4))
    {
      return sv1;
    }
    else if((sv2 == sv3) || (sv2 == sv4))
    {
      return sv2;
    }

    return NULL;
  }

  inline const SVertex* min2d() const 
  {
    if(_VertexA->point2D() < _VertexB->point2D())
      return _VertexA;
    else
      return _VertexB;
  }
  inline const SVertex* max2d() const 
  {
    if(_VertexA->point2D() < _VertexB->point2D())
      return _VertexB;
    else
      return _VertexA;
  }
 
  /* Information access interface */
  /* Information access interface */
 
  //Material material() const ;
  Id shape_id() const ;
  const SShape * shape() const ;
  float shape_importance() const ;
  inline const int qi() const {return invisibility();}
  occluder_container::const_iterator occluders_begin() const ;
  occluder_container::const_iterator occluders_end() const ;
  bool occluders_empty() const ;
  int occluders_size() const ;
  inline const Polygon3r& occludee() const {return aFace();}
  const SShape * occluded_shape() const ;
  //inline const bool  occludee_empty() const {return _occludeeEmpty;}
  const bool  occludee_empty() const ;
  real z_discontinuity() const ;
  //  inline float local_average_depth(int iCombination = 0) const ;
  //  inline float local_depth_variance(int iCombination = 0) const ;
  //  inline real local_average_density(float sigma = 2.3f, int iCombination = 0) const ;
  //inline Vec3r shaded_color(int iCombination = 0) const {}
  int viewedge_nature() const ;
  //float viewedge_length() const ;
  inline Vec3r orientation2d() const {return Vec3r(_VertexB->point2d()-_VertexA->point2d());}
  inline Vec3r orientation3d() const {return Vec3r(_VertexB->point3d()-_VertexA->point3d());}
  //  //inline real curvature2d() const {return viewedge()->curvature2d((_VertexA->point2d()+_VertexB->point2d())/2.0);}
  //  inline Vec3r curvature2d_as_vector(int iCombination = 0) const ;
  //  /* angle in degrees*/
  //  inline real curvature2d_as_angle(int iCombination = 0) const ;  


  // Iterator access (Interface1D)
  /*! Returns an iterator over the 2 (!) SVertex
   *  pointing to the first SVertex. */
  virtual inline Interface0DIterator verticesBegin();
  /*! Returns an iterator over the 2 (!) SVertex
   *  pointing after the last SVertex. */
  virtual inline Interface0DIterator verticesEnd();

  /*! Returns an iterator over the FEdge points,
   *  pointing to the first point. The difference with
   *  verticesBegin() is that here we can iterate over
   *  points of the FEdge at a any given sampling.
   *  Indeed, for each iteration, a virtual point is created.
   *  \param t
   *    The sampling with which we want to iterate over points of
   *    this FEdge.
   */
  virtual inline Interface0DIterator pointsBegin(float t=0.f);
  /*! Returns an iterator over the FEdge points,
   *  pointing after the last point. The difference with
   *  verticesEnd() is that here we can iterate over
   *  points of the FEdge at a any given sampling.
   *  Indeed, for each iteration, a virtual point is created.
   *  \param t
   *    The sampling with which we want to iterate over points of
   *    this FEdge.
   */
  virtual inline Interface0DIterator pointsEnd(float t=0.f);

  // get the face(s) this edge is from
  virtual inline WFace * getFace1() { assert(0); return NULL; }
  virtual inline WFace * getFace2() { assert(0); return NULL; }

  bool intersectParametric(FEdge & fe2, Vec3r viewpoint, real t, real u);
};

//
// SVertexIterator
//
/////////////////////////////////////////////////

namespace FEdgeInternal {

  class SVertexIterator : public Interface0DIteratorNested
  {
  public:

    SVertexIterator() {
      _vertex = NULL;
      _edge = NULL;
    }

    SVertexIterator(const SVertexIterator& vi) {
      _vertex = vi._vertex;
      _edge = vi._edge;
    }

    SVertexIterator(SVertex* v, FEdge* edge) {
      _vertex = v;
      _edge = edge;
    }

    SVertexIterator& operator=(const SVertexIterator& vi) {
      _vertex = vi._vertex;
      _edge = vi._edge;
      return *this;
    }

    virtual string getExactTypeName() const {
      return "SVertexIterator";
    }

    virtual SVertex& operator*() {
      return *_vertex;
    }

    virtual SVertex* operator->() {
      return &(operator*());
    }

    virtual SVertexIterator& operator++() {
      increment();
      return *this;
    }

    virtual SVertexIterator operator++(int) {
      SVertexIterator ret(*this);
      increment();
      return ret;
    }

    virtual SVertexIterator& operator--() {
      decrement();
      return *this;
    }

    virtual SVertexIterator operator--(int) {
      SVertexIterator ret(*this);
      decrement();
      return ret;
    }

    virtual void increment() {
      if (_vertex == _edge->vertexB()) {
	_vertex = 0;
	return;
      }

      _vertex = _edge->vertexB();
    }

    virtual void decrement() {
      if (_vertex == _edge->vertexA()) {
	_vertex = 0;
	return;
      }
      _vertex = _edge->vertexA();
    }

    virtual bool isBegin() const {
      return _vertex == _edge->vertexA();
    }

    virtual bool isEnd() const {
      return _vertex == _edge->vertexB();
    }

    virtual bool operator==(const Interface0DIteratorNested& it) const {
      const SVertexIterator* it_exact = dynamic_cast<const SVertexIterator*>(&it);
      if (!it_exact)
	return false;
      return ((_vertex == it_exact->_vertex) &&
	      (_edge == it_exact->_edge));
    }

    virtual float t() const{
      if(_vertex == _edge->vertexA()){
        return 0;
      }
      return ((float)_edge->getLength2D());
    }
    virtual float u() const{
      if(_vertex == _edge->vertexA()){
        return 0;
      }
      return 1.0;
    }
    virtual SVertexIterator* copy() const {
      return new SVertexIterator(*this);
    }

  private:

    SVertex*	_vertex;
    FEdge*	_edge;
  };

} // end of namespace FEdgeInternal

// Iterator access (implementation)

Interface0DIterator FEdge::verticesBegin() {
  Interface0DIterator ret(new FEdgeInternal::SVertexIterator(_VertexA, this));
  return ret;
}

Interface0DIterator FEdge::verticesEnd() {
  Interface0DIterator ret(new FEdgeInternal::SVertexIterator(0, this));
  return ret;
}

Interface0DIterator FEdge::pointsBegin(float t) {
  return verticesBegin();
}

Interface0DIterator FEdge::pointsEnd(float t) {
  return verticesEnd();
}

/*! Class defining a sharp FEdge. A Sharp FEdge
 *  corresponds to an initial edge of the input mesh.
 *  It can be a silhouette, a crease or a border.
 *  If it is a crease edge, then it is borded
 *  by two faces of the mesh. Face a lies on its right
 *  whereas Face b lies on its left.
 *  If it is a border edge, then it doesn't have any
 *  face on its right, and thus Face a = 0.
 */
class WEdge;
class LIB_VIEW_MAP_EXPORT FEdgeSharp : public FEdge
{
protected:
  Vec3r _aNormal; // When following the edge, normal of the right face
  Vec3r _bNormal; // When following the edge, normal of the left face
  unsigned _aMaterialIndex;
  unsigned _bMaterialIndex;

  WEdge * _edge;

public:
  /*! Default constructor. */
  inline FEdgeSharp() : FEdge(){
    _aMaterialIndex = _bMaterialIndex = 0;
    _edge = NULL;
    //    _faceA =_faceB = NULL;
  }
  /*! Builds an FEdgeSharp going from vA to vB. */ 
  inline FEdgeSharp(SVertex *vA, SVertex *vB) : FEdge(vA, vB){
    _aMaterialIndex = _bMaterialIndex = 0;
    _edge = NULL;
    //    _faceA = _faceB = NULL;
  }
  /*! Copy constructor. */
  inline FEdgeSharp(FEdgeSharp& iBrother) : FEdge(iBrother){
    _aNormal = iBrother._aNormal;
    _bNormal = iBrother._bNormal;
    _aMaterialIndex = iBrother._aMaterialIndex;
    _bMaterialIndex = iBrother._bMaterialIndex;
    _edge = iBrother._edge;
    //    _faceA = iBrother._faceA; _faceB = iBrother._faceB;
  }
  /*! Destructor. */
  virtual ~FEdgeSharp() { }
  /*! Cloning method. */
  virtual FEdge* dupplicate(){
    FEdge *clone = new FEdgeSharp(*this);
    return clone;
  }
  /*! Returns the normal to the face lying on the
   *  right of the FEdge. If this FEdge is a border,
   *  it has no Face on its right and therefore, no normal.
   */
  inline const Vec3r& normalA() {return _aNormal;}
  /*! Returns the normal to the face lying on the
   *  left of the FEdge.
   */
  inline const Vec3r& normalB() {return _bNormal;}
  /*! Returns the index of the material of the face lying on the
   *  right of the FEdge. If this FEdge is a border,
   *  it has no Face on its right and therefore, no material.
   */
  inline unsigned aMaterialIndex() const {return _aMaterialIndex;}
  /*! Returns the material of the face lying on the
   *  right of the FEdge. If this FEdge is a border,
   *  it has no Face on its right and therefore, no material.
   */
  const Material& aMaterial() const ;
  /*! Returns the index of the material of the face lying on the
   *  left of the FEdge.
   */
  inline unsigned bMaterialIndex() const {return _bMaterialIndex;}
  /*! Returns the  material of the face lying on the
   *  left of the FEdge.
   */
  const Material& bMaterial() const ;
  
  inline WEdge * edge() const { return _edge; } 

  //  inline WFace * faceA() const { return _faceA; }
  //  inline WFace * faceB() const { return _faceB; }

  /*! Sets the normal to the face lying on the right of the FEdge. */
  inline void SetNormalA(const Vec3r& iNormal) {_aNormal = iNormal;}
  /*! Sets the normal to the face lying on the left of the FEdge. */ 
  inline void SetNormalB(const Vec3r& iNormal) {_bNormal = iNormal;}
  /*! Sets the index of the material lying on the right of the FEdge.*/
  inline void SetaMaterialIndex(unsigned i) {_aMaterialIndex = i;}
  /*! Sets the index of the material lying on the left of the FEdge.*/
  inline void SetbMaterialIndex(unsigned i) {_bMaterialIndex = i;}
  
  inline void SetEdge(WEdge * edge) { _edge = edge; }

  virtual WFace * getFace1();
  virtual WFace * getFace2();

  //inline void SetFaceA(WFace * faceA) { _faceA = faceA; }
  //  inline void SetFaceB(WFace * faceB) { _faceB = faceB; }

};

/*! Class defining a smooth edge. This kind of edge typically
 *  runs across a face of the input mesh. It can be
 *  a silhouette, a ridge or valley, a suggestive contour.
 */
class LIB_VIEW_MAP_EXPORT FEdgeSmooth : public FEdge
{
protected:
  Vec3r _Normal;
  unsigned _MaterialIndex;
  //  bool _hasVisibilityPoint;
  //  Vec3r _VisibilityPointA;  // The edge on which the visibility will be computed represented 
  //  Vec3r _VisibilityPointB;  // using its 2 extremity points A and B
  void * _Face; // In case of exact silhouette, Face is the WFace crossed by Fedge 
               // NON GERE PAR LE COPY CONSTRUCTEUR
public:
  /*! Default constructor. */
  inline FEdgeSmooth() : FEdge(){
    _Face=0;
    _MaterialIndex = 0;
    _isSmooth = true;
  }
  /*! Builds an FEdgeSmooth going from vA to vB. */  
  inline FEdgeSmooth(SVertex *vA, SVertex *vB) : FEdge(vA, vB){
    _Face=0;
    _MaterialIndex = 0;
    _isSmooth = true;

  }
  /*! Copy constructor. */
  inline FEdgeSmooth(FEdgeSmooth& iBrother) : FEdge(iBrother){
    _Normal = iBrother._Normal;
    _Face = iBrother._Face;
    _MaterialIndex = iBrother._MaterialIndex;
    _isSmooth = true;
  }
  /*! Destructor. */
  virtual ~FEdgeSmooth() {}
  /*! Cloning method. */
  virtual FEdge* dupplicate(){
    FEdge *clone = new FEdgeSmooth(*this);
    return clone;
  }

  inline void * face() const {return _Face;}
  inline virtual WFace * getFace1() { return (WFace*)_Face; }
  inline virtual WFace * getFace2() { return NULL; }

  /*! Returns the normal to the Face it is running accross. */  // Aaron: no, it seems to be an approximation to the interpolated normal at the center of the face
  inline const Vec3r& normal() {return _Normal;}
  /*! Returns the index of the material of the face it is running accross. */
  inline unsigned materialIndex() const {return _MaterialIndex;}
  /*! Returns the material of the face it is running accross. */
  const Material& material() const ;

  inline void SetFace(void * iFace) {_Face = iFace;}
  /*! Sets the normal to the Face it is running accross. */
  inline void SetNormal(const Vec3r& iNormal) {_Normal = iNormal;}
  /*! Sets the index of the material of the face it is running accross. */
  inline void SetMaterialIndex(unsigned i) {_MaterialIndex = i;}
};

// edge that comes as a result of triangle-triangle intersections
class LIB_VIEW_MAP_EXPORT FEdgeIntersection : public FEdgeSmooth
{  // inelegant to make this extend FEdgeSmooth instead of FEdge, perhaps, but this is less
  // likely to make things break for now.  (main effect of being "smooth" appears to be cusp detection 
  // and ray casting)
protected:
  WFace * _face1;// the two faces that intersected to produce this edge
  WFace * _face2; 

public:
  inline FEdgeIntersection() : FEdgeSmooth() { _face1 = _face2 = NULL; };
  inline FEdgeIntersection(SVertex *vA, SVertex *vB) : FEdgeSmooth(vA, vB)
  { _face1 = _face2 = NULL; }
  inline FEdgeIntersection(FEdgeIntersection & fei) : FEdgeSmooth(fei) 
  { _face1 = fei._face1; _face2 = fei._face2; _isSmooth = false; };
  virtual ~FEdgeIntersection() { };
  virtual FEdge * dupplicate() { return new FEdgeIntersection(*this); };
  virtual inline WFace * getFace1() { return _face1; }
  virtual inline WFace * getFace2() { return _face2; }
  inline void SetFaces(WFace * f1, WFace * f2) { _face1 = f1; _face2 = f2; }
};


class POBoundaryEdge;

/*
class LIB_VIEW_MAP_EXPORT FEdgePOIntersection : public FEdgeSmooth
{
protected:
  // intersection between a mesh face and a boundary edge

  POBoundaryEdge * _bnd; // boundary edge source
  
public:
  inline FEdgePOIntersection() : FEdgeSmooth() { _bnd = NULL; };
  inline FEdgePOIntersection(SVertex * vA, SVertex * vB) : FEdgeSmooth(vA, vB) { _bnd = NULL; };
  inline FEdgePOIntersection(FEdgePOIntersection & fep) : FEdgeSmooth(fep) { _bnd = fep._bnd; }
  virtual ~FEdgePOIntersection() {};
  virtual FEdge * dupplicate() { return new FEdgePOIntersection(*this); };
  inline POBoundaryEdge * getBnd() { return _bnd; }
  inline void SetBnd(POBoundaryEdge * bnd) { _bnd = bnd; };
};
*/


                  /**********************************/
                  /*                                */
                  /*                                */
                  /*             SShape             */
                  /*                                */
                  /*                                */
                  /**********************************/


/*! Class to define a feature shape. It is the gathering 
 *  of feature elements from an identified input shape
 */
class LIB_VIEW_MAP_EXPORT SShape
{
private:
  vector<FEdge*> _chains;          // list of fedges that are chains starting points.
  vector<SVertex*> _verticesList;  // list of all vertices
  vector<FEdge*>   _edgesList;     // list of all edges
  Id _Id;
  BBox<Vec3r> _BBox;
  vector<Material> _Materials;  

  float _importance;

  ViewShape *_ViewShape;

public:
  /*! A field that can be used by the user to store any data.
   *  This field must be reseted afterwards using ResetUserData().
   */
  void* userdata; // added by E.T.
  /*! Default constructor */
  inline SShape()
  {
    userdata = 0;

    _importance = 0.f;
    _ViewShape = 0;
  }
  /*! Copy constructor */
  inline SShape(SShape& iBrother)
  {
    userdata = 0;
    _Id = iBrother._Id;
    _BBox = iBrother.bbox();
    _Materials = iBrother._Materials;

    _importance = iBrother._importance;

    _ViewShape = iBrother._ViewShape;

     
    //---------
    // vertices
    //---------
    vector<SVertex*>::iterator sv,svend;
    vector<SVertex*>& verticesList = iBrother.GetVertexList();
    for(sv=verticesList.begin(), svend=verticesList.end();
        sv!=svend;
        sv++)
      {
        SVertex *newv = new SVertex(*(*sv));
        newv->SetShape(this);
        _verticesList.push_back(newv);
      }
    
    //------
    // edges
    //------
    vector<FEdge*>::iterator e,eend;
    vector<FEdge*>& edgesList = iBrother.GetEdgeList();
    for(e=edgesList.begin(),eend=edgesList.end();
        e!=eend;
        e++)
      {
        FEdge *newe = (*e)->dupplicate();
        _edgesList.push_back(newe);
      } 

        //-------------------------
        // starting chain edges
        //-------------------------
        vector<FEdge*>::iterator fe,fend;
        vector<FEdge*>& fedges = iBrother.GetChains();
        for(fe=fedges.begin(),fend=fedges.end();
        fe!=fend;
        fe++)
        {
          _chains.push_back((FEdge*)((*fe)->userdata));
        }
        
    
    //-------------------------
    // remap edges in vertices:
    //-------------------------
    for(sv=_verticesList.begin(),svend=_verticesList.end();
        sv!=svend;
        sv++)
      {
        const vector<FEdge*>& fedgeList = (*sv)->fedges();
        vector<FEdge*> newfedgelist;
        for(vector<FEdge*>::const_iterator fed=fedgeList.begin(),fedend=fedgeList.end();
            fed!=fedend;
            fed++)
          {
            FEdge *current = *fed;
            newfedgelist.push_back((FEdge*)current->userdata);
          }
        (*sv)->SetFEdges(newfedgelist); 
      }
    
    //-------------------------------------
    // remap vertices and nextedge in edges:
    //-------------------------------------
    for(e=_edgesList.begin(),eend=_edgesList.end();
        e!=eend;
        e++)
      {
        (*e)->SetVertexA((SVertex*)((*e)->vertexA()->userdata));
        (*e)->SetVertexB((SVertex*)((*e)->vertexB()->userdata));
        (*e)->SetNextEdge((FEdge*)((*e)->nextEdge()->userdata));
        (*e)->SetPreviousEdge((FEdge*)((*e)->previousEdge()->userdata));
      }
    

    // reset all brothers userdata to NULL:
    //-------------------------------------
    //---------
    // vertices
    //---------
    for(sv=_verticesList.begin(),svend=_verticesList.end();
        sv!=svend;
        sv++)
      {
        (*sv)->userdata = NULL;
      }

    //------
    // edges
    //------
    for(e=_edgesList.begin(),eend=_edgesList.end();
        e!=eend;
        e++)
      {
        (*e)->userdata = NULL;
      }  
  }
  /*! Cloning method. */
  virtual SShape * dupplicate()
  {
    SShape *clone = new SShape(*this);
    return clone;
  }
  /*! Destructor. */
  virtual  ~SShape();

  /*! Adds a FEdge to the list of FEdges. */
  inline void AddEdge(FEdge *iEdge)
    {
      _edgesList.push_back(iEdge);
    }

  /*! Adds a SVertex to the list of SVertex of this Shape.
   * The SShape attribute of the SVertex is also set to 'this'.
   */
  inline void AddNewVertex(SVertex* iv) {iv->SetShape(this);_verticesList.push_back(iv);}
  inline void AddChain(FEdge *iEdge){
    _chains.push_back(iEdge);
  }

  inline SVertex * CreateSVertex(const Vec3r& P3D, const Vec3r& P2D, const Id& id)
  {
    SVertex *Ia = new SVertex(P3D, id);
    Ia->SetPoint2D(P2D);
    AddNewVertex(Ia);
    return Ia;
  }
  /* splits an edge into several edges. 
   *  The edge's vertices are passed rather than 
   *  the edge itself. This way, all feature edges (SILHOUETTE,
   *  CREASE, BORDER) are splitted in the same time.
   *  The processed edges are flagged as done (using the userdata
   *  flag).One single new vertex is created whereas 
   *  several splitted edges might created for the different 
   *  kinds of edges. These new elements are added to the lists
   *  maintained by the shape.
   *  new chains are also created.
   *    ioA
   *      The first vertex for the edge that gets splitted
   *    ioB
   *      The second vertex for the edge that gets splitted
   *    iParameters
   *      A vector containing 2D real vectors indicating the parameters
   *      giving the intersections coordinates in 3D and in 2D.
   *      These intersections points must be sorted from B to A.
   *      Each parameter defines the intersection point I as I=A+T*AB.
   *      T<0 and T>1 are then incorrect insofar as they give intersections
   *      points that lie outside the segment.
   *    ioNewEdges
   *      The edges that are newly created (the initial edges are not 
   *      included) are added to this list.
   */
  void SplitEdge(FEdge *fe, const vector<Vec2r>& iParameters, vector<FEdge*>& ioNewEdges);

  /* splits an edge into 2 edges. The new vertex and edge are added
   *  to the sshape list of vertices and edges
   *  a new chain is also created.
   *  returns the new edge.
   *    ioEdge
   *      The edge that gets splitted
   *    newpoint
   *      x,y,z coordinates of the new point.
   */
  FEdge* SplitEdgeIn2(FEdge* ioEdge, SVertex * ioNewVertex);

  /*! Sets the Bounding Box of the Shape */
  inline void SetBBox(const BBox<Vec3r>& iBBox) {_BBox = iBBox;}

  /*! Compute the bbox of the sshape */
  inline void ComputeBBox()
  {
    if(0 == _verticesList.size())
      return;

    Vec3r firstVertex = _verticesList[0]->point3D();
    real XMax = firstVertex[0];
    real YMax = firstVertex[1];
    real ZMax = firstVertex[2];

    real XMin = firstVertex[0];
    real YMin = firstVertex[1];
    real ZMin = firstVertex[2];

    vector<SVertex*>::iterator v,vend;
    // parse all the coordinates to find 
    // the Xmax, YMax, ZMax
    for(v=_verticesList.begin(),vend=_verticesList.end();
        v!=vend;
        v++)
      {
        Vec3r vertex = (*v)->point3D();
        // X
        real x = vertex[0];
        if(x > XMax)
          XMax = x;
        if(x < XMin)
          XMin = x;
        
        // Y
        real y = vertex[1];
        if(y > YMax)
          YMax = y;
        if(y < YMin)
          YMin = y;
        
        // Z
        real z = vertex[2];
        if(z > ZMax)
          ZMax = z;
        if(z < ZMin)
          ZMin = z;
      }
    
    
    SetBBox(BBox<Vec3r>(Vec3r(XMin, YMin, ZMin), Vec3r(XMax, YMax, ZMax)));
  }

  inline void RemoveEdgeFromChain(FEdge *iEdge)
  {
    for(vector<FEdge*>::iterator fe=_chains.begin(), feend=_chains.end();
    fe!=feend;
    fe++)
    {
      if(iEdge == (*fe))
      {
        _chains.erase(fe);
        break;
      }
    }
  }

  inline void RemoveEdge(FEdge *iEdge)
  {
    for(vector<FEdge*>::iterator fe=_edgesList.begin(), feend=_edgesList.end();
    fe!=feend;
    fe++)
    {
      if(iEdge == (*fe))
      {
        _edgesList.erase(fe);
        break;
      }
    }
  }

  /* accessors */
  /*! Returns the list of SVertex of the Shape. */
  inline vector<SVertex*>& GetVertexList() {return _verticesList;} // Get vertices list
  /*! Returns the list of FEdges of the Shape. */
  inline vector<FEdge*>& GetEdgeList() {return _edgesList;} // Get edges list
  inline vector<FEdge*>& GetChains() {return _chains;}
  /*! Returns the bounding box of the shape. */
  inline const BBox<Vec3r>& bbox() {return _BBox;}
  /*! Returns the ith material of the shape. */
  inline const Material& material(unsigned i) const {return _Materials[i];}
  /*! Returns the list of materials of the Shape. */
  inline const vector<Material>& materials() const {return _Materials;}
  inline ViewShape * viewShape() {return _ViewShape;}
  inline float importance() const {return _importance;}
  /*! Returns the Id of the Shape. */
  inline Id getId() const { return _Id; }
  
  /* Modififers */
  /*! Sets the Id of the shape.*/
  inline void SetId(Id id) {_Id = id;}
  /*! Sets the list of materials for the shape */
  inline void SetMaterials(const vector<Material>& iMaterials) {_Materials = iMaterials;}
  inline void SetViewShape(ViewShape *iShape) {_ViewShape = iShape;}
  inline void SetImportance(float importance){_importance = importance;}

void RemoveVertex(SVertex* sv)
{
  for(vector<SVertex*>::iterator it = _verticesList.begin(); it!= _verticesList.end();++it)
    if ( (*it)==sv)
      {
	_verticesList.erase(it);
	return;
      }
}
};

#endif // SILHOUETTE_H
