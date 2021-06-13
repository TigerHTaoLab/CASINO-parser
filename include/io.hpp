
#ifndef INCLUDE_CASINO_PARSER_HPP_
#define INCLUDE_CASINO_PARSER_HPP_

#include <Eigen/Core>
#include <string>

namespace casino_parser
{
    class IO
    {
    public:
        static int openMAT(std::string fileToOpen, Eigen::MatrixXd &mat);
        static int openMAT(std::string fileToOpen, int rows, int cols, Eigen::MatrixXd &mat);
        static int openDAT(std::string fileToOpen, std::string matFileToWrite, std::string rangeFileToWrite);
        static void writeToCSV(std::string name, const Eigen::MatrixXd &matrix);
    };

}

#endif