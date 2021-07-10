//#include "../googletest/googletest/include/gtest/gtest.h"
#include <gtest/gtest.h>
#include <iostream>
#include "../Cities.h"

using std::cout;
using std::endl;
using std::string;


//Check if "number string" is being detected correctly
TEST(isNumber, CorrectString)
{
    cout << "IsNumber_CorrectString\n";
    string str = "139";

    bool result = isNumber(str);
    EXPECT_EQ(true, result);
}


//Check if "letter string" is being detected correctly
TEST(isNumber, LetterString)
{
    string str = "abc";

    bool result = isNumber(str);
    EXPECT_EQ(false, result);
}

//Check if loading function returns true for correct path & file
TEST(LoadJSON, CorrectPath)
{
    string str = R"(C:\Users\Filip\source\repos\Cities\cities.json)";
    bool result = LoadJSON((char*)str.c_str());
    EXPECT_EQ(true, result);
}

//Check if loading function returns false for incorrect path or file
TEST(LoadJSON, InCorrectPathFile)
{
    string str = R"(C:\Us311erss.json)";
    bool result = LoadJSON((char*)str.c_str());
    EXPECT_EQ(false, result);
}


int main() {
  auto old_buffer = std::cout.rdbuf(nullptr); //this hides cities.lib couts / printfs for sake of unit tests
  
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}