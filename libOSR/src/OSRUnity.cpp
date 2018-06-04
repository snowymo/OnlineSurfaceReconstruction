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
	d->meshSettings.setScale(5.0);

// 	std::cout << "af CreateOSRData\n";
// 	filestr.close();
	return d;
}

osr::Scan* AddScan(osr::Data* osrData, Vector3* vertices, LABColor* colors, unsigned int* faces, float* transform, int verCnt, int faceCnt)
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
// 	for (int i = 0; i < verCnt; i++) {
// 		C(0, i) = (unsigned short)(colors[i].r) * 255;
// 		C(1, i) = (unsigned short)(colors[i].g) * 255;
// 		C(2, i) = (unsigned short)(colors[i].b) * 255;
// 		C.col(i) = osr::RGBToLab(C.col(i));
// 	}
	memcpy_s(C.data(), 3 * verCnt * 2, colors, 3 * verCnt * 2); // ushort=2
	int i = 0;
	std::cout << "C.cols(" << i << ")" << C.col(i) << " by " <<colors[i].r << " " << colors[i].g << " " << colors[i].b << "\n";
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

	osr::MatrixX4uc myTransC = osrData->extractedMesh.fvisitor.colors.transpose();
	osr::MatrixX3f myTransV = osrData->extractedMesh.fvisitor.positions.transpose();
	osr::MatrixXu myTransF = osrData->extractedMesh.fvisitor.indices.transpose();

	splitFineMemMesh(myTransV, myTransF, myTransC,
		osrData->extractedMesh.extractedSplittedVerts, 
		osrData->extractedMesh.extractedSplittedFaces, 
		osrData->extractedMesh.extractedSplittedColors, osrData->extractedMesh.splitBound);

	//osrData->extractedMesh.saveFineToPLY("D:\\Scans\\integrated.ply", true);
	// now actually I have a v pointer, c pointer,  and a f pointer, also assign the amount of v and f
	// it is still possible to have more than 65k v or f after integration, so let's use splitmesh func to generate a list of v/c/f pairs, and retrieve them by index
	//osrData->extractedMesh.splitFineMemMesh();	// now everything is saved in extractedSplittedVerts, extractedSplittedColors, extractedSplittedFaces;
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
	std::cout << "result:\t";
	for (int i = 0; i < 16; i++)
		std::cout << d[i] << "\t";
	filestr.close();
	return d;
}

Vector3* GetIntegratedVerts(osr::Data* osrData, unsigned int& count)
{
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.fvisitor.positions.cols();
	float* ret = (((osrData->extractedMesh).fvisitor.positions).data());

	return (Vector3*)ret;
}

Vector3* GetIntegratedVertsByIdx(osr::Data* osrData, int index, unsigned int& count)
{
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.extractedSplittedVerts[index].cols();
	float* ret = (((osrData->extractedMesh).extractedSplittedVerts[index]).data());

	return (Vector3*)ret;
}

Color32* GetIntegratedColors(osr::Data* osrData, unsigned int& count)
{
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.fvisitor.colors.cols();
	unsigned char* ret = (((osrData->extractedMesh).fvisitor.colors).data());

	return (Color32*)ret;
}

Color32* GetIntegratedColorsByIdx(osr::Data* osrData, int index, unsigned int& count)
{
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.extractedSplittedColors[index].cols();
	unsigned char* ret = (((osrData->extractedMesh).extractedSplittedColors[index]).data());

	return (Color32*)ret;
}

unsigned int * GetIntegratedIndices(osr::Data* osrData, unsigned int& count)
{
	
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.fvisitor.indices.cols();
	return (((osrData->extractedMesh).fvisitor.indices).data());
}

unsigned int * GetIntegratedIndicesByIdx(osr::Data* osrData, int index, unsigned int& count)
{
	// return float pointer of extractedMesh.verts
	if (osrData == NULL)
		return NULL;

	count = (unsigned int)osrData->extractedMesh.extractedSplittedFaces[index].cols();
	return (((osrData->extractedMesh).extractedSplittedFaces[index]).data());
}

int GetIntegrateAmount(osr::Data* osrData)
{
	return osrData->extractedMesh.extractedSplittedColors.size();
}

osr::Scan* AddOldScan(osr::Data* osrData, Vector3* vertices, Color32* colors, unsigned int* faces, float* transform, int verCnt, int faceCnt)
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
	//memcpy_s(C.data(), 3 * verCnt * 2, colors, 3 * verCnt * 2); // ushort=2
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

	//std::cout << "AddScan\tinitialize()\n";
	//filestr.close();
	return newscan;
}

// std::vector<Eigen::Matrix<float, Eigen::Dynamic, 3>> subVs;
// std::vector<Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>> subFs;
// std::vector<Eigen::Matrix<unsigned char, 4, Eigen::Dynamic>> subCs;

void splitFineMemMesh(const Eigen::Matrix<float, Eigen::Dynamic, 3> & transV, 
	const Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> & transF, 
	const Eigen::Matrix<unsigned char, Eigen::Dynamic, 4> & transC,
	std::vector<Eigen::Matrix<float, 3, Eigen::Dynamic>>& subVs,
	std::vector<Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>>& subFs, 
	std::vector<Eigen::Matrix<unsigned char, 4, Eigen::Dynamic>>& subCs, 
	int bound/*=64995*/)
{

	// clear up the splitted containers at the beginning
	subVs.clear();
	subCs.clear();
	subFs.clear();

	// triangle triangle adjacency
	Eigen::MatrixXi TT;
	//MatrixX3f transV = fvisitor.positions.transpose();
	igl::triangle_triangle_adjacency(transF, TT);

	// face discover to record if the face is already into the queue
	//int *FD = new int[F.cols()]{ 0 };
	Eigen::VectorXi FD = Eigen::VectorXi::Zero(transF.rows());

	// face queue
	std::queue<int> FQ;

	bool isOverSize = false;

	// set of face and vertex for current submesh
	std::set<int> FS;
	std::set<int> VS;

	// if there is still face not being discovered.
	while (!FD.isOnes()) {
		// find the first non-one
		int curIdx = findFirstZero(FD);
		if (curIdx < 0) break;

		// visit it
		FQ.push(curIdx);

		while (!FQ.empty()) {
			int curFace = FQ.front();
			FD(curFace) = 1;
			FQ.pop();
			//std::cout << "\ndealing " << curFace << " pushing neighbours ";

			FS.insert(curFace);
			for (int i = 0; i < transF.cols(); i++)
				VS.insert(transF(curFace, i));

			// get three adjacencies and push into the queue aka visit
			for (int i = 0; i < transF.cols(); i++) {
				int neighbour = TT(curFace, i);

				// check if already discovered
				if ((neighbour != -1) && (FD(neighbour) == 0)) {
					FQ.push(neighbour);
					FD(neighbour) = 2;
					//std::cout << neighbour << "\t";
				}
				//std::cout << "\n";
			}

			// check the size of vertices and faces
			if (FS.size() > bound || VS.size() > bound) {
				isOverSize = true;
				break;
			}
		}

		if (isOverSize) {
			splitHelper(FS, VS, transV, transF, transC, subVs, subFs, subCs);
			isOverSize = false;
		}
	}
	if (FS.size() > 0) {
		splitHelper(FS, VS, transV, transF, transC, subVs, subFs, subCs);
	}
}

int findFirstZero(Eigen::VectorXi v)
{
	for (int i = 0; i < v.rows(); i++)
		if (v[i] == 0)
			return i;
	return -1;
}

void splitHelper(std::set<int> &FS, std::set<int> &VS,
	const Eigen::Matrix<float, Eigen::Dynamic, 3> & transV,
	const Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> & transF,
	const Eigen::Matrix<unsigned char, Eigen::Dynamic, 4> & transC,
	std::vector<Eigen::Matrix<float, 3, Eigen::Dynamic>>& subVs,
	std::vector<Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>>& subFs,
	std::vector<Eigen::Matrix<unsigned char, 4, Eigen::Dynamic>>& subCs) 
{
	// deal with current submesh, turn set of faces to MatrixXd
	Eigen::Matrix<float, Eigen::Dynamic, 3> transsubV;
	Eigen::Matrix<unsigned char, Eigen::Dynamic, 4> transsubC;
	Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> transsubF(FS.size(), transF.cols());
	Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> transresF(FS.size(), transF.cols());

	int i = 0;
	for (std::set<int>::iterator it = FS.begin(); it != FS.end(); ++it, i++) {
		transsubF.row(i) = transF.row(*it);
	}
	Eigen::VectorXi UJ;

	igl::remove_unreferenced(transV, transsubF, transsubV, transresF, UJ);
	igl::remove_unreferenced(transC, transsubF, transsubC, transresF, UJ);

	subVs.push_back(transsubV.transpose());
	subCs.push_back(transsubC.transpose());
	subFs.push_back(transresF.transpose());

	FS.clear();
	VS.clear();
}

void SetSplitBound(osr::Data* osrData, int bound)
{
	osrData->extractedMesh.splitBound = bound;
}
