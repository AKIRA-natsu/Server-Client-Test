#pragma once
#include <memory>

namespace AKIRA_Common {
    template <typename T>
    class Singleton {
    private:
        static std::shared_ptr<T> instance;
    protected:
        Singleton() {}              // prevent instantiation
    public:
        static std::shared_ptr<T> GetInstance() {
            if (instance == nullptr)
                instance = std::make_shared<T>();
            return instance;
        }
    };

    // store singleton instance
    // confirm first time use GetInstance(), instance will be created and store in intance
    template <typename T>
    std::shared_ptr<T> Singleton<T>::instance = nullptr;
}