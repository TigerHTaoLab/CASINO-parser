
#ifndef INCLUDE_CASINO_HISTOGRAM_HPP_
#define INCLUDE_CASINO_HISTOGRAM_HPP_

#include <Eigen/Core>

namespace casino_parser
{
    template <typename T>
    struct HistogramZ
    {
        int nz;
        double zMin, zMax, zDiv;
        Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> rawMatrix;
        void resize(double zMin_, double zMax_, int nDiv);
        void add(int k, int col, T value);
        //void add(double z, int col, T value);
        void xyz2ijk(double z, int &k);
    };
    template <typename T>
    void registerTrajectory(const Eigen::MatrixXd &datMat, int rowBegin, int rowEnd, HistogramZ<T> &histogramMat, double threshold);

    template <typename T>
    void parseTrajectories(const Eigen::MatrixXd &datMat, const Eigen::MatrixXd &dimMat, HistogramZ<T> &histogramMat, double threshold);
    //(const Eigen::MatrixXd &datMat, int rowBegin, int rowEnd, HistogramZ<T> &histogramMat, double threshold);
}

#endif