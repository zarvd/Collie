file(GLOB SRCS *.cc)

ADD_EXECUTABLE(unittest ${SRCS})

TARGET_LINK_LIBRARIES(unittest collie gtest)

add_test(NAME unittest
         COMMAND unittest --gtest_output=xml:report.xml)
