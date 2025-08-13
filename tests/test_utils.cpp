// #include <cassert>

#include "gtest/gtest.h"
#include <utils.hpp>

namespace test_utils
{

Logger logger("test_utils");

TEST(Utils, testSize)
{
    Size size1;
    Size size2({200,300,400});
    Size size3({200,300,400},50);
    Size size4({200,300,400},{20,30,40});
    // std::cout<<size4.rotate(RotateOrient::I).to_json()<<std::endl;
    // std::cout<<size4.to_json()<<std::endl;
    
    ASSERT_EQ(size1.remain,Vec3i({0,0,0}));
    ASSERT_EQ(size1.size,Vec3i({0,0,0}));
    ASSERT_EQ(size2.size,Vec3i({200,300,400}));
    ASSERT_EQ(size3.remain,Vec3i({50,50,50}));
    ASSERT_EQ(size4.rotate(RotateOrient::I),Size({200,300,400},{20,30,40}));
    ASSERT_EQ(size4.rotate(RotateOrient::X),Size({200,400,300},{20,40,30}));
    ASSERT_EQ(size4.rotate(RotateOrient::Y),Size({400,300,200},{40,30,20}));
    ASSERT_EQ(size4.rotate(RotateOrient::Z),Size({300,200,400},{30,20,40}));
    ASSERT_EQ(size4.rotate(RotateOrient::XY),Size({300,400,200},{30,40,20}));
    ASSERT_EQ(size4.rotate(RotateOrient::XZ),Size({400,200,300},{40,20,30}));


    std::pair<int,int> x(200,20);
    ASSERT_EQ(size4[Orient::X],x);
    std::pair<int,int> y(300,30);
    ASSERT_EQ(size4[Orient::Y],y);
    std::pair<int,int> z(400,40);
    ASSERT_EQ(size4[Orient::Z],z);
    logger.log_json("Size",size4.to_json());
}

TEST(Utils, testPartType)
{
    PartType partType3(3,500,600,700,true);
    PartType partType3b=partType3;
    ASSERT_EQ(partType3b,partType3);
    logger.log_json("PartType",partType3.to_json());
}

TEST(Utils, testSheetType)
{
    SheetType SheetType3(3,500,600,700,true);
    SheetType SheetType3b=SheetType3;
    ASSERT_EQ(SheetType3b,SheetType3);
    logger.log_json("SheetType",SheetType3.to_json());
}
}