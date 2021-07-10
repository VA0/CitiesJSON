cmake_minimum_required(VERSION 3.8)
#cmake_policy(SET CMP0079 NEW)


add_executable(CitiesTest Tests.cpp)



# target_link_libraries(Cities PUBLIC
#     gtest_main
#     Example
# )

# add_test(
#      NAME Cities
#      COMMAND Cities
#  )

# add_test(test
#     CitiesTest
#     CitiesTest
#  )

 
# add_test(GetTrue_Correct
# CitiesTest
# )

add_test(isNumber
CitiesTest
)
add_test(LoadJSON
CitiesTest
)
#add_test(LoadJSON_CorrectPath
#CitiesTest
#)





#include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${COMMON_INCLUDES}) 

INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${COMMON_INCLUDES})
TARGET_LINK_LIBRARIES(CitiesTest ${Boost_LIBRARIES} gtest)

TARGET_LINK_LIBRARIES(CitiesTest Cities)


