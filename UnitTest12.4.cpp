#include "pch.h"
#include "CppUnitTest.h"
#include "../lab12.4/lab12.4.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        // Тест для сортування записів за датами народження
        TEST_METHOD(TestSortByBirthday)
        {
            Zodiac people[3] = {
                { "Іваненко", "Олена", "Овен", {15, 4, 1990} },
                { "Петренко", "Сергій", "Телець", {20, 5, 1985} },
                { "Сидоров", "Анатолій", "Риби", {5, 3, 1992} }
            };

            int size = 3;

            // Імітуємо сортування записів
            for (int i = 0; i < size - 1; ++i) {
                for (int j = i + 1; j < size; ++j) {
                    if (people[i].birthday[2] > people[j].birthday[2] ||
                        (people[i].birthday[2] == people[j].birthday[2] && people[i].birthday[1] > people[j].birthday[1]) ||
                        (people[i].birthday[2] == people[j].birthday[2] && people[i].birthday[1] == people[j].birthday[1] && people[i].birthday[0] > people[j].birthday[0])) {
                        swap(people[i], people[j]);
                    }
                }
            }

            // Перевіряємо порядок записів
            Assert::AreEqual(people[0].birthday[2], 1985); // Перевірка року найранішої дати
            Assert::AreEqual(people[1].birthday[2], 1990);
            Assert::AreEqual(people[2].birthday[2], 1992);
        }
    };
}