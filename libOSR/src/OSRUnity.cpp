#include "dllwrapper/OSRUnity.h"
#include "osr/common.h"
#include "osr/Colors.h"
//#include "gui/ScanRenderer.h"

osr::Data* CreateOSRData()
{
// 	std::streambuf *psbuf;
// 	std::ofstream filestr;
// 	filestr.open("test.txt", std::ios::out | std::ios::app);
// 	psbuf = filestr.rdbuf();        // get file's streambuf
// 	std::cout.rdbuf(psbuf);
// 
// 	std::cout << "bf CreateOSRData\n";
	osr::Data *d = new osr::Data();


// 	std::cout << "af CreateOSRData\n";
// 	filestr.close();
	return d;
}

osr::Scan* AddScan(osr::Data* osrData, Vector3* vertices, Color32* colors, unsigned int* faces, float* transform, int verCnt, int faceCnt)
{
// 	std::streambuf *psbuf;
// 	std::ofstream filestr;
// 	filestr.open("test.txt");
// 	psbuf = filestr.rdbuf();        // get file's streambuf
// 	std::cout.rdbuf(psbuf);

	std::cout << "AddScan\tresize(3," << verCnt << ")\n";
	osr::Matrix3Xf V = osr::Matrix3Xf();
	
	V.resize(3, verCnt);
	memcpy_s(V.data(), 3 * verCnt * 4, vertices, 3 * verCnt * 4); // float=4

	osr::Matrix3Xf N = osr::Matrix3Xf();
	N.resize(3, 0);

// 		osr::Matrix3Xuc C = osr::Matrix3Xuc();
// 		C.resize(3, verCnt);
// 		for (int i = 0; i < verCnt; i++) {
// 			C(0, i) = colors[i].r;
// 			C(1, i) = colors[i].g;
// 			C(2, i) = colors[i].b;
// 		}

	osr::Matrix3Xus C = osr::Matrix3Xus();
	C.resize(3, verCnt);
	for (int i = 0; i < verCnt; i++) {
		C(0, i) = (unsigned short)(colors[i].r) * 255;
		C(1, i) = (unsigned short)(colors[i].g) * 255;
		C(2, i) = (unsigned short)(colors[i].b) * 255;
		C.col(i) = osr::RGBToLab(C.col(i));
	}
	int i = 0;
	std::cout << "C.cols(" << i << ")" << C.col(i) << " by " << colors[i].r << " " << colors[i].g << " " << colors[i].b << "\n";
	i = 100;
	std::cout << "C.cols(" << i << ")" << C.col(i) << " by " << colors[i].r << " " << colors[i].g << " " << colors[i].b << "\n";
	//memcpy_s(C.data(), 3 * verCnt * 2, colors, 3 * verCnt * 2); // uchar = char = 2

	osr::MatrixXu F = osr::MatrixXu();
	F.resize(3, faceCnt);
	memcpy_s(F.data(), 3 * faceCnt * 4, faces, 3 * faceCnt * 4); // uint = int = 4

	//osr::MatrixXu F(faces);
	Eigen::Matrix4f transform4f(transform);
	Eigen::Affine3f transformMatrix(transform4f);
	Eigen::Affine3f identityMatrix = Eigen::Affine3f::Identity();
	std::cout << "AddScan\tbefore new scan\n";
	osr::Scan* newscan = new osr::Scan(V, N, C, F, "lala", transformMatrix);
	newscan->initialize();
	std::cout << "AddScan\tafter new scan\n";
	//osr::Scan* newscan = new osr::Scan(true);
	//newscan->ScanUnity(V, N, C, F, "wu", transformMatrix);
	osrData->AddScan(newscan);
	std::cout << "AddScan\tafter addscan()\n";
	//newscan->renderer = std::make_shared<osr::gui::ScanRenderer>();
	
	std::cout << "AddScan\tinitialize()\n";
	//filestr.close();
	return newscan;
}

void DestroyOSRData(osr::Data* osrData)
{
	
	delete osrData;
}

void Integrate(osr::Data* osrData, osr::Scan* scan)
{
// 	std::streambuf *psbuf;
// 	std::ofstream filestr;
// 	filestr.open("test.txt", std::ios::out | std::ios::app);
// 	psbuf = filestr.rdbuf();        // get file's streambuf
// 	std::cout.rdbuf(psbuf);
	std::cout << "IntegrateScan\tbefore IntegrateScan(scan);\n";
	osrData->IntegrateScan(scan);

	// do something similar to saveFineToPly -> generateFineMesh
	osrData->extractedMesh.extractFineMemoryMesh(true);
	//osrData->extractedMesh.saveFineToPLY("D:\\Scans\\integrated.ply", true);
	// now actually I have a v pointer, c pointer,  and a f pointer, also assign the amount of v and f
	//filestr.close();
}

float* Register(osr::Data* osrData, osr::Scan* scan)
{
	std::streambuf *psbuf;
	std::ofstream filestr;
	filestr.open("test.txt");
	psbuf = filestr.rdbuf();        // get file's streambuf
	std::cout.rdbuf(psbuf);

	osrData->RegisterScan(scan);

	// return the changed transformation
	float* d = scan->transform().data();
	filestr.close();
	return d;
}

Vector3* GetIntegratedVerts(osr::Data* osrData, unsigned int& count)
{
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.extractedVerts.cols();
	float* ret = (((osrData->extractedMesh).extractedVerts).data());

	return (Vector3*)ret;
}

Color32* GetIntegratedColors(osr::Data* osrData, unsigned int& count)
{
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.extractedColors.cols();
	unsigned char* ret = (((osrData->extractedMesh).extractedColors).data());

	return (Color32*)ret;
}

unsigned int * GetIntegratedIndices(osr::Data* osrData, unsigned int& count)
{
	
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.extractedFaces.cols();
	return (((osrData->extractedMesh).extractedFaces).data());
}
