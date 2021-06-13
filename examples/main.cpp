#include <io.hpp>
#include <histogram.hpp>
#include <iostream>

int main()
{

    std::string datFile = "/home/be/Projects/CASINO_Visualizer/data/200k/05/00.dat";
    std::string matFile_w = "./test.mat";
    std::string rangeFile_w = "./test.range";
    std::string histogramFile_w = "./test.csv";

    //casino_parser::IO::openDAT(datFile, matFile_w, rangeFile_w);

    Eigen::MatrixXd datMat;
    casino_parser::IO::openMAT(matFile_w, datMat);

    Eigen::MatrixXd dim;
    dim.resize(3, 1);
    dim(0, 0) = 0;
    dim(1, 0) = 800;
    dim(2, 0) = 800;

    casino_parser::HistogramZ<double> histogramZ;

    casino_parser::parseTrajectories<double>(datMat, dim, histogramZ, 0);
    casino_parser::IO::writeToCSV(histogramFile_w, histogramZ.rawMatrix);

    return 0;
}