#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <conio.h>

using namespace std;

struct Medicine {
    string name;
    string company;
    double price;
    string expireDate;
    int inStock;
    vector<string> similarMedicines;
};

vector<Medicine> medicines;

void saveDataToFile() {
    ofstream file("D://medicines.txt");
    if (file.is_open()) {
        for (const auto& med : medicines) {
            file << med.name << "," << med.company << "," << med.price << "," << med.expireDate << "," << med.inStock << ",";
            for (const auto& simMed : med.similarMedicines) {
                file << simMed << ";";
            }
            file << "\n";
        }
        file.close();
    } 
	else 
	{
        cout << "Unable to open file for saving data." << endl;
        exit(0);
    }
}

void loadDataFromFile() {
    ifstream file("D://medicines.txt");
    if (file.is_open()) 
	{
        medicines.clear();
        string line;
        while (getline(file, line))
		{
            Medicine med;
            size_t pos = 0;
            pos = line.find(",");
            med.name = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(",");
            med.company = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(",");
            med.price = stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(",");
            med.expireDate = line.substr(0, pos);
            line.erase(0, pos + 1);
            pos = line.find(",");
            med.inStock = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(";");
            while (pos != string::npos) 
			{
                med.similarMedicines.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
                pos = line.find(";");
            }
            medicines.push_back(med);
        }
        file.close();
    }
	else 
	{
        cout << "Unable to open file for loading data." << endl;
    	exit(0);
	}
}

void addMedicine() {
    Medicine med;
    cout << "Enter Medicine Name: ";
    cin.ignore();
    getline(cin, med.name);
    cout << "Enter Company Name: ";
    getline(cin, med.company);
    cout << "Enter Price: ";
    cin >> med.price;
    cout << "Enter Expire Date (YYYY-MM-DD): ";
    cin >> med.expireDate;
    cout << "Enter In Stock Count: ";
    cin >> med.inStock;

    cout << "Enter Similar Medicines (comma-separated, enter 'done' when finished): ";
    string simMedicine;
    while (cin >> simMedicine && simMedicine != "done") {
        med.similarMedicines.push_back(simMedicine);
    }

    medicines.push_back(med);
    saveDataToFile();
    cout <<endl<<"Medicine added successfully!" << endl;
    
    cout<<endl<<"Press any key to get back to Menu...";
    getch();
}

void editMedicine() {
    string medName;
    cout << "Enter Medicine Name to Edit: ";
    cin.ignore();
    getline(cin, medName);

    auto it = find_if(medicines.begin(), medicines.end(), [&](const Medicine& med) {
        return med.name == medName;
    });

    if (it != medicines.end()) 
	{
        cout << "Enter New Price: ";
        cin >> it->price;
        cout << "Enter New Expire Date (YYYY-MM-DD): ";
        cin >> it->expireDate;
        cout << "Enter New In Stock Count: ";
        cin >> it->inStock;

        cout << "Enter Similar Medicines (comma-separated, enter 'done' when finished): ";
        string simMedicine;
        it->similarMedicines.clear();
        while (cin >> simMedicine && simMedicine != "done") {
            it->similarMedicines.push_back(simMedicine);
        }

        saveDataToFile();
        cout<<endl<<"Medicine edited successfully!" << endl;
    } 
	else 
	{
        cout<<endl<<"Medicine not found." << endl;
    }
    
    cout<<endl<<"Press any key to get back to Menu...";
    getch();
}

void sellMedicine()
{
    string medName;
    int quantity;
    cout << "Enter Medicine Name to Sell: ";
    cin.ignore();
    getline(cin, medName);
    cout << "Enter Quantity: ";
    cin >> quantity;

    auto it = find_if(medicines.begin(), medicines.end(), [&](const Medicine& med) {
        return med.name == medName;
    });

    if (it != medicines.end() && it->inStock >= quantity) {
        it->inStock -= quantity;
        saveDataToFile();
        cout <<endl<< "Medicine sold successfully!" << endl;
    } else if (it == medicines.end()) {
        cout <<endl<<"Medicine not found." << endl;
    } else {
        cout <<endl<<"Not enough stock available." << endl;
    }
    
    cout<<endl<<"Press any key to get back to Menu...";
    getch();
    
}

void displayExpiringMedicines() {
    time_t now = time(0);
    tm* currentDate = localtime(&now);
    currentDate->tm_mday += 7;
    mktime(currentDate);

    cout << "Medicines expiring in the next week:" << endl;
    for (const auto& med : medicines)
	{
        tm expireDate;
        sscanf(med.expireDate.c_str(), "%d-%d-%d", &expireDate.tm_year, &expireDate.tm_mon, &expireDate.tm_mday);
        expireDate.tm_year -= 1900;
        expireDate.tm_mon -= 1;

        if (difftime(mktime(&expireDate), now) > 7 * 24 * 60 * 60)
		{
            cout << "Name: " << med.name << ", In Stock: " << med.inStock << endl;
        }
    }
    
    cout<<endl<<"Press any key to get back to Menu...";
    getch();
}

void searchMedicine()
{
    string searchTerm;
    cout << "Enter Medicine Name or Company Name to Search: ";
    cin.ignore();
    getline(cin, searchTerm);

    cout << "Search results:" << endl;
    for (const auto& med : medicines)
	{
        if (med.name.find(searchTerm) != string::npos || med.company.find(searchTerm) != string::npos) {
            cout << "Name: " << med.name << ", Company: " << med.company << ", In Stock: " << med.inStock << endl;
        }
    }
    
    cout<<endl<<"Press any key to get back to Menu...";
    getch();
}

void deleteMedicine() {
    string medName;
    cout << "Enter Medicine Name to Delete: ";
    cin.ignore();
    getline(cin, medName);

    auto it = find_if(medicines.begin(), medicines.end(), [&](const Medicine& med) 
	{
        return med.name == medName;
    });

    if (it != medicines.end()) 
	{
        medicines.erase(it);
        saveDataToFile();
        cout << "Medicine deleted successfully!" << endl;
    } 
	else 
	{
        cout << "Medicine not found." << endl;
    }
    
    cout<<endl<<"Press any key to get back to Menu...";
    getch();
}

void displayOutOfStockMedicines() {
    cout << "Out of Stock Medicines:" << endl;
    for (const auto& med : medicines) {
        if (med.inStock == 0) {
            cout << "Name: " << med.name << ", Company: " << med.company << endl;
        }
    }
    
    cout<<endl<<"Press any key to get back to Menu...";
    getch();
}

int main() 
{
    loadDataFromFile();
    int choice;
    do 
	{
		system("cls");
		
        cout<< "- Pharmacy Management System Menu -"<<endl<<endl;
        cout<< "1. Add Medicine"<<endl;
    	cout<< "2. Edit Medicine"<<endl;
        cout<< "3. Sell Medicine"<<endl;
        cout<< "4. Expiring Medicines"<<endl;
        cout<< "5. Search Medicine"<<endl;
        cout<< "6. Delete Medicine"<<endl;
    	cout<< "7. Out of Stock Medicines"<<endl;
        cout<< "0. Exit"<<endl<<endl;
        cout<< "Enter your choice: ";
        cin >> choice;
		system("cls");
        switch (choice) 
		{
            case 1:
                addMedicine();
                break;
            case 2:
                editMedicine();
                break;
            case 3:
                sellMedicine();
                break;
            case 4:
                displayExpiringMedicines();
                break;
            case 5:
                searchMedicine();
                break;
            case 6:
                deleteMedicine();
                break;
            case 7:
                displayOutOfStockMedicines();
                break;
            case 0:
                saveDataToFile();
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 0);

    return 0;
}
