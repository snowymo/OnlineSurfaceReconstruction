/*
	This file is part of the implementation for the technical paper

		Field-Aligned Online Surface Reconstruction
		Nico Schertler, Marco Tarini, Wenzel Jakob, Misha Kazhdan, Stefan Gumhold, Daniele Panozzo
		ACM TOG 36, 4, July 2017 (Proceedings of SIGGRAPH 2017)

	Use of this source code is granted via a BSD-style license, which can be found
	in License.txt in the repository root.

	@author Nico Schertler
*/

#include <iostream>

#include "dllwrapper/OSRUnity.h"
#include "osr/common.h"
#include "osr/Scan.h"
#include "osr/meshio.h"
#include <nanogui/nanogui.h>

#include "osr/gui/Viewer.h"
#include "osr/BatchSession.h"
#include "osr/Colors.h"
#define DEBUG_VISUALIZATION


static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg)
{
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	// The AMD variant of this extension provides a less detailed classification of the error,
	// which is why some arguments might be "Unknown".
	switch (source) {
	case GL_DEBUG_CATEGORY_API_ERROR_AMD:
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_CATEGORY_APPLICATION_AMD:
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (id == 131185 || id == 131218 || id == 131154) //buffer usage info, shader recompilation, pixel path
		return;

	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}

void runGUI()
{
	nanogui::init();

	{
		nanogui::ref<osr::gui::Viewer> viewer = new osr::gui::Viewer();
		viewer->setVisible(true);

#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugCallback, nullptr);
#endif

		try
		{
			nanogui::mainloop();
		}
		catch (std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
		}

	}

	nanogui::shutdown();
}

void runBatch(int argc, char *argv[])
{
	osr::BatchSession batch;
	batch.parseCommandLineArguments(argc, argv);
	batch.run();
}

void testDLL() {
	std::chrono::high_resolution_clock::time_point t1, t2;
	std::chrono::duration<double> time_span;


	// create data instance
	osr::Data* osrData = CreateOSRData();
	osr::Matrix3Xf V, N, myV, myN;
	osr::Matrix3Xus C, myC;
	osr::MatrixXu F, myF;

	// add scan
	osr::Scan* scan, *myScan;
// 	osr::load_ply("D:\\Scans\\currentScan.ply", F, V, N, C, false);
// 	scan = new osr::Scan(V, N, C, F, "original");
// 	scan->initialize();

	osr::load_ply_rgb("D:\\Scans\\currentScan.ply", myF, myV, myN, myC, false);
	// turn rgb color to lab color
	osr::Matrix3Xus myC2 = osr::Matrix3Xus();
	myC2.resize(3, myC.cols());
	osr::Matrix4Xuc color4split;
	color4split.resize(4, myC.cols());
	for (int i = 0; i < myC.cols(); i++) {
		color4split(0, i) = (unsigned char)myC(0, i);
		color4split(1, i) = (unsigned char)myC(1, i);
		color4split(2, i) = (unsigned char)myC(2, i);
		color4split(3, i) = 1;

		myC2(0, i) = (unsigned short)(myC(0, i)) * 255;
		myC2(1, i) = (unsigned short)(myC(1, i)) * 255;
		myC2(2, i) = (unsigned short)(myC(2, i)) * 255;
		myC2.col(i) = osr::RGBToLab(myC2.col(i));
	}

	float* identityTransform = new float[16]{ 0 };
	identityTransform[0] = identityTransform[5] = identityTransform[10] = identityTransform[15] = 1;
	//myScan = new osr::Scan(myV, myN, myC, myF, "lab");

	myScan = new osr::Scan(myV, myN, myC2, myF, "rgb");
	myScan->initialize();

	// shrink the size of the extracted mesh
	osrData->meshSettings.setScale(5.0);
	bool doIntegrate = true;
	if (doIntegrate) {
		t1 = std::chrono::high_resolution_clock::now();
		osrData->IntegrateScan(myScan);
		t2 = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

		std::cout << "IntegrateScan took me " << time_span.count() << " seconds.";
		// save to file or save to memory
		// the memory should be in the format that splitmesh support so we can split that into submeshes
		//osrData->extractedMesh.saveFineToPLY("testdll.ply");

		t1 = std::chrono::high_resolution_clock::now();
		osrData->extractedMesh.extractFineMemoryMesh(true);
		t2 = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		std::cout << "extractFineMemoryMesh took me " << time_span.count() << " seconds.";
		// saved in fVisitor
		std::cout << "faces:" << osrData->extractedMesh.fvisitor.indexCount() << "\n";
		std::cout << "verts:" << osrData->extractedMesh.fvisitor.vertCount() << "\n";
	}
	

	t1 = std::chrono::high_resolution_clock::now();
	std::vector<Eigen::Matrix<float, 3, Eigen::Dynamic>> subVs;
	std::vector<Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>> subFs;
	std::vector<Eigen::Matrix<unsigned char, 4, Eigen::Dynamic>> subCs;

	

// 	splitFineMemMesh(osrData->extractedMesh.fvisitor.positions.transpose(), osrData->extractedMesh.fvisitor.indices.transpose(), osrData->extractedMesh.fvisitor.colors.transpose(),
// 		subVs, subFs, subCs);
	osr::MatrixX4uc myTransC = osrData->extractedMesh.fvisitor.colors.transpose();
	osr::MatrixX3f myTransV = osrData->extractedMesh.fvisitor.positions.transpose();
	osr::MatrixXu myTransF = osrData->extractedMesh.fvisitor.indices.transpose();
	
	splitFineMemMesh(myTransV, myTransF, myTransC,
		subVs, subFs, subCs);
	//osrData->extractedMesh.splitFineMemMesh();
	t2 = std::chrono::high_resolution_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "splitFineMemMesh took me " << time_span.count() << " seconds to split into " << subVs.size() << " pieces\n";

	
}

int main(int argc, char *argv[])
{
// 	std::streambuf *psbuf;
// 	std::ofstream filestr;
// 	filestr.open("test.txt");
// 	
// 	psbuf = filestr.rdbuf();        // get file's streambuf
// 	std::cout.rdbuf(psbuf);         // assign streambuf to cout
// 
// 	std::cout << "This is written to the file";
// 
// 
// 	filestr.close();

	// use here to test dll, gui cannot be loaded in release mode and debug is super slow
	testDLL();
	return 0;
	try
	{
#if _WIN32
		std::cout.imbue(std::locale("en-US"));
#else	
		std::cout.imbue(std::locale("en_US.utf8"));
#endif
	}
	catch (...)
	{
		std::cerr << "Warning: Could not set english locale." << std::endl;
	}

	Eigen::initParallel();
	Eigen::setNbThreads(1);
	
	if (argc == 1) //no arguments
		runGUI();
	else
		runBatch(argc, argv);

	return 0;
}