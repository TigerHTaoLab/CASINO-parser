#include <io.hpp>
#include <histogram.hpp>
#include <vector>
#include <iostream>
bool ProgramOptionExists(int argc, char **argv, const std::string &option)
{
    return std::find(argv, argv + argc, option) != argv + argc;
}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> GetProgramOptionAsString(
    int argc,
    char **argv,
    const std::string &option,
    char delim,
    const std::string &default_value = "")
{
    char **itr = std::find(argv, argv + argc, option);
    std::vector<std::string> optStrVec;
    if (itr != argv + argc && ++itr != argv + argc)
    {

        split(std::string(*itr), delim, optStrVec);
    }

    return optStrVec;
}

int main(int argc, char *argv[])
{

    std::vector<std::string> cmdArgs(argv, argv + argc);

    if (argc < 3)
    {
        std::cerr << "Usage: ./dat2hist path/to/file/xx.dat  path/to/directory/output" << std::endl;
        return 1;
    }
    if (ProgramOptionExists(argc, argv, "-h") || ProgramOptionExists(argc, argv, "--help"))
    {
        std::cout << "-d2m       : convert from xx.dat to path/to/directory/mat.txt\n";
        std::cout << "-m2h a_b_c : convert from path/to/directory/mat.txt path/to/directory/histogram-z.csv\n";
        std::cout << "             where x = zMin, y = zMax, c = number of bins\n";
        return 1;
    }

    std::string datFile = argv[1];
    std::string outputDir = argv[2];
    std::string matFile_w = outputDir + "/mat.txt";
    std::string rangeFile_w = outputDir + "/range.txt";
    std::string histogramFile_w = outputDir + "/histogram-z.csv";

    if (ProgramOptionExists(argc, argv, "-d2m"))
    {
        std::cout << "Reading from " << datFile << "...\n";
        std::cout << "Writing to " << matFile_w << "...\n";
        casino_parser::IO::openDAT(datFile, matFile_w, rangeFile_w);
        std::cout << "Done!\n";
    }

    if (ProgramOptionExists(argc, argv, "-m2h"))
    {

        auto optStrVec = GetProgramOptionAsString(argc, argv, "-m2h", '_');

        Eigen::MatrixXd dim;
        dim.resize(3, 1);
        dim(0, 0) = stod(optStrVec[0]);
        dim(1, 0) = stod(optStrVec[1]);
        dim(2, 0) = stod(optStrVec[2]);

        std::cout << "Histogram range: zMin = " << dim(0, 0) << " , zMax= " << dim(0, 0) << " , bin #s = " << dim(2, 0) << "\n";

        std::cout << "Reading from " << matFile_w << "...\n";
        Eigen::MatrixXd datMat;
        casino_parser::IO::openMAT(matFile_w, datMat);
        std::cout << "Done!\n";

        casino_parser::HistogramZ<double> histogramZ;
        std::cout << "Calculating histogram for all trajectories...\n";
        casino_parser::parseTrajectories<double>(datMat, dim, histogramZ, 0);
        std::cout << "Done!\n";
        std::cout << "Writing to " << histogramFile_w << "...\n";
        casino_parser::IO::writeToCSV(histogramFile_w, histogramZ.rawMatrix);
        std::cout << "Done!\n";
    }

    return 0;
}
