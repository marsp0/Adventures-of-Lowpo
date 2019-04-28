#include "catch.hpp"
#include <iostream>
#include <unistd.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Loader.hpp"

const float EPSILON = 0.0005;

TEST_CASE("Geometry parsing")
{
    std::string filename = "./test/files/loader_test_geometry_parser.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    std::unordered_map<std::string, std::shared_ptr<Geometry>> result = Loader::ParseGeometry(libraryGeometries);
    // parsed 3 objects
    REQUIRE(result.size() == 3);

    // Plane tests
    std::shared_ptr<Geometry> plane = result["Plane"];
    REQUIRE(plane->name == "Plane");
    REQUIRE(plane->indices.size() == 18);
    REQUIRE(plane->vertices.size() == 12);
    REQUIRE(plane->texCoords.size() == 12);

    // Plane tests
    std::shared_ptr<Geometry> cube = result["Cube.000"];
    REQUIRE(cube->name == "Cube.000");
    REQUIRE(cube->indices.size() == 108);
    REQUIRE(cube->vertices.size() == 24);
    REQUIRE(cube->texCoords.size() == 72);

    // Plane tests
    std::shared_ptr<Geometry> cube1 = result["Cube.001"];
    REQUIRE(cube1->name == "Cube.001");
    REQUIRE(cube1->indices.size() == 108);
    REQUIRE(cube1->vertices.size() == 24);
    REQUIRE(cube1->texCoords.size() == 72);
}

TEST_CASE("Controller parsing")
{
    std::string filename = "./test/files/controllers.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryControllers = collada->FirstChildElement("library_controllers");
    std::unordered_map<std::string, std::shared_ptr<Controller>> result = Loader::ParseControllers(libraryControllers);
    REQUIRE(result.size() == 1);

    std::shared_ptr<Controller> controller = result["Armature"];
    REQUIRE(controller->id == "Armature_Cube_001-skin");
    REQUIRE(controller->name == "Armature");
    REQUIRE(controller->indices.size() == 740 * 4);
    REQUIRE(controller->weights.size() == 740 * 4);
}

TEST_CASE("Animations parsing")
{
    std::string filename = "./test/files/controllers.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryAnimations = collada->FirstChildElement("library_animations");
    std::unordered_map<std::string, std::shared_ptr<AnimationNode>> result = Loader::ParseAnimations(libraryAnimations);

    std::vector<std::string> names{"Armature_Torso_pose_matrix",        "Armature_Chest_pose_matrix",
                                    "Armature_Neck_pose_matrix",        "Armature_Head_pose_matrix",
                                    "Armature_Head_end_pose_matrix",    "Armature_Upper_Arm_L_pose_matrix",
                                    "Armature_Lower_Arm_L_pose_matrix", "Armature_Hand_L_pose_matrix",
                                    "Armature_Hand_L_end_pose_matrix",  "Armature_Upper_Arm_R_pose_matrix",
                                    "Armature_Lower_Arm_R_pose_matrix", "Armature_Hand_R_pose_matrix",
                                    "Armature_Hand_R_end_pose_matrix",  "Armature_Upper_Leg_L_pose_matrix",
                                    "Armature_Lower_Leg_L_pose_matrix", "Armature_Foot_L_pose_matrix", "Armature_Foot_L_end_pose_matrix",
                                    "Armature_Upper_Leg_R_pose_matrix", "Armature_Lower_Leg_R_pose_matrix",
                                    "Armature_Foot_R_pose_matrix",      "Armature_Foot_R_end_pose_matrix"};

    REQUIRE(result.size() == 21);
    for (int i = 0; i < names.size(); i++)
    {
        std::shared_ptr<AnimationNode> animation = result[names[i]];
        REQUIRE(animation->id == names[i]);
        REQUIRE(animation->matrices.size() == 21);
        REQUIRE(animation->timeStamps.size() == 21);
    }
}

TEST_CASE("Visual Scene Static parsing")
{
    std::string filename = "./test/files/book_scene.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryAnimations = collada->FirstChildElement("library_visual_scenes");
    std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>> result = Loader::ParseVisualScenesStatic(libraryAnimations);
    REQUIRE(result.size() == 14);
    for (std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>>::iterator it = result.begin(); it != result.end(); it++)
    {
        REQUIRE(it->second->id.size() > 0);
        REQUIRE(it->second->name.size() > 0);
    }
}

TEST_CASE("Visual Scene Animated parsing")
{
    std::string filename = "./test/files/book_scene.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryAnimations = collada->FirstChildElement("library_visual_scenes");
    std::unordered_map<std::string, std::shared_ptr<InstanceController>> result = Loader::ParseVisualScenesAnimated(libraryAnimations);
    
    REQUIRE(result.size() == 1);
    
    std::shared_ptr<InstanceController> instance = result["Cube"];
    REQUIRE(instance->id == "Cube");
    REQUIRE(instance->name == "Cube");
    REQUIRE(instance->url == "Armature_Cube-skin");
}

TEST_CASE("Visual Scene Skeleton Parsing")
{
    std::string filename = "./test/files/controllers.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryVisualScenes = collada->FirstChildElement("library_visual_scenes");
    std::unordered_map<std::string, std::shared_ptr<SkeletonNode>> result = Loader::ParseVisualScenesSkeletons(libraryVisualScenes);

    REQUIRE(result.size() == 1);

    std::shared_ptr<SkeletonNode> skeleton = result["Armature"];
    REQUIRE( skeleton->id == "Armature" );
    REQUIRE( skeleton->name == "Armature" );
    REQUIRE( skeleton->sid == "" );
    std::string matrixString = "0.09427643 0 0 0 0 0.09427643 4.12095e-9 0 0 -4.12095e-9 0.09427643 0 0 0 0 1";
    REQUIRE( glm::determinant(skeleton->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( skeleton->children.size() == 1 );

    std::shared_ptr<SkeletonNode> torso = skeleton->children[0];
    REQUIRE( torso->id == "Armature_Torso" );
    REQUIRE( torso->name == "Torso" );
    REQUIRE( torso->sid == "Torso" );
    matrixString = "1 0 0 0 0 -0.06466545 -0.997907 0 0 0.997907 -0.06466532 3.210999 0 0 0 1";
    REQUIRE( glm::determinant(torso->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( torso->children.size() == 3 );

    std::shared_ptr<SkeletonNode> chest = torso->children[0];
    REQUIRE( chest->id == "Armature_Chest" );
    REQUIRE( chest->name == "Chest" );
    REQUIRE( chest->sid == "Chest" );
    matrixString = "1 7.10543e-15 4.76837e-7 0 -7.13595e-8 0.9887388 0.1496516 1.482405 -4.71468e-7 -0.1496516 0.9887387 -2.98023e-8 0 0 0 1";
    REQUIRE( glm::determinant(chest->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( chest->children.size() == 3 );

    std::shared_ptr<SkeletonNode> neck = chest->children[0];
    REQUIRE( neck->id == "Armature_Neck" );
    REQUIRE( neck->name == "Neck" );
    REQUIRE( neck->sid == "Neck" );
    matrixString = "-1 -1.44018e-7 6.05564e-7 0 4.5605e-8 0.953299 0.3020283 1.295159 -6.20781e-7 0.3020283 -0.953299 -5.96046e-8 0 0 0 1";
    REQUIRE( glm::determinant(neck->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( neck->children.size() == 1 );

    std::shared_ptr<SkeletonNode> head = neck->children[0];
    REQUIRE( head->id == "Armature_Head" );
    REQUIRE( head->name == "Head" );
    REQUIRE( head->sid == "Head" );
    matrixString = "1 2.99489e-8 -2.48448e-7 1.42109e-14 -8.79698e-8 0.9715177 -0.2369666 0.3023286 2.34275e-7 0.2369666 0.9715178 0 0 0 0 1";
    REQUIRE( glm::determinant(head->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( head->children.size() == 1 );

    std::shared_ptr<SkeletonNode> headEnd = head->children[0];
    REQUIRE( headEnd->id == "Armature_Head_end" );
    REQUIRE( headEnd->name == "Head_end" );
    REQUIRE( headEnd->sid == "Head_end" );
    matrixString = "1 6.21725e-15 8.84744e-14 -1.77636e-15 -6.43929e-15 0.9999999 1.49012e-8 1.809841 -1.09225e-13 -1.86265e-8 0.9999999 -1.49012e-8 0 0 0 1";
    REQUIRE( glm::determinant(headEnd->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( headEnd->children.size() == 0 );

    std::shared_ptr<SkeletonNode> upperArmL = chest->children[1];
    REQUIRE( upperArmL->id == "Armature_Upper_Arm_L" );
    REQUIRE( upperArmL->name == "Upper_Arm.L" );
    REQUIRE( upperArmL->sid == "Upper_Arm_L" );
    matrixString = "-0.8112754 0.5809819 0.0655095 0.9250668 -0.5825275 -0.7936532 -0.1754318 0.7715864 -0.04993066 -0.1804848 0.9823095 -0.1557154 0 0 0 1";
    REQUIRE( glm::determinant(upperArmL->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( upperArmL->children.size() == 1 );

    std::shared_ptr<SkeletonNode> lowerArmL = upperArmL->children[0];
    REQUIRE( lowerArmL->id == "Armature_Lower_Arm_L" );
    REQUIRE( lowerArmL->name == "Lower_Arm.L" );
    REQUIRE( lowerArmL->sid == "Lower_Arm_L" );
    matrixString = "-0.01571715 0.04884753 -0.9986824 4.76837e-7 -0.4106453 0.9103686 0.05099085 1.480745 0.9116598 0.4109057 0.005750604 5.96046e-8 0 0 0 1";
    REQUIRE( glm::determinant(lowerArmL->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( lowerArmL->children.size() == 1 );

    std::shared_ptr<SkeletonNode> handL = lowerArmL->children[0];
    REQUIRE( handL->id == "Armature_Hand_L" );
    REQUIRE( handL->name == "Hand.L" );
    REQUIRE( handL->sid == "Hand_L" );
    matrixString = "0.992012 -0.1146639 -0.05257458 -5.36442e-7 0.1225457 0.9748493 0.1861493 1.083283 0.02990764 -0.1911052 0.9811139 -1.19209e-6 0 0 0 1";
    REQUIRE( glm::determinant(handL->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( handL->children.size() == 1 );

    std::shared_ptr<SkeletonNode> handLEnd = handL->children[0];
    REQUIRE( handLEnd->id == "Armature_Hand_L_end" );
    REQUIRE( handLEnd->name == "Hand.L_end" );
    REQUIRE( handLEnd->sid == "Hand_L_end" );
    matrixString = "0.9999999 -7.45058e-8 3.50177e-7 -2.98023e-8 5.96046e-8 0.9999999 -2.98023e-7 0.7135494 -3.7998e-7 3.8743e-7 0.9999999 0 0 0 0 1";
    REQUIRE( glm::determinant(handLEnd->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( handLEnd->children.size() == 0);

    std::shared_ptr<SkeletonNode> upperArmR = chest->children[2];
    REQUIRE( upperArmR->id == "Armature_Upper_Arm_R" );
    REQUIRE( upperArmR->name == "Upper_Arm.R" );
    REQUIRE( upperArmR->sid == "Upper_Arm_R" );
    matrixString = "-0.8112754 -0.5809814 0.06550947 -0.9250667 0.5468315 -0.7936532 -0.2666267 0.7715864 0.2068967 -0.1804854 0.9615708 -0.1557164 0 0 0 1";
    REQUIRE( glm::determinant(upperArmR->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( upperArmR->children.size() == 1);

    std::shared_ptr<SkeletonNode> lowerArmR = upperArmR->children[0];
    REQUIRE( lowerArmR->id == "Armature_Lower_Arm_R" );
    REQUIRE( lowerArmR->name == "Lower_Arm.R" );
    REQUIRE( lowerArmR->sid == "Lower_Arm_R" );
    matrixString = "0.5250603 0.01771525 -0.8508808 -4.76837e-7 -0.3603519 0.9103686 -0.2034111 1.480745 0.7710113 0.4134196 0.4843819 1.19209e-7 0 0 0 1";
    REQUIRE( glm::determinant(lowerArmR->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( lowerArmR->children.size() == 1);

    std::shared_ptr<SkeletonNode> handR = lowerArmR->children[0];
    REQUIRE( handR->id == "Armature_Hand_R" );
    REQUIRE( handR->name == "Hand.R" );
    REQUIRE( handR->sid == "Hand_R" );
    matrixString = "0.9693302 -0.178756 -0.1686568 -1.07288e-6 0.1513025 0.9748496 -0.1636341 1.083283 0.1936658 0.1330971 0.971997 1.66893e-6 0 0 0 1";
    REQUIRE( glm::determinant(handR->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( handR->children.size() == 1);

    std::shared_ptr<SkeletonNode> handREnd = handR->children[0];
    REQUIRE( handREnd->id == "Armature_Hand_R_end" );
    REQUIRE( handREnd->name == "Hand.R_end" );
    REQUIRE( handREnd->sid == "Hand_R_end" );
    matrixString = "0.9999999 4.47035e-8 -8.9407e-8 0 -4.47035e-8 0.9999999 2.08616e-7 0.7135496 6.70552e-8 -1.78814e-7 1 2.38419e-7 0 0 0 1";
    REQUIRE( glm::determinant(handREnd->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( handREnd->children.size() == 0);

    std::shared_ptr<SkeletonNode> upperLegL = torso->children[1];
    REQUIRE( upperLegL->id == "Armature_Upper_Leg_L" );
    REQUIRE( upperLegL->name == "Upper_Leg.L" );
    REQUIRE( upperLegL->sid == "Upper_Leg_L" );
    matrixString = "-0.9999337 0.01149671 -8.3375e-4 0.5668193 -0.01149607 -0.9894001 0.144762 -0.001348972 8.40411e-4 0.1447601 0.9894729 -0.1043749 0 0 0 1";
    REQUIRE( glm::determinant(upperLegL->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( upperLegL->children.size() == 1 );

    std::shared_ptr<SkeletonNode> lowerLegL = upperLegL->children[0];
    REQUIRE( lowerLegL->id == "Armature_Lower_Leg_L" );
    REQUIRE( lowerLegL->name == "Lower_Leg.L" );
    REQUIRE( lowerLegL->sid == "Lower_Leg_L" );
    matrixString = "-0.999156 0.04105425 -0.001460831 0 0.04052997 0.9793482 -0.1980774 1.282774 -0.006701252 -0.1979695 -0.9801853 -2.98023e-8 0 0 0 1";
    REQUIRE( glm::determinant(lowerLegL->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( lowerLegL->children.size() == 1 );

    std::shared_ptr<SkeletonNode> lowerFootL = lowerLegL->children[0];
    REQUIRE( lowerFootL->id == "Armature_Foot_L" );
    REQUIRE( lowerFootL->name == "Foot.L" );
    REQUIRE( lowerFootL->sid == "Foot_L" );
    matrixString = "-0.999156 0.04105425 -0.001460831 0 0.04052997 0.9793482 -0.1980774 1.282774 -0.006701252 -0.1979695 -0.9801853 -2.98023e-8 0 0 0 1";
    REQUIRE( glm::determinant(lowerFootL->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON );
    REQUIRE( lowerFootL->children.size() == 1 );

    std::shared_ptr<SkeletonNode> lowerFootLEnd = lowerFootL->children[0];
    REQUIRE( lowerFootLEnd->id == "Armature_Foot_L_end" );
    REQUIRE( lowerFootLEnd->name == "Foot.L_end" );
    REQUIRE( lowerFootLEnd->sid == "Foot_L_end" );
    matrixString = "1 -6.43959e-8 3.27903e-7 5.96046e-8 6.4396e-8 1 -2.98023e-8 0.9110205 -3.27903e-7 0 1 0 0 0 0 1";
    REQUIRE( glm::determinant(lowerFootLEnd->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( lowerFootLEnd->children.size() == 0 );

    std::shared_ptr<SkeletonNode> upperLegR = torso->children[2];
    REQUIRE( upperLegR->id == "Armature_Upper_Leg_R" );
    REQUIRE( upperLegR->name == "Upper_Leg.R" );
    REQUIRE( upperLegR->sid == "Upper_Leg_R" );
    matrixString = "-0.9999337 -0.01149671 -8.34033e-4 -0.5668193 0.01125456 -0.9894001 0.1447814 -0.001348972 -0.002490893 0.1447601 0.9894712 -0.1043749 0 0 0 1";
    REQUIRE( glm::determinant(upperLegR->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( upperLegR->children.size() == 1 );

    std::shared_ptr<SkeletonNode> lowerLegR = upperLegR->children[0];
    REQUIRE( lowerLegR->id == "Armature_Lower_Leg_R" );
    REQUIRE( lowerLegR->name == "Lower_Leg.R" );
    REQUIRE( lowerLegR->sid == "Lower_Leg_R" );
    matrixString = "-0.9991658 -0.04072399 0.003094322 -5.96046e-8 -0.0405297 0.979348 -0.1980772 1.282774 0.005036073 -0.1980373 -0.9801815 -2.98023e-8 0 0 0 1";
    REQUIRE( glm::determinant(lowerLegR->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( lowerLegR->children.size() == 1 );

    std::shared_ptr<SkeletonNode> footR = lowerLegR->children[0];
    REQUIRE( footR->id == "Armature_Foot_R" );
    REQUIRE( footR->name == "Foot.R" );
    REQUIRE( footR->sid == "Foot_R" );
    matrixString = "-0.9995611 -0.008793648 0.02829308 0 -0.02962813 0.2966889 -0.9545145 1.493332 -5.76714e-7 -0.9549337 -0.2968192 1.49012e-8 0 0 0 1";
    REQUIRE( glm::determinant(footR->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( footR->children.size() == 1 );

    std::shared_ptr<SkeletonNode> footREnd = footR->children[0];
    REQUIRE( footREnd->id == "Armature_Foot_R_end" );
    REQUIRE( footREnd->name == "Foot.R_end" );
    REQUIRE( footREnd->sid == "Foot_R_end" );
    matrixString = "1 -2.61649e-7 1.2222e-6 0 2.61649e-7 0.9999999 -8.9407e-8 0.9110197 -1.2222e-6 8.9407e-8 1 -2.98023e-8 0 0 0 1";
    REQUIRE( glm::determinant(footREnd->matrix) - glm::determinant(glm::make_mat4(Loader::SplitStringFloat(matrixString).data())) < EPSILON);
    REQUIRE( footREnd->children.size() == 0 );
}