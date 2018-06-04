#pragma once

#include "osr/data.h"
#include "osr/common.h"
#include <igl/triangle_triangle_adjacency.h>
#include <igl/remove_unreferenced.h>
#include<vector>
#include<queue>
// class OSRObject {
// public:
// 	osr::Data data;
// 	HierarchyRenderer hierarchyRenderer;
// };

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Color32 {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct LABColor {
	unsigned short r;
	unsigned short g;
	unsigned short b;
};



int findFirstZero(Eigen::VectorXi v);

void splitHelper(std::set<int> &FS, std::set<int> &VS, 
	const Eigen::Matrix<float, Eigen::Dynamic, 3> & transV, 
	const Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> & transF, 
	const Eigen::Matrix<unsigned char, Eigen::Dynamic, 4> & transC,
	std::vector<Eigen::Matrix<float, 3, Eigen::Dynamic>>& subVs,
	std::vector<Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>>& subFs, 
	std::vector<Eigen::Matrix<unsigned char, 4, Eigen::Dynamic>>& subCs);

extern "C" {
	__declspec(dllexport) osr::Data* CreateOSRData();

	__declspec(dllexport) osr::Scan* AddScan(osr::Data* osrData, Vector3* vertices, LABColor* colors, unsigned int* faces, float* transform, int verCnt, int faceCnt);// return addr of scan
	__declspec(dllexport) osr::Scan* AddOldScan(osr::Data* osrData, Vector3* vertices, Color32* colors, unsigned int* faces, float* transform, int verCnt, int faceCnt);// return index of data.scans

	__declspec(dllexport) void DestroyOSRData(osr::Data* osrData);

	__declspec(dllexport) void Integrate(osr::Data* osrData, osr::Scan* scan);
	// 
	__declspec(dllexport) int GetIntegrateAmount(osr::Data* osrData);
	__declspec(dllexport) Vector3* GetIntegratedVertsByIdx(osr::Data* osrData, int index, unsigned int& count);
	__declspec(dllexport) Color32* GetIntegratedColorsByIdx(osr::Data* osrData, int index, unsigned int& count);
	__declspec(dllexport) unsigned int * GetIntegratedIndicesByIdx(osr::Data* osrData, int index, unsigned int& count);
	// discarded
	__declspec(dllexport) Vector3* GetIntegratedVerts(osr::Data* osrData, unsigned int& count);
	__declspec(dllexport) Color32* GetIntegratedColors(osr::Data* osrData, unsigned int& count);
	__declspec(dllexport) unsigned int * GetIntegratedIndices(osr::Data* osrData, unsigned int& count);

	__declspec(dllexport) float* Register(osr::Data* osrData, osr::Scan* scan);

	__declspec(dllexport) void SetSplitBound(osr::Data* osrData, int bound);
	__declspec(dllexport) void splitFineMemMesh(
		const Eigen::Matrix<float, Eigen::Dynamic, 3> & transV, 
		const Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> & transF, 
		const Eigen::Matrix<unsigned char, Eigen::Dynamic, 4> & transC,
		std::vector<Eigen::Matrix<float, 3, Eigen::Dynamic>>& subVs,
		std::vector<Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>>& subFs,
		std::vector<Eigen::Matrix<unsigned char, 4, Eigen::Dynamic>>& subCs,
		int bound=64995);

// 	__declspec(dllexport) unsigned char* Delete(osr::Data* osrData);
// 
// 	__declspec(dllexport) unsigned int* FillHole(osr::Data* osrData);
// 
// 	__declspec(dllexport) float* Cleanup(osr::Data* osrData);
// 
// 	__declspec(dllexport) const char* Smooth(osr::Data* osrData);
};
