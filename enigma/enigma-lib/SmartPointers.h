#pragma once
#include <memory>

template <typename T> using SharedPtr = std::shared_ptr<T>;
template <typename T> using WeakPtr = std::weak_ptr<T>;
template <typename T> using UniquePtr = std::unique_ptr<T>;

using std::make_shared, std::make_unique, std::move;
