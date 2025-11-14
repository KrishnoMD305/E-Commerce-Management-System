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

public:
    // Constructor
    Product(int i, string n, float p, int s): id(i), name(n), price(p), stock(s){
        totalProducts++; // increases static counter every time a new product is creted
    }


    // getters method 
    // used const so that the function cannot modify the data
    int getID()const{return id; }
    string getName()const{return name;}
    float getPrice()const{return price;}
    int getStock()const{return stock;}


    // setters method
    // 
    void setPrice(float p){price = p;}
    void setStock(int s){stock = s;}
    void reduceStock(int qty){stock -= qty;}

    // static function
    static int getTotalProducts(){
        return totalProducts;
    }




};

int Product::totalProducts = 0; //initializes the static variable

// Inherited from product
class DigitalProduct:public Product{
private:
    string downloadLink;
    float fileSize; //MB


public:
    // Constructor
    DigitalProduct(int i, string n, float p, int s, string link, float size): Product(i,n,p,s), downloadLink(link), fileSize(size){}




};


// Main system class for everything
class ECommerceSystem{

};

int main(){

    return 0;
}