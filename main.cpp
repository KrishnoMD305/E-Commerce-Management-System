#include<iostream>
#include<sstream> // for string string manipulation
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

    // virtual destructor
    virtual ~Product(){}
    // if not added a virtual destructor in base class it will not call child destructor


    // These functions will be overriden in child class
    // child classes will give self defination of their own
    // pure virtual function
    virtual void displayDetails()const = 0;
    virtual string getType()const = 0;
    virtual string serialize()const = 0;



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


    // overriding the ppure virtual function of base class
    // shows digital products info
    void displayDetails()const override{
        cout<<"[Digital Product]\n";
        cout<<"  ID: "<<id<<" | Name: "<<name<<" | Price: $"<<price<<" | Stock: "<<stock<<"\n";
        cout<<"  Download Link: "<<downloadLink<<" | Size: "<<fileSize<<" MB\n";
    }

    // get the type digital
    string getType()const override{
        return "Digital";
    }

    // serialize the digital products list for saving to file
    string serialize()const override{
        stringstream ss; 
        ss<<"DIGITAL|"<<id<<"|"<<name<<"|"<<price<<"|"<<stock<<"|"<<downloadLink<<"|"<<fileSize;
        return ss.str();
    }

    ~DigitalProduct(){}

};

// also inherited from product
class PhysicalProduct:public Product{
private:
// physical products attributes
    float weight; // KG
    string dimensions; // A x B x C

public:
    //constructor
    PhysicalProduct(int i, string n, float p, int s, float w, string dim): Product(i,n,p,s), weight(w), dimensions(dim){}


    // overriding pure virtual function from parent class

    // show details
    void displayDetails()const override{
        cout<<"  [Physical Product]\n";
        cout<<"  ID: "<<id<<" | Name: "<<name<<" | Price: $"<<price<<" | Stock: "<<stock<<"\n";
        cout<<"  Weight: "<<weight<<" kg | Dimensions: "<<dimensions<<"\n";
    }

    // type - physical
    string getType()const override{
        return "Physical";
    }


    // arrange the product detailes for file storage
    string serialize()const override{
        stringstream ss;
        ss<<"PHYSICAL|"<<id<<"|"<<name<<"|"<<price<<"|"<<stock<<"|"<<weight<<"|"<<dimensions;
        return ss.str(); //
        // convert the stream into string and returns it
    }


    ~PhysicalProduct(){}

};

// item + the quantity of that item
struct CartItem{
    Product* product;
    int quantity; // quantity of that product

    // Constructor
    CartItem(Product* p, int q): product(p),quantity(q) {}
};




// Main system class for everything
class ECommerceSystem{

};

int main(){

    return 0;
}