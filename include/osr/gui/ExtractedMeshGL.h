/*
	This file is part of the implementation for the technical paper

		Field-Aligned Online Surface Reconstruction
		Nico Schertler, Marco Tarini, Wenzel Jakob, Misha Kazhdan, Stefan Gumhold, Daniele Panozzo
		ACM TOG 36, 4, July 2017 (Proceedings of SIGGRAPH 2017)

	Use of this source code is granted via a BSD-style license, which can be found
	in License.txt in the repository root.

	@author Nico Schertler
*/

#pragma once

#include "osr/ExtractedMesh.h"
#include "osr/common.h"
#include <nsessentials/gui/GLBuffer.h>
#include <nsessentials/gui/GLVertexArray.h>
#include "osr/gui/tools/Selection.h"

namespace osr
{
	namespace gui
	{
		//used by the fallback renderer to write the fine mesh into vertex / index buffers
		class FineMeshToBufferVisitor : public MeshVisitor
		{
		public:
			FineMeshToBufferVisitor(nse::gui::GLBuffer& positionBuffer, nse::gui::GLBuffer& colorBuffer, nse::gui::GLBuffer& indexBuffer);

			void begin(unsigned int vertices, unsigned int faces);
			void addVertex(const Eigen::Vector3f& position, const Eigen::Vector3f& color);
			void addVertex(const Eigen::Vector3f& position, const Eigen::Vector4f& color);
			void addFace(unsigned int count, const uint32_t* indices);
			void end();

			unsigned int indexCount() const;

		private:
			Eigen::Matrix<float, 4, Eigen::Dynamic> positions;
			Eigen::Matrix<float, 4, Eigen::Dynamic> colors;
			unsigned int nextVertex;
			MatrixXu indices;
			unsigned int nextFace;

			nse::gui::GLBuffer &positionBuffer, &colorBuffer, &indexBuffer;
		};

		//Adds rendering functionality to the extracted mesh
		class ExtractedMeshGL : public ExtractedMesh
		{
		public:
			ExtractedMeshGL(const MeshSettings& meshSettings);
			~ExtractedMeshGL();

			void draw(const Eigen::Matrix4f& modelView, const Eigen::Matrix4f& projection);

			bool drawAdjacency;
			bool drawCollapsed;
			bool drawExtracted;
			bool drawModified;
			bool wireframe;
			bool coarseWireframe;
			bool highlightBoundary;

			void reset();

		protected:
			void post_extract();
			void adjacencyData(std::vector<Vector3f>&& adj, std::vector<Vector3f>&& color);
			void collapsedGraphData(std::vector<Vector3f>&& adj, std::vector<Vector3f>&& color);
			void modifiedData(std::vector<Vector3f>& points);
		private:
			//These buffers are for SSBO-supported rendering
			nse::gui::GLBuffer vertexBuffer, edgeBuffer, triBuffer, quadBuffer, colorBuffer;
			//This is a plain buffer for SSBO-unsupported rendering
			nse::gui::GLBuffer vertexBufferNoSSBO, colorBufferNoSSBO, indexBuffer;
			unsigned int indexCount;
			nse::gui::GLVertexArray mesh;

			nse::gui::GLBuffer edgesWireframeBuffer;
			nse::gui::GLVertexArray edgesWireframe;
			int edgesWireframeSize;

			nse::gui::GLVertexArray adjacency;

			std::vector<Vector3f> adjacencyVis;
			std::vector<Vector3f> adjacencyVisColor;
			nse::gui::GLBuffer adjColorBuffer, adjPositionBuffer;
			bool adjDirty, modifiedDirty;

			nse::gui::GLVertexArray collapsed;
			std::vector<Vector3f> collapsedVis;
			std::vector<Vector3f> collapsedVisColor;
			nse::gui::GLBuffer collapsedColorBuffer, collapsedPositionBuffer;
			bool collapsedDirty;

			tools::Selection modifiedSet;
		};
	}
}