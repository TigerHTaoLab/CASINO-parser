#include <io.hpp>
#include <vector>
#include <fstream>

namespace casino_parser
{
    const Eigen::IOFormat CSVFormat = Eigen::IOFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", "\n");

    int IO::openDAT(std::string fileToOpen, std::string matFileToWrite, std::string rangeFileToWrite)
    {
        std::vector<std::string> matrixEntries;
        std::ifstream matrixDataFile(fileToOpen);
        std::string matrixRowString;
        std::string matrixEntry;

        int matrixRowNumber = 0;
        int trajectoryNumber = 0;
        int collisionNumber = 0;
        bool print = false;
        double zMax = -1.0, xMax = -1.0, yMax = -1.0;

        std::vector<double> entries;
        std::ofstream file(matFileToWrite.c_str());

        while (std::getline(matrixDataFile, matrixRowString))
        {
            if (matrixRowNumber == 0)
                std::getline(matrixDataFile, matrixRowString);

            if (matrixRowString[0] == 'X')
            {
                std::getline(matrixDataFile, matrixRowString);
                print = true;
                trajectoryNumber++;
                entries.clear();
            }
            else if (matrixRowString.substr(1, 5) == "-----")
            {
                auto matTrajectory = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(entries.data(), entries.size() / 4, 4);

                for (int k = 0; k < matTrajectory.rows(); k++)
                {

                    double zAbs, xAbs, yAbs;
                    zAbs = std::abs(matTrajectory(k, 2));
                    yAbs = std::abs(matTrajectory(k, 1));
                    xAbs = std::abs(matTrajectory(k, 0));

                    if (zAbs > zMax)
                        zMax = zAbs;
                    if (yAbs > yMax)
                        yMax = yAbs;
                    if (xAbs > xMax)
                        xMax = xAbs;
                }

                if (file.is_open())
                    file << matTrajectory.format(CSVFormat) << std::endl; //file << "m" << '\n' <<  colm(matrix) << '\n';

                printf("Parsing trajectory \t%07d\r", trajectoryNumber);
                print = false;
                collisionNumber = 0;
            }

            if (print)
            {
                std::stringstream matrixRowStringStream(matrixRowString);
                matrixEntries.clear();
                while (std::getline(matrixRowStringStream, matrixEntry, '\t'))
                {
                    matrixEntries.push_back(matrixEntry);
                }

                entries.push_back(stod(matrixEntries[0]));
                entries.push_back(stod(matrixEntries[1]));
                entries.push_back(stod(matrixEntries[2]));
                entries.push_back(stod(matrixEntries[6]));

                collisionNumber++;
            }
            matrixRowNumber++; //update the column numbers
        }

        printf("\nMax. Abs. X | Y | Z = %5.2f\t%5.2f\t%5.2f\n", xMax, yMax, zMax);
        Eigen::MatrixXd rangeMat;
        rangeMat.resize(3, 1);
        rangeMat(0, 0) = xMax;
        rangeMat(1, 0) = yMax;
        rangeMat(2, 0) = zMax;
        std::ofstream rangeFile(rangeFileToWrite.c_str());

        if (rangeFile.is_open())
            rangeFile << rangeMat.format(CSVFormat) << std::endl;
        rangeFile.close();
        file.close();

        return 1;
    };

    void IO::writeToCSV(std::string name, const Eigen::MatrixXd &matrix)
    {
        std::ofstream file(name.c_str());
        if (file.is_open())
        {
            file << matrix.format(CSVFormat);
        }
    }
    int IO::openMAT(std::string fileToOpen, int rows, int cols, Eigen::MatrixXd &mat)
    {
        std::vector<double> matrixEntries;
        matrixEntries.reserve(rows * cols);

        std::ifstream matrixDataFile(fileToOpen);

        std::string matrixRowString;

        std::string matrixEntry;

        //Timer timer;

        int matrixRowNumber = 0;
        //timer.start();

        while (std::getline(matrixDataFile, matrixRowString))
        {
            std::stringstream matrixRowStringStream(matrixRowString);
            int count = 0;
            while (std::getline(matrixRowStringStream, matrixEntry, ','))
            {

                double aa = 0;
                try
                {
                    aa = std::stod(matrixEntry);
                }
                catch (const std::out_of_range &oor)
                {
                    aa = 0.0;
                }

                matrixEntries.push_back(aa);
                count++;
            }
            matrixRowNumber++; //update the column numbers
        }
        //std::cout << timer.elapsedSeconds() << "\n";
        //timer.stop();

        mat = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);
        return 1;
    }

    int IO::openMAT(std::string fileToOpen, Eigen::MatrixXd &mat)
    {
        std::vector<double> matrixEntries;
        std::ifstream matrixDataFile(fileToOpen);
        std::string matrixRowString;
        std::string matrixEntry;
        int matrixRowNumber = 0;

        while (std::getline(matrixDataFile, matrixRowString))
        {
            std::stringstream matrixRowStringStream(matrixRowString);
            while (std::getline(matrixRowStringStream, matrixEntry, ','))
            {
                double aa = 0;
                try
                {
                    aa = std::stod(matrixEntry);
                }
                catch (const std::out_of_range &oor)
                {
                    //std::cerr << "Out of Range error: " << oor.what() << '\n';
                    aa = 0.0;
                }
                matrixEntries.push_back(aa);
            }
            matrixRowNumber++; //update the column numbers
        }

        mat = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);
        return 1;
    }

} // namespace name
