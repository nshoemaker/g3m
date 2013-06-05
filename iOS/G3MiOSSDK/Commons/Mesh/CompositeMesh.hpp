//
//  CompositeMesh.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/17/12.
//
//

#ifndef __G3MiOSSDK__CompositeMesh__
#define __G3MiOSSDK__CompositeMesh__

#include "Mesh.hpp"

class CompositeMesh : public Mesh {
private:
  std::vector<Mesh*> _children;

  Extent* calculateExtent() const;

  mutable Extent* _extent;
  

public:
  virtual ~CompositeMesh();
  
  int getVertexCount() const;

  const Vector3D getVertex(int i) const;

  void render(const G3MRenderContext* rc) const;

  Extent* getExtent() const;

  bool isTransparent(const G3MRenderContext* rc) const;

  void addMesh(Mesh* mesh);

  //GLClient NotDrawable

  void notifyGLClientChildrenParentHasChanged();
  void modifyGLGlobalState(GLGlobalState& GLGlobalState) const{}
  void modifyGPUProgramState(GPUProgramState& progState) const{}
  
  void rawRender(const G3MRenderContext* rc, GLStateTreeNode* myStateTreeNode);
  bool isVisible(const G3MRenderContext* rc);
  void modifiyGLState(GLState* state);


};

#endif
