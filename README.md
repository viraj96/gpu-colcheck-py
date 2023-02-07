# GPu-Voxel based collision checking in Python

gpu-colcheck-py is a python module that provides a collision checking functionality that is built over [gpu-voxels](https://www.gpu-voxels.org/) library. This module was built using [pybind11](https://github.com/pybind/pybind11) Cmake-based build system. To install and get it work you will need to get first install gpu-voxels on your machine and then follow the installation instructions below to get it working. 

## Prerequisites

* A compiler with C++11 support
* Pip 10+ or CMake >= 3.4 (or 3.14+ on Windows, which was the first version to support VS 2019)
* Ninja or Pip 10+
* [gpu-voxels](https://github.com/fzi-forschungszentrum-informatik/gpu-voxels) and all its requirements


## Installation

```python
git clone --recursive https://github.com/pybind/cmake_example.git
pip install ./cmake_example
```

## Notes

* Once gpu-voxels is built run the following commands,

```sh
export LD_LIBRARY_PATH=<path-to-gpu-voxels-export-dir>/lib:$LD_LIBRARY_PATH
export GPU_VOXELS_MODEL_PATH=<path-to-gpu-voxels-models-dir>
export GPU_VOXELS_PREFIX_PATH=<path-to-gpu-voxels-export-dir>

```

* You will need to generate the voxel files for the robots that you want to load using the instructions given in the [gpu-voxels](https://github.com/fzi-forschungszentrum-informatik/gpu-voxels). 
* Make sure that the urdf files that you use to load the robots are placed on the positive quadrant of the xyz-axes as gpu-voxels does not support negative quadrants and hence the collision checker might not work as expected. 
* To run the first copy `rs007l/` to the `GPU_VOXELS_MODEL_PATH` and then run test script,
```python
python test_basic.py
```
* Tested on Ubuntu 20.04, Cuda 12 and Python 3.8
 
## References

If you find this repository useful then please cite the original [work](https://github.com/fzi-forschungszentrum-informatik/gpu-voxels).
