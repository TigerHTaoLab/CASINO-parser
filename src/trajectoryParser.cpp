#include <histogram.hpp>

#define SIGN(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))
#define FRAC0(x) (x - floor(x))
#define FRAC1(x) (1 - x + floor(x))

#include <iostream>

namespace casino_parser
{

    template <typename T>
    void parseTrajectories(const Eigen::MatrixXd &datMat, const Eigen::MatrixXd &dimMat, HistogramZ<T> &histogramMat, double threshold)
    {
        double zMin = dimMat(0, 0);
        double zMax = dimMat(1, 0);
        int nz = dimMat(2, 0);

        histogramMat.resize(zMin, zMax, nz);

        //registerTrajectory(datMat, 0, 10, histogramMat, 0);

        int rowBegin = 0, rowEnd = -1;
        double eLast = datMat(0, 3);

        for (int row = 1; row < datMat.rows(); row++)
        {
            if (row == datMat.rows() - 1)
                eLast = -100;

            if (datMat(row, 3) > eLast)
            {
                rowBegin = rowEnd + 1;
                rowEnd = row - 1;
                if (row == datMat.rows() - 1)
                    rowEnd = row;
                registerTrajectory(datMat, rowBegin, rowEnd, histogramMat, threshold);
            }
            eLast = datMat(row, 3);
        }
    }

    template <typename T>
    void registerTrajectory(const Eigen::MatrixXd &datMat, int rowBegin, int rowEnd, HistogramZ<T> &histogramMat, double threshold)
    {
        auto &trajectoryMat = datMat.block(rowBegin, 0, rowEnd - rowBegin + 1, 4);
        //std::cout << trajectoryMat << std::endl;

        double incidentEnergy = trajectoryMat(0, 3);
        double minimumEnergy = incidentEnergy * threshold;

        for (int row = 0; row < trajectoryMat.rows(); row++)
        {
            double currentEnergy = trajectoryMat(row, 3);
            int rowK;
            histogramMat.xyz2ijk(trajectoryMat(row, 2), rowK);
            histogramMat.add(rowK, 2, currentEnergy);
            if (row > 0)
                if (currentEnergy >= minimumEnergy)
                {
                    double z, z_1, interval_z1_z, zMin, zMax;
                    int k, k_1, kMin, kMax;
                    z = trajectoryMat(row, 2);
                    z_1 = trajectoryMat(row - 1, 2);
                    histogramMat.xyz2ijk(z, k);
                    histogramMat.xyz2ijk(z_1, k_1);
                    interval_z1_z = std::abs(z - z_1);
                    double currentDissipation = trajectoryMat(row - 1, 3) - trajectoryMat(row, 3);

                    if (z < z_1)
                    {
                        zMin = z;
                        zMax = z_1;
                    }
                    else
                    {
                        zMax = z;
                        zMin = z_1;
                    }
                    if (k < k_1)
                    {
                        kMin = k;
                        kMax = k_1;
                    }
                    else
                    {
                        kMax = k;
                        kMin = k_1;
                    }

                    if (kMin == kMax)
                    {
                        double weight = 1.0;
                        int kk = kMin;
                        // z, c, e, de
                        histogramMat.add(kk, 1, 1.0);

                        histogramMat.add(kk, 3, currentDissipation * weight);
                    }
                    else if (abs(kMin - kMax) == 1)
                    {
                        double weight_begin = FRAC1(zMin) / interval_z1_z;
                        double weight_end = FRAC0(zMax) / interval_z1_z;
                        int kk = kMin;
                        histogramMat.add(kk, 1, 1.0);
                        histogramMat.add(kk, 3, currentDissipation * weight_begin);
                        kk = kMax;
                        histogramMat.add(kk, 1, 1.0);
                        histogramMat.add(kk, 3, currentDissipation * weight_end);
                    }
                    else
                    {
                        double weight_begin = FRAC1(zMin) / interval_z1_z;
                        double weight_end = FRAC0(zMax) / interval_z1_z;
                        double weight_mid = 1 / (interval_z1_z);

                        int kk = kMin;
                        histogramMat.add(kk, 1, 1.0);
                        //histogramMat.add(kk, 2, currentEnergy);
                        histogramMat.add(kk, 3, currentDissipation * weight_begin);
                        kk = kMax;
                        histogramMat.add(kk, 1, 1.0);
                        //histogramMat.add(kk, 2, currentEnergy);
                        histogramMat.add(kk, 3, currentDissipation * weight_end);

                        for (int kkk = kMin + 1; kkk <= kMax - 1; kkk++)
                        {
                            histogramMat.add(kkk, 1, 1.0);
                            histogramMat.add(kkk, 3, currentDissipation * weight_mid);
                        }
                    }
                }
        }
    };

    template void registerTrajectory(const Eigen::MatrixXd &datMat, int rowBegin, int rowEnd, HistogramZ<int> &histogramMat, double threshold);
    template void registerTrajectory(const Eigen::MatrixXd &datMat, int rowBegin, int rowEnd, HistogramZ<double> &histogramMat, double threshold);
    template void parseTrajectories(const Eigen::MatrixXd &datMat, const Eigen::MatrixXd &dimMat, HistogramZ<int> &histogramMat, double threshold);
    template void parseTrajectories(const Eigen::MatrixXd &datMat, const Eigen::MatrixXd &dimMat, HistogramZ<double> &histogramMat, double threshold);
}