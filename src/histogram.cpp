#include <histogram.hpp>
#include <iostream>
namespace casino_parser
{
    template <typename T>
    void HistogramZ<T>::resize(double zMin_, double zMax_, int nDiv)
    {
        zMin = zMin_;
        zMax = zMax_;
        zDiv = (zMax - zMin) / nDiv;
        nz = (zMax - zMin) / zDiv;

        rawMatrix.resize(nz, 4);
        rawMatrix.setZero();

        for (int k = 0; k < nz; k++)
        {
            rawMatrix(k, 0) = zMin + k * zDiv;
        }
    };

    template <typename T>
    void HistogramZ<T>::add(int k, int col, T value)
    {
        if (k >= 0 && k < nz)
        {
            rawMatrix(k, col) += value;
        }
    };

    // template <typename T>
    // void HistogramZ<T>::add(double z, int col, T value)
    // {

    //     int k;
    //     xyz2ijk(z, k);
    //     if (k >= 0 && k < nz)
    //     {
    //         rawMatrix(k, col) += value;
    //     }
    // };

    template <typename T>
    void HistogramZ<T>::xyz2ijk(double z, int &k)
    {
        k = floor((z - zMin) / zDiv);
    };

    template struct HistogramZ<int>;
    template struct HistogramZ<double>;
}
