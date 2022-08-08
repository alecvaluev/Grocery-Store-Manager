#define MAX_ITEMS_IN_SALE 14
#define NUM_CATS 7

struct SalesRecord
{
	int id;
	double amount;
};

struct Record
{
	int amount;
	int category;
	double price;
	int byWeight;
	char name[25];
};

struct StockRecord 
{
	struct Record records;
	int id;
	double sold;
};

int readStockItems(struct StockRecord stock[], int maxStock, int stdIn);
int readSale(struct StockRecord stock[], int numStockItems, 
	struct SalesRecord saleItems[]);
void printStockReport(struct StockRecord* stock, int numStockItems);
double printSalesReport(struct StockRecord stock[], 
	struct SalesRecord saleItems[], int numSaleItems);
void centreText(int length, char sign, char text[]);
void topSellersText(int length, char sign, int num, char cat[]);
void getTopSellers(struct StockRecord stock[], int numStockItems, 
	struct SalesRecord topSellers[], int num, int cat);
void printTopSellers(struct StockRecord stock[], 
	struct SalesRecord topSellers[], int num, int cat);
void bubbleSortByFirst(int ids[], double numbers[], int numStockItems);