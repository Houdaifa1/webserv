#include <iostream>
#include <set>


int main()
{
    std::set<int> num;

   num.insert(7);
   num.insert(1);
   num.insert(5);
   num.insert(2);
   num.insert(1);

   num.erase(7); // to remove a number 

    std::set<int>::iterator it;

    for (it = num.begin(); it != num.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "num.count5: " << num.count(5) << std::endl;
    std::cout << "num.count2: " << num.count(2) << std::endl;

    char buffer[4096];
    int bytes = 5;
    // buffer = "hello";
    std::string raw_request("hello", bytes);
    std::cout << " raw_request: " << raw_request << std::endl;
}