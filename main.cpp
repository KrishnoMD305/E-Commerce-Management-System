#include<iostream>
#include<sstream> // for string string manipulation
#include<list> // for list<> stl container
#include<vector> // for vector<> STL container
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
// The cart item will be a struct type
struct CartItem{
    Product* product;
    int quantity; // quantity of that product

    // Constructor
    CartItem(Product* p, int q): product(p),quantity(q) {}
};


class Cart{
private:
// the operation in list is O(1)
// efficient
    list<CartItem> items; 

public:
    // add product in cart method
    void addProduct(Product* p, int qty){
        //checking if the product is already on cart or not
        for(auto& item : items){
            // checking by ID
            if(item.product->getID() == p->getID()){
                item.quantity += qty; // if existed then just updated the quantity
                cout<<"  Updated quantity of '"<<p->getName()<<"' in cart.\n";
                return;
            }
        }
        // if it is not already in cart then push back in the cart
        items.push_back(CartItem(p,qty));
        cout<<"  Added '"<<p->getName()<<"' to cart (Qty: "<<qty<<").\n";
    }

    // Overloaded the addproduct function
    void addProduct(Product* p){
        addProduct(p,1); // quantity = 1
    }

    // for showing cart in console
    void displayCart()const{
        // if empty then show empty message
        if(items.empty()){
            cout<<"  Your cart is empty.\n";
            return;
        }
        cout<<"\n  === YOUR CART ===\n";
        float total = 0; // for calculating total price
        for(const auto& item : items){
            cout<<"  - "<<item.product->getName()<<" x"<<item.quantity<<" @ $"<<item.product->getPrice()<<" each\n";
            total += item.product->getPrice() * item.quantity; // added in total
        }
        cout<<"  Total: $"<<total<<"\n"; // show total
    }

    // calculate the total price that are available in carts
    float calculateTotal()const{
        float total = 0;
        for(const auto& item : items){
            // here item is CartItem struct type
            // the product is Product type
            // getPrice is the getter method for Product class
            total += item.product->getPrice() * item.quantity;
        }
        return total;
    }

    // returns a constant reference to a list of CartItem objects
    const list<CartItem>& getItems()const{
        return items;
    }


    // clear the list 
    void clearCart(){
        items.clear();
    }

    // checks if list is empty
    bool isEmpty()const{
        return items.empty();
    }


};


class Order{
private:
    static int orderCounter; // static member for order id generation
    int orderId;
    string username;
    vector<CartItem> orderedItems;
    float totalAmount;
    string status; // status will be pending

public:

    // Constructor
    Order(const string& user, const list<CartItem>& items, float total) : username(user), totalAmount(total), status("Pending"){
        orderId = ++orderCounter;

        // copying the cart items to order
        for(const auto& item : items){
            orderedItems.push_back(item);
        }
    }

    // display the order items 
    void displayOrder()const{
        cout<<"\n  === ORDER #"<<orderId<<" ===\n";
        cout<<"  Customer: "<<username<<"\n";
        cout<<"  Status: "<<status<<"\n";
        cout<<"  Items:\n";

        // show the item names and the quantity
        for(const auto& item : orderedItems){
            cout<<"    - "<<item.product->getName()<<" x"<<item.quantity<<"\n";
        }

        cout<<"  Total Amount: $"<<totalAmount<<"\n";
    }

    // getter methods
    int getOrderId()const{
        return orderId;
    }
    float getTotalAmount()const{
        return totalAmount;
    }

};


int Order::orderCounter = 1000; // initializing the static member


class User{
protected:
// attributes for users
    string username;
    string password;
    string email;
    static int totalUsers; //

public:
    // Constructor
    User(string uname, string pass, string mail) : username(uname), password(pass), email(mail){
        totalUsers++;
    }

    // virtual destructor
    virtual ~User(){

    }

    // virtual function for runtime polymorphismmm
    virtual void displayInfo()const{
        cout<<"  Username: "<<username<<"\n";
        cout<<"  Email: "<<email<<"\n";
    }

    // virtual getrole function
    virtual string getRole()const{
        return "User";
    }

    // pure virtual function
    virtual string serialize() const = 0;

    // getters methods
    string getUsername()const{
        return username;
    }
    string getPassword()const{
        return password;
    }
    string getEmail()const{
        return email;
    }
    static int getTotalUsers(){
        return totalUsers;
    }

};

int User::totalUsers = 0; // initialize the static member

// inherited from user
class Customer: public User{
private:
    Cart cart;
    // stores the orders of the customers
    vector<Order> orderHistory; // store Order objects

public:
// Constructor
    Customer(string uname, string pass, string mail) : User(uname,pass,mail){

    }

    // destructor
    ~Customer(){}

    // override the virtual function
    void displayInfo()const override{
        cout<<"  [Customer Profile]\n";
        User::displayInfo();
        cout<<"  Total Orders: "<<orderHistory.size()<<"\n";
    }

    // override
    string getRole()const override{
        return "Customer";
    }

    // overridden function
    // serialize customer data to file
    string serialize()const override{
        stringstream ss;
        ss<<"CUSTOMER|"<<username<<"|"<<password<<"|"<<email;
        return ss.str();
    }

    // return the cart
    Cart& getCart(){
        return cart;
    }

    // added the product and quantity through customer
    void addToCart(Product* p, int qty){
        cart.addProduct(p,qty);
    }
    // display the cart
    void viewCart()const{
        cart.displayCart();
    }

// method for placing order 
    void placeOrder(){
        if(cart.isEmpty()){
            cout<<"  Cart is empty. Add items before placing order.\n";
            return;
        }

        float total = cart.calculateTotal();
        // create a new order object
        Order newOrder(username, cart.getItems(), total);

        // reduce the stocks forrr  each product
        for(const auto& item : cart.getItems()){
            item.product->reduceStock(item.quantity);
        }
        // added the order to orderhistory
        orderHistory.push_back(newOrder);
        cout<<"\n  Order placed successfully! Order ID: "<<newOrder.getOrderId()<<"\n";
        cart.clearCart();

    }

    // view the history of order
    void viewOrderHistory()const{
        if(orderHistory.empty()){
            cout<<"  No order history available.\n";
            return;
        }

        cout<<"\n  === ORDER HISTORY ===\n";

        for(const auto& order : orderHistory){
            order.displayOrder();
        }
    }


};

// also inherited from user
class Admin: public User{
public:



};


// Main system class for everything
class ECommerceSystem{

};

int main(){

    return 0;
}