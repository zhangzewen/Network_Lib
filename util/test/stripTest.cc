#include <gtest/gtest.h>
#include "util.h"


TEST(lstripTest, StringUtilTest)
{
  ASSERT_EQ(std::string("zhangjie\tjie    "), lstrip(std::string("\t\r\n   zhangjie\tjie    ")));
  ASSERT_EQ(std::string("\r\n   zhangjie\tjie    "), lstrip(std::string("\t\r\n   zhangjie\tjie    "), std::string("\t")));
  ASSERT_EQ(std::string("\t\r\n   zhangjie\tjie    "), lstrip(std::string("\t\r\n   zhangjie\tjie    "), std::string("\r")));
  ASSERT_EQ(std::string("xxxzhangjie\tjie    "), lstrip(std::string("\t\r\n   zhangjie\tjie    "), std::string("x")));
}

//TEST(rstripTest, StringUtilTest)
//{
//  ASSERT_EQ(std::string("zhangjie\tjie    "), rstrip(std::string("\t\r\n   zhangjie\tjie    ")));
//  ASSERT_EQ(std::string("\r\n   zhangjie\tjie    "), rstrip(std::string("\t\r\n   zhangjie\tjie    "), std::string("\t")));
//  ASSERT_EQ(std::string("\t\r\n   zhangjie\tjie    "), rstrip(std::string("\t\r\n   zhangjie\tjie    "), std::string("\r")));
//  ASSERT_EQ(std::string("xxxzhangjie\tjie    "), rstrip(std::string("\t\r\n   zhangjie\tjie    "), std::string("x")));
//}
//
//TEST(stripTest, StringUtilTest)
//{
//  ASSERT_EQ(std::string("zhangjie\tjie    "), strip(std::string("\t\r\n   zhangjie\tjie    ")));
//  ASSERT_EQ(std::string("\r\n   zhangjie\tjie    "), strip(std::string("\t\r\n   zhangjie\tjie    "), std::string("\t")));
//  ASSERT_EQ(std::string("\t\r\n   zhangjie\tjie    "), strip(std::string("\t\r\n   zhangjie\tjie    "), std::string("\r")));
//  ASSERT_EQ(std::string("xxxzhangjie\tjie    "), strip(std::string("\t\r\n   zhangjie\tjie    "), std::string("x")));
//}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


