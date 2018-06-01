#pragma once

#include "osr/data.h"

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

extern "C" {
	__declspec(dllexport) osr::Data* CreateOSRData();

	__declspec(dllexport) osr::Scan* AddScan(osr::Data* osrData, Vector3* vertices, Color32* colors, unsigned int* faces, float* transform, int verCnt, int faceCnt);// return index of data.scans

	__declspec(dllexport) void DestroyOSRData(osr::Data* osrData);

	__declspec(dllexport) void Integrate(osr::Data* osrData, osr::Scan* scan);
	__declspec(dllexport) Vector3* GetIntegratedVerts(osr::Data* osrData, unsigned int& count);
	__declspec(dllexport) Color32* GetIntegratedColors(osr::Data* osrData, unsigned int& count);
	__declspec(dllexport) unsigned int * GetIntegratedIndices(osr::Data* osrData, unsigned int& count);

	__declspec(dllexport) float* Register(osr::Data* osrData, osr::Scan* scan);

// 	__declspec(dllexport) unsigned char* Delete(osr::Data* osrData);
// 
// 	__declspec(dllexport) unsigned int* FillHole(osr::Data* osrData);
// 
// 	__declspec(dllexport) float* Cleanup(osr::Data* osrData);
// 
// 	__declspec(dllexport) const char* Smooth(osr::Data* osrData);
};
