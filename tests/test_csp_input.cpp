// #include <cassert>

#include "gtest/gtest.h"
#include <csp_input.hpp>
#include <fstream>
namespace test_csp_input
{
Logger logger("test_csp_input");
RawProblem rawProblem;

TEST(CspInput, testRawPartType)
{
    ASSERT_THROW(RawPartType(1,2,3,false,1,{2,1},"aaa"), std::runtime_error);
    ASSERT_NO_THROW(RawPartType(1,2,3,false,1,{1,2},"aaa"));
    // RawPartType rpt(1,2,3,false,std::array<int,2>(1,2),"aaa");

}

TEST(CspInput, testReadInventory)
{
    rawProblem.readInventoryFromXlsx("../assets/stk_inventory.xlsx");

}

TEST(CspInput, testReadIngredient)
{
    rawProblem.readIngredientFromXlsx("../assets/stk_pllist.xlsx");
    auto result=rawProblem.plids2vecRawPartType({28});
    for(auto rpt:result){
        logger.log(rpt->to_string());
    }
}

TEST(CspInput, testConstructProblem)
{
    auto problems=rawProblem.constructProblem({26});
    int i=0;
    for(const auto& problem:problems){
        i++;
        logger.log_json(std::to_string(i),problem.to_json());
    }

}

}

