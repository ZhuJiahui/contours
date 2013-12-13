//
//   Copyright 2013 Pixar
//
//   Licensed under the Apache License, Version 2.0 (the "Apache License")
//   with the following modification; you may not use this file except in
//   compliance with the Apache License and the following modification to it:
//   Section 6. Trademarks. is deleted and replaced with:
//
//   6. Trademarks. This License does not grant permission to use the trade
//      names, trademarks, service marks, or product names of the Licensor
//      and its affiliates, except as required to comply with Section 4(c) of
//      the License and to reproduce the content of the NOTICE file.
//
//   You may obtain a copy of the Apache License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the Apache License with the above modification is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
//   KIND, either express or implied. See the Apache License for the specific
//   language governing permissions and limitations under the Apache License.
//

#ifndef OSD_CPU_D3D11_VERTEX_BUFFER_H
#define OSD_CPU_D3D11_VERTEX_BUFFER_H

#include "../version.h"

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace OpenSubdiv {
namespace OPENSUBDIV_VERSION {

///
/// \brief Concrete vertex buffer class for Cpu subvision and DirectX drawing.
///
/// OsdCpuD3D11VertexBuffer implements OsdCpuVertexBufferInterface and
/// OsdD3D11VertexBufferInterface.
///
/// An instance of this buffer class can be passed to OsdCpuComputeController.
///
class OsdCpuD3D11VertexBuffer {
public:
    /// Creator. Returns NULL if error.
    static OsdCpuD3D11VertexBuffer * Create(int numElements, 
                                            int numVertices, 
                                            ID3D11Device *device);

    /// Destructor.
    virtual ~OsdCpuD3D11VertexBuffer();

    /// This method is meant to be used in client code in order to provide coarse
    /// vertices data to Osd.
    void UpdateData(const real *src, int startVertex, int numVertices, void *param);

    /// Returns how many elements defined in this vertex buffer.
    int GetNumElements() const;

    /// Returns how many vertices allocated in this vertex buffer.
    int GetNumVertices() const;

    /// Returns the address of CPU buffer.
    real * BindCpuBuffer();

    /// Returns the D3D11 buffer object.
    ID3D11Buffer *BindD3D11Buffer(ID3D11DeviceContext *deviceContext);

protected:
    /// Constructor.
    OsdCpuD3D11VertexBuffer(int numElements, 
                            int numVertices,
                            ID3D11Device *device);

    bool allocate(ID3D11Device *device);

private:
    int _numElements;
    int _numVertices;
    ID3D11Buffer *_d3d11Buffer;
    real *_cpuBuffer;
};

}  // end namespace OPENSUBDIV_VERSION
using namespace OPENSUBDIV_VERSION;

}  // end namespace OpenSubdiv

#endif  // OSD_CPU_D3D11_VERTEX_BUFFER_H