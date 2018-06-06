#include "osr/MeshVisitor.h"
#include "osr/Colors.h"
#include "osr/common.h"
using namespace osr;

Eigen::Vector3f colorDisplacementToRGBColor(const Vector4f& color)
{
	Vector3us Lab(color.x(), color.y(), color.z());
	auto rgb = LabToRGB(Lab);

	Eigen::Vector3f r;
	for (int i = 0; i < 3; ++i)
		r(i) = std::min(1.0f, std::max(0.0f, (float)rgb(i) / 65535.0f));
	return r;
}

WritePLYMeshVisitor::WritePLYMeshVisitor(const std::string& path)
	: ply(path, std::ios::binary)
{ }

void WritePLYMeshVisitor::begin(unsigned int vertices, unsigned int faces)
{
	ply << "ply" << std::endl;
	ply << "format binary_little_endian 1.0" << std::endl;
	ply << "element vertex " << vertices << std::endl;
	ply << "property float32 x" << std::endl;
	ply << "property float32 y" << std::endl;
	ply << "property float32 z" << std::endl;
	ply << "property uchar red" << std::endl;
	ply << "property uchar green" << std::endl;
	ply << "property uchar blue" << std::endl;
	ply << "element face " << faces << std::endl;
	ply << "property list uint8 int32 vertex_index" << std::endl;
	ply << "end_header" << std::endl;
}

void WritePLYMeshVisitor::addVertex(const Eigen::Vector3f& pos, const Eigen::Vector3f& color)
{
	ply.write(reinterpret_cast<const char*>(pos.data()), 3 * sizeof(float));
	auto gammaCorrectedColor = osr::gammaCorrect(color);
	ply.write(reinterpret_cast<const char*>(gammaCorrectedColor.data()), 3 * sizeof(unsigned char));
}

void osr::WritePLYMeshVisitor::addVertex(const Eigen::Vector3f& pos, const Eigen::Vector4f& color)
{
	ply.write(reinterpret_cast<const char*>(pos.data()), 3 * sizeof(float));
	auto gammaCorrectedColor = osr::gammaCorrect(colorDisplacementToRGBColor(color));
	ply.write(reinterpret_cast<const char*>(gammaCorrectedColor.data()), 3 * sizeof(unsigned char));
}

void WritePLYMeshVisitor::addFace(unsigned int count, const uint32_t* indices)
{
	ply.write(reinterpret_cast<const char*>(&count), 1);
	ply.write(reinterpret_cast<const char*>(indices), sizeof(uint32_t) * count);
}

void WritePLYMeshVisitor::end()
{
	ply.close();
}

//===================RGB VISITOR==========================//

osr::WriteRGBPLYMeshVisitor::WriteRGBPLYMeshVisitor(const std::string& path)
	: ply(path, std::ios::binary)
{

}

void osr::WriteRGBPLYMeshVisitor::addVertex(const Eigen::Vector3f& pos, const Eigen::Vector4f& color)
{
	ply.write(reinterpret_cast<const char*>(pos.data()), 3 * sizeof(float));
	Eigen::Vector3f c(color(0), color(1), color(2));
	auto gammaCorrectedColor = osr::gammaCorrect(c);
	ply.write(reinterpret_cast<const char*>(gammaCorrectedColor.data()), 3 * sizeof(unsigned char));
}

void WriteRGBPLYMeshVisitor::begin(unsigned int vertices, unsigned int faces)
{
	ply << "ply" << std::endl;
	ply << "format binary_little_endian 1.0" << std::endl;
	ply << "element vertex " << vertices << std::endl;
	ply << "property float32 x" << std::endl;
	ply << "property float32 y" << std::endl;
	ply << "property float32 z" << std::endl;
	ply << "property uchar red" << std::endl;
	ply << "property uchar green" << std::endl;
	ply << "property uchar blue" << std::endl;
	ply << "element face " << faces << std::endl;
	ply << "property list uint8 int32 vertex_index" << std::endl;
	ply << "end_header" << std::endl;
}

void WriteRGBPLYMeshVisitor::addVertex(const Eigen::Vector3f& pos, const Eigen::Vector3f& color)
{
	ply.write(reinterpret_cast<const char*>(pos.data()), 3 * sizeof(float));
	auto gammaCorrectedColor = osr::gammaCorrect(color);
	ply.write(reinterpret_cast<const char*>(gammaCorrectedColor.data()), 3 * sizeof(unsigned char));
}

void WriteRGBPLYMeshVisitor::addFace(unsigned int count, const uint32_t* indices)
{
	ply.write(reinterpret_cast<const char*>(&count), 1);
	ply.write(reinterpret_cast<const char*>(indices), sizeof(uint32_t) * count);
}

void WriteRGBPLYMeshVisitor::end()
{
	ply.close();
}

//=========================MEM VISITOR==============================//
FineToMemoryVisitor::FineToMemoryVisitor()
{

}

void FineToMemoryVisitor::begin(unsigned int vertices, unsigned int faces)
{
	positions.resize(3, vertices);
	colors.resize(4, vertices);
	indices.resize(3, faces);
	nextVertex = 0;
	nextFace = 0;
}

void FineToMemoryVisitor::addVertex(const Eigen::Vector3f& pos, const Eigen::Vector3f& color)
{
	positions.col(nextVertex) = Vector3f(pos.x(), pos.y(), pos.z());
	auto gammaCorrectedColor = osr::gammaCorrect(color);
	colors.col(nextVertex) = Vector4uc(gammaCorrectedColor.x(), gammaCorrectedColor.y(), gammaCorrectedColor.z(), 255.0f);
	nextVertex++;
}

// not using now
void FineToMemoryVisitor::addVertex(const Eigen::Vector3f& pos, const Eigen::Vector4f& color)
{
	positions.col(nextVertex) = Vector3f(pos.x(), pos.y(), pos.z());
	auto gammaCorrectedColor = osr::gammaCorrect(colorDisplacementToRGBColor(color));
	colors.col(nextVertex) = Vector4uc(gammaCorrectedColor.x(), gammaCorrectedColor.y(), gammaCorrectedColor.z(), 255.0f);
	nextVertex++;
}

void FineToMemoryVisitor::addFace(unsigned int count, const uint32_t* indices)
{
// #if _DEBUG
// 	if (count != 3)
// 		std::cerr << "Warning: The FineMeshToBufferVisitor can only handle triangles. You passed a face with " << count << " vertices." << std::endl;
// #endif
	for (int i = 0; i < 3; ++i)
		this->indices.coeffRef(i, nextFace) = indices[i];
	nextFace++;
}

void FineToMemoryVisitor::end()
{
// 	positionBuffer.uploadData(positions).bindToAttribute("position");
// 	colorBuffer.uploadData(colors).bindToAttribute("color");
// 	indexBuffer.uploadData(indices.size() * sizeof(unsigned int), indices.data());
}

unsigned int FineToMemoryVisitor::indexCount() const
{
	return indices.cols();
}

unsigned int osr::FineToMemoryVisitor::vertCount() const
{
	return positions.cols();
}
