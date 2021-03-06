# A Parser for CASINO V2

This is a parser for the raw data output from CASINO v2.

## Compile

```shell
mkdir bin -p
mkdir build -p
cd build
cmake ..
make -j
```

## Usage

`./casino_parser path/to/file/xx.dat path/to/directory/output -d2m -m2h 0_800_800`

* `path/to/file/xx.dat` is the input file (raw data output from CASINO)
* `path/to/directory/output` stores output files `mat.txt`, `range.txt` and `histogram-z.csv`.
* `-d2m`: flag to perform conversion from `xx.dat` to `mat.txt` and `range.txt`
* `-m2h a_b_c`: flag to perform conversion from `mat.txt` to `histogram-z.csv` where `a_b_c` sets the z-min, z-max and number of z-bins in the histogram.

## Output format

### mat.txt

Resulting matrix from CASINO dat file.

Columns = [x | y | z | electron energy]

### range.txt

Maximum xyz coordinates that a electron trajectory has ever reached.

Rows = [max(abs(x)); max(abs(y)); max(abs(z)); ]

### histogram-z.csv

A histogram in which the statistics of all electron trajectories are partitioned w.r.t the z coordinate.

Columns = [beginning coordinate of a z-bin | hits of dissipating events | energy | dissipation]

## Result

Down below shows the result of CASINO simulations with a beam energy of 5eV. 200000 and 1000000 electron trajectories were generated in two independent simulations. The statistics of 1000k electrons was multiplied by a factor of 0.2 in order to compare with the result of 200k electrons. Our histogram code is consistent in the sense that it converges to a steady distribution as the total number of samples increases.

![](./python/result.svg)
