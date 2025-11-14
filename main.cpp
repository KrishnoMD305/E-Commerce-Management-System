#include<iostream>
using namespace std;


class Product{
protected:
    // attributes for products
    int id;
    string name;
    float price;
    int stock;
    static int totalProducts; // it should be shared by all objects that's why static
};
// Main system class for everything
class ECommerceSystem{

};

int main(){

    return 0;
}