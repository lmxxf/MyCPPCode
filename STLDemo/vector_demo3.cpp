// #include <iostream>
// #include <vector>

// int main() {
//     std::vector<int> v1 = {1, 2, 3, 4, 5};

//     // 使用新的元素替换v1中的元素
//     v1.assign({100, 200, 300});

//     for (int value : v1) {
//         std::cout << value << " ";
//     }

//     return 0;
// }



// #include <iostream>
// #include <vector>

// int main() {
//     std::vector<int> v1 = {1, 2, 3, 4, 5};

//     // 使用5个10替换v1中的元素
//     v1.assign(3, 10);

//     for (int value : v1) {
//         std::cout << value << " ";
//     }

//     return 0;
// }


// #include <iostream>
// #include <vector>

// int main() {
//     std::vector<int> v1 = {1, 2, 3, 4, 5};
//     std::vector<int> v2 = {10, 20, 30, 40, 50, 60};

//     // 使用v2的前4个元素替换v1中的元素
//     v1.assign(v2.begin(), v2.begin() + 4);

//     for (int value : v1) {
//         std::cout << value << " ";
//     }

//     return 0;
// }


#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector = {10,20,30};

  auto it = myvector.emplace ( myvector.begin()+1, 100 );
  myvector.emplace ( it, 200 );
  myvector.emplace ( myvector.end(), 300 );

  std::cout << "myvector contains:";
  for (auto& x: myvector)
    std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
