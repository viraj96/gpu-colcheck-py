#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include <cstdlib>
#include <signal.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <gpu_voxels/GpuVoxels.h>
#include <gpu_voxels/helpers/MetaPointCloud.h>
#include <gpu_voxels/logging/logging_gpu_voxels.h>
#include <gpu_voxels/robot/urdf_robot/urdf_robot.h>

#include <kdl/frames_io.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

using namespace gpu_voxels;

class collisionCheck {
    public:
        collisionCheck(int x, int y, int z, double voxel_size, std::string path_to_first_robot, std::string path_to_second_robot);
        ~collisionCheck();

    bool check_collisions(std::map<std::string, float> first_robot_config, std::map<std::string, float> second_robot_config);

    private:    
        GpuVoxelsSharedPtr gvl;

};

collisionCheck::collisionCheck(int x, int y, int z, double voxel_size, std::string path_to_first_robot, std::string path_to_second_robot) {
    
    icl_core::logging::initialize();
    icl_core::logging::setLogLevel(icl_core::logging::LogLevel::eLL_MUTE);

    // Add a gpu-voxel instance and define the voxel grid size
    gvl = GpuVoxels::getInstance();
    gvl->initialize(x, y, z, voxel_size);

    // Add robots to two different maps so that we can query collision between different maps
    gvl->addMap(MT_BITVECTOR_VOXELMAP, "first_robot");
    gvl->addMap(MT_BITVECTOR_VOXELMAP, "second_robot");
    
    // Add robot urdfs to corresponding maps
    gvl->addRobot("body1", path_to_first_robot, true);
    gvl->addRobot("body2", path_to_second_robot, true);

}

collisionCheck::~collisionCheck() {

    // Not even required, as we use smart pointers.
    gvl.reset();

}

bool collisionCheck::check_collisions(std::map<std::string, float> first_robot_config, std::map<std::string, float> second_robot_config) {
    
    // Assign configuration values to the corresponding robots
    robot::JointValueMap myRobotJointValues_one(first_robot_config), myRobotJointValues_two(second_robot_config);

    gvl->setRobotConfiguration("body1", myRobotJointValues_one);
    gvl->setRobotConfiguration("body2", myRobotJointValues_two);

    // Insert the robots to their corresponding maps
    gvl->insertRobotIntoMap("body1", "first_robot", eBVM_OCCUPIED);
    gvl->insertRobotIntoMap("body2", "second_robot", eBVM_OCCUPIED);

    // Check for collision between the maps
    auto collision = gvl->getMap("first_robot")->as<voxelmap::BitVectorVoxelMap>()->collideWith(gvl->getMap("second_robot")->as<voxelmap::BitVectorVoxelMap>());

    gvl->clearMap("first_robot");
    gvl->clearMap("second_robot");

    if (collision != 0)
        return true;
    else
        return false;

}

namespace py = pybind11;

PYBIND11_MODULE(collision_checker, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: collision_checker

        .. autosummary::
           :toctree: _generate

           check_collisions
    )pbdoc";

    py::class_<collisionCheck>(m, "collisionCheck")
        .def(py::init<int, int, int, double, std::string, std::string>())
        .def("check_collisions", &collisionCheck::check_collisions);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
