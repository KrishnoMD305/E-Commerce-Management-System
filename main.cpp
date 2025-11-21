#include<iostream>
#include<sstream> // for string string manipulation
#include<list> // for list<> stl container
#include<vector> // for vector<> STL container
#include<map> // for map<> STL container
#include<fstream> // for input and output manipulation
using namespace std;

// forward declaration
class User;
class Order; 


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

    // friend function declaration
    friend void displayOrderSummary(const Order& order, const User& user);

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

    // friend function declaration
    friend void displayOrderSummary(const Order& order, const User& user);

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
    // show the customer info
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
// constructor
    Admin(string uname, string pass, string mail): User(uname,pass,mail){

    }
    // destructor
    ~Admin(){

    }

    // overriden the function
    // display the admin info
    void displayInfo()const override{
        cout<<"  [Admin Profile]\n";
        User::displayInfo();
        cout<<"  Role: Administrator\n";
    }

    // override the getrole function
    string getRole()const override{
        return "Admin";
    }

    //override the function
    // serialize admin data to file
    // converts the admin objects data into a single formatted string
    // 
    string serialize()const override{ // function is marked const because it doesn't modify any class member
        stringstream ss; // string stream to build the output string
        ss<<"ADMIN|"<<username<<"|"<<password<<"|"<<email; // separated by |
        return ss.str();
    }


};

// a friend function of both the order and user classes
// allows the private and protected member accessing
void displayOrderSummary(const Order& order, const User& user){
    // const reference is used to avoid unnecessary copying and ensure not modifying objects
    cout<<"\n  === ORDER SUMMARY ===\n";
    cout<<"  Order ID: "<<order.orderId<<"\n"; // unique order id
    cout<<"  Customer: "<<user.username<<" ("<<user.email<<")\n";
    cout<<"  Total: $"<<order.totalAmount<<"\n"; // total amount customer must pay
    cout<<"  Status: "<<order.status<<"\n";
}


// Function Templatee
// reusable template function for displaying a list of objects of type T
template<typename T>
void displayList(const vector<T*>& items, const string& title){
    cout<<"\n  === "<<title<<" ===\n"; // print the list title
    if(items.empty()){
        cout<<"  No items available.\n";
        return;
    }

    // iterate over each item in the list
    for(const auto& item : items){
        item->displayDetails();
        cout<<"  "<<string(50, '-')<<"\n"; // print a separator line
    }

}

// class template
template<typename T>
class Repository{
private:
    map<int, T*> storage; // container
    string filename;

public: 
// Constructor and destructor
    Repository(const string& file) : filename(file){}
    ~Repository(){
        for(auto& pair : storage){
            delete pair.second;
        }
    }

    // add or replaces an object
    void add(int key, T* item){
        storage[key] = item;
    }

    // get object associated with specified key
    T* get(int key){
        auto it = storage.find(key);
        if(it != storage.end()){
            return it->second;
        }
        return nullptr;
    }

    // list of all storage objects
    vector<T*> getAll(){
        vector<T*> result;
        for(auto& pair : storage){
            result.push_back(pair.second);
        }
        return result;
    }

    //delete the object and key
    void remove(int key){
        auto it = storage.find(key);
        if(it != storage.end()){
            delete it->second;
            storage.erase(it);
        }
    }

    // chk whether the given key exists
    bool exists(int key){
        return storage.find(key) != storage.end();
    }

    // returns the number of objects
    int size()const{
        return storage.size();
    }

    // freeing memory
    void clear(){
        for(auto& pair : storage){
            delete pair.second;
        }
        storage.clear();
    }

    //direct access to map
    map<int, T*>& getStorage(){
        return storage;
    }

};

// handles all I/O operations
class FileHandler{
private:
// holds the filename or path where product data will be stored or loaded.
    // declared as static so only one copy exists for the entire class
    // const ensures the file path cannot be modified after initialization
    static const string PRODUCTS_FILE;
    // store the filename for user related data
    static const string USERS_FILE;

public:

// saves all products stored in the given repository to the file
// serializes each Product object to a string and writes it line by line into the file
    static void saveProducts(Repository<Product>& repo){ // static because not object specified
        ofstream file(PRODUCTS_FILE);
        if(!file.is_open()){
            cout<<"  [ERROR] Unable to open products file for writing.\n";
            return;
        }

        vector<Product*> products = repo.getAll(); // retrieve all stored Product objects from the repository
        
        // iterate through each Product and write its serialized representation
        for(const auto& product : products){
            file<<product->serialize()<<"\n";
        }
        file.close();
        cout<<"  [FILE] Products saved successfully!\n";
    }

    // loads product data from the file and adds each product and and stores them in the repository
    static void loadProducts(Repository<Product>& repo){
        ifstream file(PRODUCTS_FILE);

        if(!file.is_open()){
            cout<<"  [FILE] Products file not found. Creating new file...\n";
            return;
        }
        string line;

        // read the file line by line
        // each line represents a serialized product
        while(getline(file, line)){
            // skip empty lines to avoid errors or empty product
            if(line.empty()) continue;

            // load the serialized line into a stringstream for extraction
            stringstream ss(line);
            // product fields
            string type, name, link, dimensions;
            int id, stock;
            float price, fileSize, weight;

            // extract id, price, stock
            getline(ss, type, '|');
            ss>>id; // 
            ss.ignore(); // skip delimiter
            getline(ss, name, '|');
            ss>>price;
            ss.ignore(); // skip delimiter
            ss>>stock;
            ss.ignore(); // skip delimiter

            if(type == "DIGITAL"){
                // extract link and file size
                getline(ss, link, '|');
                ss>>fileSize;

                // add the reconstructed digital to the repository
                repo.add(id, new DigitalProduct(id, name, price, stock, link, fileSize));
            }else if(type == "PHYSICAL"){
                ss>>weight;
                ss.ignore();
                getline(ss,dimensions);

                // add the reconstructed physical to the repository
                repo.add(id, new PhysicalProduct(id,name,price,stock,weight,dimensions));
            }

        }

        file.close();
        cout<<"  [FILE] Loaded "<<repo.size()<<" products from file.\n";

    }

    // saves all User objects to the file
    static void saveUsers(Repository<User>& repo){
        ofstream file(USERS_FILE);
        if(!file.is_open()){
            cout<<"  [ERROR] Unable to open users file for writing.\n";
            return;
        }

        vector<User*> users = repo.getAll(); // retrieve all User objects

        // iterate over each User and save serialized form to the file
        for(const auto& user : users){
            file<<user->serialize()<<"\n"; // write one user per line
        }

        file.close();
        cout<<"  [FILE] Users saved successfully!\n";
    }

    // loads all user data from file
    static void loadUsers(Repository<User>& repo){
        ifstream file(USERS_FILE);
        if(!file.is_open()){
            cout<<"  [FILE] Users file not found. Creating new file...\n";
            return;
        }

        string line;
        int idCounter = 1; // assign increasing id to loaded users

        // read the file linebyline
        // each line contains one serialized User entry
        while(getline(file, line)){
            // skip empty lines to avoid unnecessary parsing
            if(line.empty()) continue;

            stringstream ss(line); // use stringstream to extract the serialized user fields
            string type, username, password, email;

            // extract each field separated by |
            getline(ss,type, '|');
            getline(ss,username, '|');
            getline(ss,password, '|');
            getline(ss,email);

            // determine the type of User
            if(type == "CUSTOMER"){
                repo.add(idCounter++, new Customer(username,password,email));
            }else if(type == "ADMIN"){
                repo.add(idCounter++, new Admin(username,password,email));
            }
        }

        file.close();
        cout<<"  [FILE] Loaded "<<repo.size()<<" users from file.\n";
    }

    // checks whether the required data files exist and are accessible for reading
    /*
    true-if both files can be successfully opened
    false-if file cannot be accessed
    */
    static bool filesExist(){
        ifstream pFile(PRODUCTS_FILE);
        ifstream uFile(USERS_FILE);

        // check if both files were successfully opened
        bool exists = pFile.good() && uFile.good(); // good() returns true if the stream is usable state

        pFile.close();
        uFile.close();

        return exists; // return true only if both files were accessible
    }

};

// initialize the static variables
const string FileHandler::PRODUCTS_FILE = "products.txt";
const string FileHandler::USERS_FILE = "users.txt";


// Main system class for everything
class ECommerceSystem{

};

int main(){

    return 0;
}