#include<iostream>
#include<sstream> // for string string manipulation
#include<list> // for list<> stl container
#include<vector> // for vector<> STL container
#include<map> // for map<> STL container
#include<fstream> // for input and output manipulation
#include<algorithm> // for necessary built in algorithm
using namespace std;

// forward declaration
//because friend function is added
class User;
class Order; 

// base abstract class representing a generic product
class Product{
protected:
    // attributes for products
    int id; // unique product id
    string name; // name of the product
    float price; // price of the product
    int stock; // available stock quantity

    // tracks how many total product objects have been created
    static int totalProducts; // it should be shared by all objects that's why static

public:
    // Constructor
    Product(int i, string n, float p, int s): id(i), name(n), price(p), stock(s){ // initializes product attribute and increment product count
        totalProducts++; // increases static counter every time a new product is creted
    }

    // virtual destructor
    virtual ~Product(){} 
    // if not added a virtual destructor in base class it will not call child destructor


    // These functions will be overriden in child class
    // child classes will give self defination of their own
    // pure virtual function
    virtual void displayDetails()const = 0; // display productspecific details
    virtual string getType()const = 0; // return category of the product
    virtual string serialize()const = 0; // serialize deta into a string format

    // Type conversion
    //convert product to float
    operator float() const {
        return price; // returning price
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
    void reduceStock(int qty){stock -= qty;} // reduce stock after sales

    // static function
    // returns total number of product object created
    static int getTotalProducts(){
        return totalProducts;
    }

    // operator overloading compare by product id
    bool operator==(const Product& other) const {
        return this->id == other.id; // compare product by id
    }


};

int Product::totalProducts = 0; //initializes the static variable

// Inherited from product
// derived class represents a digital product
class DigitalProduct:public Product{
private: // private data members

    string downloadLink; // URL to download the product
    float fileSize; //MB


public:
    // Constructor
    // initialize product base attributes and additional
    DigitalProduct(int i, string n, float p, int s, string link, float size): Product(i,n,p,s), downloadLink(link), fileSize(size){}


    // overriding the ppure virtual function of base class
    // shows digital products info
    void displayDetails()const override{
        cout<<"[Digital Product]\n";
        cout<<"  ID: "<<id<<" | Name: "<<name<<" | Price: $"<<price<<" | Stock: "<<stock<<"\n";
        cout<<"  Download Link: "<<downloadLink<<" | Size: "<<fileSize<<" MB\n";
    }

    // overridden gettype virtual function
    // get the type digital
    string getType()const override{
        return "Digital";
    }

    // overridden serialize virtual function
    // serialize the digital products list for saving to file
    // converts the object data into a single line format
    string serialize()const override{
        stringstream ss; 

        // DIGITAL|id|name|price|stock|downloadLink|fileSize
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

    // opearetor overloading to add product in the cart
    Cart& operator+=(Product* p) {
        addProduct(p, 1);
        return *this;
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

    // operator overloading compare users by username 
    bool operator==(const User& other) const {
        return this->username == other.username;
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
private:
    Repository<Product> productRepo;
    Repository<User> userRepo;
    User* currentUser;


    void initializeSampleData(){
        cout<<"\n  [SYSTEM] Initializing sample data...\n";
        //
        // adding sample products
        productRepo.add(1, new DigitalProduct(1, "Ebook: C++ Mastery", 29.99, 100, "http://download.link/cpp", 5.2));
        productRepo.add(2, new DigitalProduct(2, "Software: Code Editor Pro", 49.99, 50, "http://download.link/editor", 250.5));
        productRepo.add(3, new PhysicalProduct(3, "Laptop Stand", 39.99, 30, 1.2, "30x25x5 cm"));
        productRepo.add(4, new PhysicalProduct(4, "Wireless Mouse", 24.99, 75, 0.15, "12x7x4 cm"));
        productRepo.add(5, new PhysicalProduct(5, "Mechanical Keyboard", 89.99, 20, 0.8, "45x15x3 cm"));

        // adding sample users
        userRepo.add(1, new Customer("alice", "pass123", "alice@example.com"));
        userRepo.add(2, new Customer("bob", "pass456", "bob@example.com"));
        userRepo.add(3, new Admin("admin", "admin123", "admin@ecommerce.com"));

        FileHandler::saveProducts(productRepo);
        FileHandler::saveUsers(userRepo);
        cout<<"  [SYSTEM] Sample data initialized and saved to files!\n";
    }

public:
    // Constructor
    ECommerceSystem() : productRepo("products.txt"),userRepo("users.txt"),currentUser(nullptr){

        // check whether previously saved data are available
        if(FileHandler::filesExist()){
            cout<<"\n  [SYSTEM] Loading data from files...\n";

            // load existing product and user data
            FileHandler::loadProducts(productRepo);
            FileHandler::loadUsers(userRepo);
        }else{
            cout<<"\n  [SYSTEM] No existing data found.\n";

            // if no data files exist create initial sample products and users
            initializeSampleData();
        }
    }

    // destructor
    ~ECommerceSystem(){
        cout<<"\n System shutting down...\n";
        // final saving
        FileHandler::saveProducts(productRepo);
        FileHandler::saveUsers(userRepo);
    }

    // displays the welcome message for the management system.
    void displayWelcome(){
        cout<<"\n";
        cout<<"  ========================================\n";
        cout<<"      E-COMMERCE MANAGEMENT SYSTEM\n";
        cout<<"      (With File Persistence)\n";
        cout<<"  ========================================\n";
    }

    // provides quick overview of the current state of the e-commerce system
    void displayStatistics(){
        cout<<"\n  === SYSTEM STATISTICS ===\n";
        cout<<"  Total Products: "<<Product::getTotalProducts()<<"\n";
        cout<<"  Total Users: "<<User::getTotalUsers()<<"\n";
        cout<<"  Products in Catalog: "<<productRepo.size()<<"\n";
    }

    // handles user authentication by asking for a username and password
    bool login(){
        string username,password;
        cout<<"\n  === LOGIN ===\n";
        cout<<"  Username: ";
        cin>>username;
        cout<<"  Password: ";
        cin>>password;

        // retrieve all registered users from the repository
        vector<User*> allUsers = userRepo.getAll();

        // search through all users and locate a user
        // [&](User* u)-lambda capturing variables by reference
        // returns bool checks if both username and password match
        auto it = find_if(allUsers.begin(),allUsers.end(),[&](User* u){return u->getUsername() == username && u->getPassword() == password;});

        if(it != allUsers.end()){
            currentUser = *it; // store the logged-in user
            cout<<"\n  Login successful! Welcome, "<<currentUser->getUsername()<<"!\n";
            return true;
        }

        // if no user matched failed login
        cout<<"\n  Invalid credentials. Please try again.\n";
        return false;


    }

// allows a new user to register an account in the system
    void registerUser(){
        string username, password, email;
        cout<<"\n  === REGISTER NEW USER ===\n";

        cout<<"  Username: ";
        cin>>username;

        cout<<"  Password: ";
        cin>>password;

        cout<<"  Email: ";
        cin>>email;

        // ensures unique incremental id
        int newId = userRepo.size() + 1;
        // create a new Customer object and add to the repository
        userRepo.add(newId, new Customer(username, password, email));

        //save all users to the file
        FileHandler::saveUsers(userRepo);

        cout << "\n  Registration successful! You can now login.\n";
    }

    /*
    This function uses runtime polymorphism to cast the current user into a 
    Customer object and then provides an interactive menu for browsing products, adding items to cart, 
    viewing the cart, placing orders, checking order history, and viewing profile information
    */
    void customerMenu(){
        // cast currentUser-base class pointer-to a Customer pointer
        Customer* customer = dynamic_cast<Customer*>(currentUser); // Runtime polymorphism
        if (!customer) return;

        while(true){
            cout<<"\n  === CUSTOMER MENU ===\n";
            cout<<"  1. Browse Products\n";
            cout<<"  2. Add Product to Cart\n";
            cout<<"  3. View Cart\n";
            cout<<"  4. Place Order\n";
            cout<<"  5. View Order History\n";
            cout<<"  6. View Profile\n";
            cout<<"  7. Logout\n";
            cout<<"  Choose option: ";

            int choice;
            cin>>choice;

            switch(choice){
                case 1:
                // displays product catalog
                    displayList(productRepo.getAll(),"PRODUCT CATALOG");
                    break;
                case 2:{
                    int productId, qty;
                    cout<<"  Enter Product ID: ";
                    cin>>productId;
                    cout<<"  Enter Quantity: ";
                    cin>>qty;

                    // retrieve product by id
                    Product* p = productRepo.get(productId);

                    // validate product existence and stock avilability
                    if(p && p->getStock() >= qty){
                        customer->addToCart(p, qty);
                        // type conversion Product to float
                        float price = static_cast<float>(*p);
                        cout<<"  Product price: $"<<price<<"\n";
                    }else{
                        cout<<"  Product not found or insufficient stock.\n";
                    }
                    break;
                }
                case 3: // view cart
                    customer->viewCart();
                    break;
                case 4: // place order
                    customer->placeOrder();
                    // save updated product stock to file
                    FileHandler::saveProducts(productRepo);
                    break;
                case 5: // view order history
                    customer->viewOrderHistory();
                    break;
                case 6:
                    customer->displayInfo(); //runtime polymorphism
                    break;
                case 7:
                    currentUser = nullptr;
                    return;
                default:
                    cout<<"  Invalid option.\n";

            }

        }
    }

    /*
    This function uses runtime polymorphism(dynamic_cast)to ensure that the current user is an Admin
    provides a menu-driven interface for managing products, system statistics, profile viewing
    */
    void adminMenu(){
        //safely cast currentUser to Admin pointer
        Admin* admin = dynamic_cast<Admin*>(currentUser);
        if(!admin) return;

        while(true){
            cout<<"\n  === ADMIN MENU ===\n";
            cout<<"  1. View All Products\n";
            cout<<"  2. Add New Product\n";
            cout<<"  3. Remove Product\n";
            cout<<"  4. Update Product Price\n";
            cout<<"  5. Update Product Stock\n";
            cout<<"  6. View System Statistics\n";
            cout<<"  7. Reload Data from Files\n";
            cout<<"  8. View Profile\n";
            cout<<"  9. Logout\n";
            cout<<"  Choose option: ";

            int choice;
            cin>>choice;

            switch(choice){

                case 1: // view all products
                    displayList(productRepo.getAll(), "ALL PRODUCTS");
                    break;

                case 2: { //add new product
                    int id, type, stock;
                    string name;
                    float price;

                    cout<<"  Product ID: ";
                    cin>>id;
                    cin.ignore();

                    cout<<"  Product Name: ";
                    getline(cin, name);

                    cout<<"  Price: $";
                    cin>>price;

                    cout<<"  Stock: ";
                    cin>>stock;

                    cout<<"  Type (1=Digital, 2=Physical): ";
                    cin>>type;

                    // digital product
                    if(type == 1){
                        string link;
                        float size;

                        cout<<"  Download Link: ";
                        cin>>link;

                        cout<<"  File Size (MB): ";
                        cin>>size;

                        productRepo.add(id, new DigitalProduct(id,name,price,stock,link,size));
                    }else{ // physical product
                        float weight;
                        string dim;

                        cout<<"  Weight (kg): ";
                        cin>>weight;
                        cin.ignore();

                        cout<<"  Dimensions: ";
                        getline(cin, dim);

                        productRepo.add(id, new PhysicalProduct(id,name,price,stock,weight,dim));
                    }

                    // save newly added product to file
                    FileHandler::saveProducts(productRepo);
                    cout<<"  Product added and saved to file!\n";
                    break;
                }
                case 3: { // remove product
                    int id;
                    cout<<"  Enter Product ID to remove: ";
                    cin>>id;

                    productRepo.remove(id);

                    // save changes to file
                    FileHandler::saveProducts(productRepo);
                    cout<<"  Product removed and changes saved!\n";
                    break;
                }
                case 4: {
                    int id;
                    float newPrice;

                    cout<<"  Enter Product ID: ";
                    cin>>id;

                    cout<<"  Enter new price: $";
                    cin>>newPrice;

                    Product* p = productRepo.get(id);

                    if(p){
                        p->setPrice(newPrice);

                        // save updated stock
                        FileHandler::saveProducts(productRepo);
                        cout<<"  Price updated and saved!\n";
                    }else{
                        cout<<"  Product not found.\n";
                    }
                    break;
                }
                case 5: { 
                    int id, newStock;

                    cout<<"  Enter Product ID: ";
                    cin>>id;

                    cout<<"  Enter new stock: ";
                    cin>>newStock;

                    Product* p = productRepo.get(id);

                    if(p){
                        p->setStock(newStock);
                        FileHandler::saveProducts(productRepo);
                        cout<<"  Stock updated and saved!\n";
                    }else{
                        cout<<"  Product not found.\n";
                    }
                    break;
                }
                case 6: //system statistics
                    displayStatistics();
                    break;
                case 7: // reload data from files
                    cout<<"\n  Reloading data from files...\n";

                    // reset repositories before reloading
                    productRepo.clear();
                    userRepo.clear();

                    FileHandler::loadProducts(productRepo);
                    FileHandler::loadUsers(userRepo);

                    cout<<"  Data reloaded successfully!\n";
                    break;
                case 8:
                    admin->displayInfo(); 
                    break;
                case 9:
                    currentUser = nullptr;
                    return;
                default:
                    cout<<"  Invalid option.\n";
            }


        }
    }


    // handles the login process for admin users only
    bool adminLogin(){
        string username,password;
        cout<<"\n  === ADMIN LOGIN ===\n";
        cout<<"  Admin Username: ";
        cin>>username;
        cout<<"  Admin Password: ";
        cin>>password;

        // retrieve all users stored in the repository
        vector<User*> allUsers = userRepo.getAll();

        // search for a matching admin
        auto it = find_if(allUsers.begin(), allUsers.end(), [&](User* u) {return u->getUsername() == username && u->getPassword() == password && u->getRole() == "Admin";});

        if(it != allUsers.end()){
            currentUser = *it; // store logged in admin
            cout<<"\n  Admin login successful! Welcome, "<<currentUser->getUsername()<<"!\n";
            return true;
        }

        cout << "\n  Invalid admin credentials. Access denied.\n";
        return false;

    }

    // registers a new admin user
    void registerAdmin(){
        string username, password, email, adminKey;
        cout<<"\n  === REGISTER NEW ADMIN ===\n";

        // ask for the admin authorization key
        cout<<"  Admin Registration Key: ";
        cin>>adminKey;

        // security check
        // only users know the predefined key "ADMIN2025" can register as admins
        if(adminKey != "ADMIN2025"){
            cout<<"\n  Invalid registration key. Admin registration denied.\n";
            return; // stop the registration process
        }

        cout<<"  Username: ";
        cin>>username;
        cout<<"  Password: ";
        cin>>password;
        cout<<"  Email: ";
        cin>>email;

        int newId = userRepo.size() + 1; // generate a unique id for the new admin
        userRepo.add(newId, new Admin(username, password, email)); // add new admin to repository

        // save updated user list to file
        FileHandler::saveUsers(userRepo);

        cout << "\n  Admin registration successful! You can now login.\n";

    }


    // main control loop for the entire E-Commerce Management System
    // displays the main menu and continuously processes user choices
    // connects all major system functionalities
    void run(){
        displayWelcome();

        while (true){
            cout<<"\n  ========================================\n";
            cout<<"  === MAIN MENU ===\n";
            cout<<"  ========================================\n";
            cout<<"  1. Customer Login\n";
            cout<<"  2. Customer Registration\n";
            cout<<"  3. Admin Login\n";
            cout<<"  4. Admin Registration\n";
            cout<<"  5. View System Info\n";
            cout<<"  6. Exit\n";
            cout<<"  ========================================\n";
            cout<<"  Choose option: ";

            int choice;
            cin>>choice;

            switch(choice){
                case 1: // customer login
                    if(login()){
                        if(currentUser->getRole() == "Customer"){
                            customerMenu();
                        }else{
                            cout<<"  This account is not a customer account.\n";
                            currentUser = nullptr; // reset user
                        }
                    }
                    break;
                case 2:
                    registerUser();
                    break;
                case 3:
                    if(adminLogin()){
                        adminMenu();
                    }
                    break;
                case 4:
                    registerAdmin();
                    break;
                case 5:
                    displayStatistics();
                    break;
                case 6:
                    cout<<"\n  Saving all data to files...\n";

                    // all updated data is saved
                    FileHandler::saveProducts(productRepo);
                    FileHandler::saveUsers(userRepo);

                    cout<<"\n  Thank you for using our E-Commerce System!\n\n";
                    return;
                default:
                    cout<<"  Invalid option.\n";
            }
        }
    }

};

int main(){
    ECommerceSystem system; 
    system.run();
    return 0;
}