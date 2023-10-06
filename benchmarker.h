//
// Created by rudri on 10/4/2023.
//

#ifndef PROG3_COMPLEJIDAD_ALGORITMICA_BENCHMARKER_H
#define PROG3_COMPLEJIDAD_ALGORITMICA_BENCHMARKER_H

#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

template <typename T, template <typename...> typename Container>
struct generate_type{
  using type = Container<T>;
};

template <typename T>
struct generate_type<T, std::map>{
  using type = std::map<T, T>;
};

template <typename T, size_t sz, size_t step, template<typename...>class ...Containers>
class bench_marker {
  
  inline static std::unordered_map<std::type_index, std::string> type_names = {
      {std::type_index(typeid(std::vector<T>)), "vector<T>"},
      {std::type_index(typeid(std::deque<T>)), "deque<T>"},
      {std::type_index(typeid(std::list<T>)), "list<T>"},
      {std::type_index(typeid(std::forward_list<T>)), "forward_list<T>"},
      {std::type_index(typeid(std::map<T, T>)), "map<T,T>"},
  };
 
  std::ofstream file;
  std::tuple<typename generate_type<T, Containers>::type...>
      containers {typename generate_type<T, Containers>::type{}...};
  
  auto dice() {
    static std::uniform_int_distribution dis {1, 100};
    static std::random_device device;
    static std::mt19937 engine {device()};
    return dis (engine);
  }

template <template <class...> class ContainerClass, typename ...Rest>
  void insert(ContainerClass<T, Rest...>& cnt) {
    cnt.push_back(dice());
  }

  void insert(std::map<T, T>& cnt) {
    cnt.insert({dice(), dice()});
  }
  
  template <typename Container>
  void record_time(Container& cnt, size_t n) {
    cnt.clear();
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i)
      insert(cnt);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    file << elapsed.count() << " ";
  }
  
public:
  explicit bench_marker(const std::string& file_name):
                       file{file_name} {}
  
  void execute() {
    std::apply([this](typename generate_type<T, Containers>::type... cont){
      file << "# ";
      ((file << type_names[std::type_index(typeid(cont))] << " "), ...);
      file << '\n';
    }, containers);
    for (size_t i = step; i < sz; i+=step) {
      std::apply([this, i](typename generate_type<T, Containers>::type... cont){
        file << i << " ";
        (record_time(cont, i), ...);
        file << '\n';
      }, containers);
    }
  }
  
};

#endif//PROG3_COMPLEJIDAD_ALGORITMICA_BENCHMARKER_H
