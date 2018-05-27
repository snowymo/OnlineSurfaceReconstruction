#pragma once
#include "osr/common.h"
#include <Eigen/Core>
#include <iostream>
#include <igl/triangle_triangle_adjacency.h>
#include <queue>
#include <set>

typedef Eigen::Matrix<float, 3, Eigen::Dynamic> Matrix3Xf;
typedef Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> MatrixXu;

//void splitMesh(Eigen::MatrixXd V, Eigen::MatrixXi F, std::vector<Eigen::MatrixXd>& subVs, std::vector<Eigen::MatrixXi>& subFs, int bound = 64995);

void splitMesh(Eigen::MatrixXd V, Eigen::MatrixXi F, Eigen::MatrixXd C,
	std::vector<Eigen::MatrixXd>& subVs, std::vector<Eigen::MatrixXi>& subFs, std::vector<Eigen::MatrixXd>& subCs,
	int bound = 64995);

int findFirstZero(Eigen::VectorXi v);

void splitMeshMergeApart(const Eigen::MatrixXd & V, const Eigen::MatrixXi & F, const Eigen::MatrixXd & C,
	std::vector<Eigen::MatrixXd>& subVs, std::vector<Eigen::MatrixXi>& subFs, std::vector<Eigen::MatrixXd>& subCs,
	int bound = 64995);

