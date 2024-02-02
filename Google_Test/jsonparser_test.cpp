#include "../Server/include/jsonparser.h"
#include "gtest/gtest.h"

// создаем тестовый класс, унаследованный от ::testing::Test
class jsonparserTest : public ::testing::Test {
protected:
    // определяем метод SetUp(), который будет вызываться перед каждым тестом
    void SetUp() override {
        // создаем объект класса jsonparser
        jp = new Serf::jsonparser();
    }

    // определяем метод TearDown(), который будет вызываться после каждого теста
    void TearDown() override {
        // удаляем объект класса jsonparser
        delete jp;
    }

    // объявляем указатель на объект класса jsonparser
    Serf::jsonparser* jp;
};

// создаем тестовый метод, который будет проверять корректность работы метода StringSorting()
TEST_F(jsonparserTest, StringSortingTest1) {
    // задаем тестовую строку
    std::string message = "127.0.0.1:37316: {\"formula\":\"5+5/2\"}\n";
    // вызываем метод StringSorting() с тестовой строкой
    jp->StringSorting(message);
   
    // проверяем, что id и json имеют ожидаемые значения
    ASSERT_EQ(jp->GetId(), "127.0.0.1:37316");
    ASSERT_EQ(jp->GetJson(), "{\"formula\":\"5+5/2\"}\n");
}

TEST_F(jsonparserTest, StringSortingTest2) {
    // задаем тестовую строку
    std::string message = "127.0.0.1:37316: {\"formula\":\"5+5/2\"}\n";
    // вызываем метод StringSorting() с тестовой строкой
    jp->StringSorting(message);
   
    // проверяем, что id и json имеют ожидаемые значения
    ASSERT_EQ(jp->GetId(), "127.0.0.1:37316");
    ASSERT_EQ(jp->GetJson(), "{\"formula\":\"5+5/2\"}\n");
}
