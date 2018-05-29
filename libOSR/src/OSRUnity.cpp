#include "dllwrapper/OSRUnity.h"
#include "osr/common.h"

osr::Data* CreateOSRData()
{
	return new osr::Data();
}

osr::Scan* AddScan(osr::Data* osrData, Vector3* vertices, Color32* colors, unsigned int* faces, float* transform, int verCnt, int faceCnt)
{
		osr::Matrix3Xf V = osr::Matrix3Xf();
		V.resize(3, verCnt);
		memcpy_s(V.data(), 3 * verCnt * 4, vertices, 3 * verCnt * 4); // float=4

		osr::Matrix3Xf N = osr::Matrix3Xf();

		osr::Matrix3Xuc C = osr::Matrix3Xuc();
		//C.resize(3, verCnt);
		//memcpy_s(C.data(), 3 * verCnt * 2, colors, 3 * verCnt * 2); // uchar = char = 2

		osr::MatrixXu F = osr::MatrixXu();
		F.resize(3, faceCnt);
		memcpy_s(F.data(), 3 * faceCnt * 4, faces, 3 * faceCnt * 4); // uint = int = 4

		//osr::MatrixXu F(faces);
		Eigen::Matrix4f transform4f(transform);
		Eigen::Affine3f transformMatrix(transform4f);
		Eigen::Affine3f identityMatrix = Eigen::Affine3f::Identity();
		//osr::Scan* newscan = new osr::Scan(V, N, C, F, "lala", identityMatrix);
		osr::Scan* newscan = new osr::Scan(true);
		newscan->ScanUnity(V, N, C, F, "wu", transformMatrix);
		osrData->AddScan(newscan);

		return newscan;
}

void DestroyOSRData(osr::Data* osrData)
{
	delete osrData;
}

void Integrate(osr::Data* osrData, osr::Scan* scan)
{
	osrData->IntegrateScan(scan);

	// do something similar to saveFineToPly -> generateFineMesh
	osrData->extractedMesh.extractFineMemoryMesh(true);

	// now actually I have a v pointer, c pointer,  and a f pointer, also assign the amount of v and f
	
}

float* Register(osr::Data* osrData, osr::Scan* scan)
{
	osrData->RegisterScan(scan);

	// return the changed transformation
	return scan->transform().data();
}

Vector3* GetIntegratedVerts(osr::Data* osrData, unsigned int& count)
{
	// return float pointer os extractedMesh.verts
	return NULL;
}

Color32* * GetIntegratedColors(osr::Data* osrData, unsigned int& count)
{
	// return float pointer os extractedMesh.verts
	return NULL;
}

unsigned int * GetIntegratedIndices(osr::Data* osrData, unsigned int& count)
{
	// return float pointer os extractedMesh.verts
	return NULL;
}
