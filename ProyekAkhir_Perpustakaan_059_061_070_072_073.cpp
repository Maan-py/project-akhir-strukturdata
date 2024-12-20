#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>

using namespace std;

#define TABLE_SIZE 100

struct Book
{
  string title;
  string author;
  int referenceNumber;
  Book *next;     
  bool isDeleted; 
};

struct TreeNode
{
  string title;
  string author;
  int referenceNumber;
  TreeNode *left;
  TreeNode *right;
};

Book *hashTable[TABLE_SIZE];     
TreeNode *root = nullptr;        
TreeNode *deletedRoot = nullptr; 

// HASH
int hashFunction(int referenceNumber)
{
  return referenceNumber % TABLE_SIZE;
}

// CEK APAKAH NOMOR REFERENSI SUDAH ADA
bool isReferenceNumberExists(int referenceNumber)
{
  int index = hashFunction(referenceNumber);
  Book *temp = hashTable[index];
  while (temp)
  {
    if (temp->referenceNumber == referenceNumber && !temp->isDeleted)
    {
      return true;
    }
    temp = temp->next;
  }
  return false;
}

// SISIPKAN BUKU KE HASH TABLE
void insertBookHash(const string &title, const string &author, int referenceNumber)
{
  int index = hashFunction(referenceNumber);
  Book *newBook = new Book{title, author, referenceNumber, nullptr, false};
  if (!hashTable[index])
  {
    hashTable[index] = newBook;
  }
  else
  {
    Book *temp = hashTable[index];
    while (temp->next)
    {
      temp = temp->next;
    }
    temp->next = newBook;
  }
}

// TAMPILKAN BUKU DI HASH TABLE
void displayBooksHash()
{
  cout << "\nBooks in the library (Hash Table):\n";
  for (int i = 0; i < TABLE_SIZE; i++)
  {
    if (hashTable[i])
    {
      Book *temp = hashTable[i];
      while (temp)
      {
        if (!temp->isDeleted)
        {
          cout << "Reference: " << temp->referenceNumber << ", Title: " << temp->title << ", Author: " << temp->author << endl;
        }
        temp = temp->next;
      }
    }
  }
}

// MENCARI BUKU BERDASARKAN NOMOR REFERENSI DI HASH TABLE
void searchBookByReference(int referenceNumber)
{
  int index = hashFunction(referenceNumber);
  Book *temp = hashTable[index];
  while (temp)
  {
    if (temp->referenceNumber == referenceNumber && !temp->isDeleted)
    {
      cout << "\nBook found: Reference: " << temp->referenceNumber << ", Title: " << temp->title << ", Author: " << temp->author << endl;
      return;
    }
    temp = temp->next;
  }
  cout << "\nBook not found with Reference Number: " << referenceNumber << endl;
}

// MENCARI BUKU BERDASARKAN JUDUL DI HASH TABLE
void searchBookByTitle(const string &title)
{
  for (int i = 0; i < TABLE_SIZE; i++)
  {
    Book *temp = hashTable[i];
    while (temp)
    {
      if (temp->title == title && !temp->isDeleted)
      {
        cout << "\nBook found: Reference: " << temp->referenceNumber << ", Title: " << temp->title << ", Author: " << temp->author << endl;
        return;
      }
      temp = temp->next;
    }
  }
  cout << "\nBook not found with Title: " << title << endl;
}

// SISIPKAN BUKU KE BST
TreeNode *insertBookBST(TreeNode *node, const string &title, const string &author, int referenceNumber)
{
  if (!node)
    return new TreeNode{title, author, referenceNumber, nullptr, nullptr};
  if (title < node->title)
    node->left = insertBookBST(node->left, title, author, referenceNumber);
  else if (title > node->title)
    node->right = insertBookBST(node->right, title, author, referenceNumber);
  return node;
}

// HAPUS BUKU DARI BST
TreeNode *deleteBookBST(TreeNode *node, int referenceNumber)
{
  if (!node)
    return node;
  if (referenceNumber < node->referenceNumber)
    node->left = deleteBookBST(node->left, referenceNumber);
  else if (referenceNumber > node->referenceNumber)
    node->right = deleteBookBST(node->right, referenceNumber);
  else
  {
    if (!node->left)
    {
      TreeNode *temp = node->right;
      delete node;
      return temp;
    }
    else if (!node->right)
    {
      TreeNode *temp = node->left;
      delete node;
      return temp;
    }
  }
  return node;
}

// HAPUS HASH TABLE ENTRY
void deleteBook(int referenceNumber)
{
  int index = hashFunction(referenceNumber);
  Book *temp = hashTable[index];
  while (temp)
  {
    if (temp->referenceNumber == referenceNumber && !temp->isDeleted)
    {
      temp->isDeleted = true;                                                                     // Tandai buku sebagai dihapus
      deletedRoot = insertBookBST(deletedRoot, temp->title, temp->author, temp->referenceNumber); // Simpan buku yang dihapus ke BST history
      root = deleteBookBST(root, referenceNumber);                                                // Hapus buku dari BST
      cout << "\nBook with Reference Number " << referenceNumber << " deleted.\n";
      return;
    }
    temp = temp->next;
  }
  cout << "\nBook not found with Reference Number: " << referenceNumber << endl;
}

// PENGURUTAN INORDER TRAVERSAL
void inOrderTraversal(TreeNode *node)
{
  if (!node)
    return;
  inOrderTraversal(node->left);
  cout << "Reference: " << node->referenceNumber << ", Title: " << node->title << ", Author: " << node->author << endl;
  inOrderTraversal(node->right);
}

// PENGURUTAN PREORDER TRAVERSAL
void preOrderTraversal(TreeNode *node)
{
  if (!node)
    return;
  cout << "Reference: " << node->referenceNumber << ", Title: " << node->title << ", Author: " << node->author << endl;
  preOrderTraversal(node->left);
  preOrderTraversal(node->right);
}

// PENGURUTAN POSTORDER TRAVERSAL
void postOrderTraversal(TreeNode *node)
{
  if (!node)
    return;
  postOrderTraversal(node->left);
  postOrderTraversal(node->right);
  cout << "Reference: " << node->referenceNumber << ", Title: " << node->title << ", Author: " << node->author << endl;
}

// MENCARI BUKU BERDASARKAN JUDUL DI BST
void rangeSearch(TreeNode *node, const string &start, const string &end)
{
  if (node == nullptr)
    return;

  if (node->title > start)
    rangeSearch(node->left, start, end);

  if (node->title >= start && node->title <= end)
  {
    cout << "Reference: " << node->referenceNumber << ", Title: " << node->title << ", Author: " << node->author << endl;
  }

  if (node->title < end)
    rangeSearch(node->right, start, end);
}

int main()
{
  int choice;
  string title, author;
  int referenceNumber;
  system("cls");

  do
  {
    system("cls");
    cout << "...............................................\n";
    cout << "| Library Management System                   |\n";
    cout << "...............................................\n";
    cout << "| 1. Add Book                                 |\n";
    cout << "| 2. Delete Book Data                         |\n";
    cout << "| 3. Display Books                            |\n";
    cout << "| 4. Search Book By Code                      |\n";
    cout << "| 5. Search Book By Title Range               |\n";
    cout << "| 6. Sort Books By Title                      |\n";
    cout << "| 7. View History                             |\n";
    cout << "| 8. About Us                                 |\n";
    cout << "| 9. Exit                                     |\n";
    cout << "...............................................\n";
    cout << ">> Enter Choice: ";
    cin >> choice;
    system("CLS");

    switch (choice)
    {
    case 1:
      cout << "........................................\n";
      cout << "                INPUT BOOK              \n";
      cout << "........................................\n";
      cout << "Enter title: ";
      cin.ignore();
      getline(cin, title);
      cout << "Enter author: ";
      getline(cin, author);
      cout << "Enter reference number: ";
      cin >> referenceNumber;
      if (isReferenceNumberExists(referenceNumber))
      {
        cout << "Reference number already exists. Please use a unique reference number.\n";
      }
      else
      {
        insertBookHash(title, author, referenceNumber);
        root = insertBookBST(root, title, author, referenceNumber);
        cout << "Book added successfully.\n";
      }
      break;
    case 2:
      cout << "........................................\n";
      cout << "       DELETE BOOK (Reference Code)     \n";
      cout << "........................................\n";
      cout << "Enter reference number to delete: ";
      cin >> referenceNumber;
      deleteBook(referenceNumber);
      break;
    case 3:
      cout << "........................................\n";
      cout << "              DISPLAY BOOKS             \n";
      cout << "........................................\n";
      displayBooksHash();
      break;
    case 4:
      cout << "........................................\n";
      cout << "    SEARCH BOOK (By Reference Code)     \n";
      cout << "........................................\n";
      cout << "Enter reference number to search: ";
      cin >> referenceNumber;
      searchBookByReference(referenceNumber);
      break;
    case 5:
    {
      cout << "........................................\n";
      cout << "      SEARCH BOOK (By Title Range)      \n";
      cout << "........................................\n";
      string start, end;
      cout << "Enter start of range: ";
      cin.ignore();
      getline(cin, start);
      cout << "Enter end of range: ";
      getline(cin, end);
      cout << "Books in range [" << start << "-" << end << "]:\n";
      rangeSearch(root, start, end);
      break;
    }
    case 6:
      cout << "........................................\n";
      cout << "        SORTING BOOKS (By Title)        \n";
      cout << "........................................\n";
      cout << "Books in sorted order:\n";
      inOrderTraversal(root);
      break;
    case 7:
      cout << "........................................\n";
      cout << "       VIEW DELETED HISTORY             \n";
      cout << "........................................\n";
      cout << "Deleted Books (In-Order):\n";
      inOrderTraversal(deletedRoot);
      cout << "\nDeleted Books (Pre-Order):\n";
      preOrderTraversal(deletedRoot);
      cout << "\nDeleted Books (Post-Order):\n";
      postOrderTraversal(deletedRoot);
      break;
    case 8:
      cout << "..................................................\n";
      cout << ".                   OUR GROUP                    .\n";
      cout << "..................................................\n";
      cout << ". Bintoro                            (123230059) .\n";
      cout << ". Rafi Pradipa Adriano               (123230061) .\n";
      cout << ". Muhammad Luqmaan                   (123230070) .\n";
      cout << ". Muhammad Raihan Syamaidzar         (123230072) .\n";
      cout << ". Ekin Ananda Putra                  (123230073) .\n";
      cout << "..................................................\n";
      cout << "Press Anywhere To Continue...";
      break;
      break;
    case 9:
      cout << "You Have Exited The Program\n";
      break;
    default:
      cout << "Invalid choice. Please try again.\n";
      break;
    }

    cout << "\nPress any key to continue...";
    getch();
  } while (choice != 9);

  return 0;
}
