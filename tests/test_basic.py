import random
import argparse
import collision_checker as cc

dof = 6
# Should have the same names as specified in robot urdfs
joints_names = ['joint1', 'joint2', 'joint3', 'joint4', 'joint5', 'joint6']
joints_range = [(-3.141592653589793, 3.141592653589793),
                (-2.356194490192345, 2.356194490192345),
                (0.0, 2.7401669256310974),
                (-3.490658503988659, 3.490658503988659),
                (-2.181661564992912, 2.181661564992912),
                (-6.283185307179586, 6.283185307179586)]

def generate_random_conf():
    config = dict()
    for joint in range(dof):
        config[joints_names[joint]] = random.uniform(joints_range[joint][0], joints_range[joint][1])
    return config

# robot urdf paths are relative to the GPU_VOXELS_MODEL_PATH
robot_one_urdf = "rs007l/rs007l_full_right.urdf"
robot_two_urdf = "rs007l/rs007l_full_left.urdf"

def test_main(iterations):
    assert cc.__version__ == "0.0.1"
    # Specify the voxel grid size
    voxels_x, voxels_y, voxels_z, size = 200, 200, 200, 0.01 # This means 200 voxels in each axes and each voxel is 0.01m i.e the total space mapped is 2x2x2m
    cc_obj = cc.collisionCheck(voxels_x, voxels_y, voxels_z, size, robot_one_urdf, robot_two_urdf)

    for _ in range(iterations):
        print("Collision? ", cc_obj.check_collisions(generate_random_conf(), generate_random_conf()))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--arm_sample_num', type=int, default=1000)
    args = parser.parse_args()
    test_main(args.arm_sample_num)
