// Course project 4 - Thread-safe queue.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include "queue.cpp"
#include "pool.cpp"

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    thread_pool pool;

    for (int i = 0; i < 100; ++i)
    {
        pool.submit([=] { std::cout << "Владимир: " << std::this_thread::get_id() << std::endl; });
    }
}