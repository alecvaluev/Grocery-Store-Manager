#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "stock.h"

char categories[7][14] = { "produce", "bakery", "meat", "dairy", "baking", 
"house wares", "miscellaneous" };

int readStockItems(struct StockRecord stock[], int maxStock, int stdIn) {
	int i = 0;
	//manual input
	if (stdIn == 1) {

		for (i = 0; i < maxStock; i++) {
			scanf("%d,%d,%lf,%d,%25[^'\n']%*c", &stock[i].records.amount, 
				&stock[i].records.category, &stock[i].records.price, 
				&stock[i].records.byWeight, &stock[i].records.name);
			//exit loop if 0 entered
			if (stock[i].records.amount == 0) break;

			//check inputs
			do {
				if (stock[i].records.category < 1 || 
					stock[i].records.category > 7) {
					printf("Invalid Category - Enter a number between 1 and\
 %d: ", NUM_CATS);
					scanf("%d,%lf,%d,%25[^'\n']", &stock[i].records.category, 
						&stock[i].records.price, &stock[i].records.byWeight, 
						&stock[i].records.name);
				}
				else if (stock[i].records.byWeight < 0 || 
					stock[i].records.byWeight > 1) {
					printf("Invalid soldByWeight - Enter a number between\
 0 and 1: ");
					scanf("%d,%25[^'\n']", &stock[i].records.byWeight, 
						&stock[i].records.name);
				}
			} while (stock[i].records.category < 1 || 
				stock[i].records.category > 7 || stock[i].records.byWeight < 0
				|| stock[i].records.byWeight > 1);
			
			//set the id number
			stock[i].id = i + 1;
		}
	}
	//reading data from a file
	else if (stdIn == 0) {
		FILE* file = NULL;
		file = fopen("db.txt", "r");

		if (file == NULL) {
			printf("Failed to open file.\n");
		}
		else {
			i = 0;
			while(!feof(file)) {
				fscanf(file, "%d%[^,]%d%[^,]%lf%[^,]%d%[^,]%25[^'\n']", 
					&stock[i].records.amount, &stock[i].records.category,
					&stock[i].records.price, &stock[i].records.byWeight, 
					&stock[i].records.name);
				//set the id number
				stock[i].id = i + 1;
				i++;
				if (i == 15) break;
			}
			fclose(file);
		}
	}
	return i;
}

int readSale(struct StockRecord stock[], int numStockItems, 
	struct SalesRecord saleItems[]) {
	int i, itemId = 1, numSaleItems = 0;
	double quantity;

	for(i = 0; i < numStockItems; i++) {
		printf("Enter a product ID to purchase, and the quantity\
(0 to stop) : ");
		scanf("%d,%lf", &itemId, &quantity);
		//exit the function if id = 0
		if (itemId == 0) break;
		//check the values 
		while (itemId < 0 || itemId > numStockItems || quantity < 0.10 || 
			quantity > 100.00) {
			if (itemId < 0 || itemId > numStockItems) {
				printf("Invalid Product - Enter a number between 0 and %d: ",
					numStockItems);
				scanf("%d,%lf", &itemId, &quantity);
			}
			if (quantity < 0.10 || quantity > 100.00) {
				printf("Invalid quantity - Enter a number between 0.10 and\
 100.00: ");
				scanf("%d,%lf", &itemId, &quantity);
			}
			
		}
		//set the sales id/amount
		saleItems[i].id = stock[itemId - 1].id;
		if (quantity <= stock[itemId - 1].records.amount) {
			stock[itemId - 1].records.amount -= (int)quantity;
			stock[itemId - 1].sold += (int)quantity * 
				stock[itemId - 1].records.price;
			saleItems[i].amount = (int)quantity;
		} 
		else {
			saleItems[i].amount = stock[itemId - 1].records.amount;
			stock[itemId - 1].sold += stock[itemId - 1].records.amount * 
				stock[itemId - 1].records.price;
			stock[itemId - 1].records.amount = 0;
		}
	}
	return i;
}

void printStockReport(struct StockRecord* stock, int numStockItems) {
	int i;
	printf("%4s%31s%16s%8s%9s\n", "ID", "Product", "Category", "Price",
		"Quantity");

	for (i = 0; i < numStockItems; i++) {
		printf("%4d%31s%16s%8.2lf%9d\n", stock[i].id, stock[i].records.name,
			categories[stock[i].records.category - 1], stock[i].records.price,
			stock[i].records.amount);
	}
	return;
}

double printSalesReport(struct StockRecord stock[],
	struct SalesRecord saleItems[], int numSaleItems) {
	double sale = 0, tax = 0;
	int i, k;

	printf("\n");
	centreText(64, '*', " Seneca Groceries ");
	printf("\n");
	centreText(64, '=', "");
	printf("\n");

	for (i = 0; i < numSaleItems; i++) {
		k = saleItems[i].id - 1;

		printf("%30s%9.2lf%8.2lf\n", stock[k].records.name,
			stock[k].records.price, stock[k].records.price * saleItems[i].amount);

		sale += stock[k].records.price * saleItems[i].amount;
		if (stock[k].records.category == 6 || stock[k].records.category == 7) {
			tax += stock[k].records.price * saleItems[i].amount * 0.13;
		}
	}
	printf("Purchase Total%33.2lf\n", sale);
	printf("Tax%44.2lf\n", tax);
	printf("Total%42.2lf\n", sale + tax);
	printf("Thank you for shopping at Seneca!\n\n");

	return sale;
}

void centreText(int length, char sign, char text[]) {
	int i, line;

	if (length > (int)strlen(text)) line = length - (int)strlen(text);
	else line = 0;
	//print 1st half
    for (i = 0; i < line / 2; i++) printf("%c", sign);
	printf("%s", text);

	if (line % 2 == 0) line /= 2;
	else line = line / 2 + 1;
	//print 2nd half
	for (i = 0; i < line; i++) printf("%c", sign);
	return;
}

void topSellersText(int length, char sign, int num, char cat[]) {
	int i, line;
	line = length - (int)strlen(cat) - 19;
	//print 1st half
	for (i = 0; i < line / 2; i++) printf("%c", sign);
	printf(" Top %d sellers in %s ", num, cat);
	
	if (line % 2 == 0) line /= 2;
	else line = line / 2 + 1;
	//print 2nd half
	for (i = 0; i < line; i++) printf("%c", sign);
	return;
}

void getTopSellers(struct StockRecord stock[], int numStockItems,
	struct SalesRecord topSellers[], int num, int cat) {
	int ids[MAX_ITEMS_IN_SALE], i, j;
	double numbers[MAX_ITEMS_IN_SALE];

	for (i = 0; i < numStockItems; i++) {//copy id/amount to arrays
		ids[i] = stock[i].id;
		numbers[i] = stock[i].sold / stock[i].records.price;
	}
	//sort id and numbers by id
	bubbleSortByFirst(&ids, &numbers, numStockItems);
	
	//copy data to the topSellers
	i = 0;
	for(j = 0; j < numStockItems; j++){
		if (stock[ids[j] - 1].records.category == cat + 1) {
			topSellers[i].amount = numbers[j];
			topSellers[i].id = ids[j];
			i++;
			if (i == num) break;
		}
    }
	return;
}

void bubbleSortByFirst(int ids[], double numbers[], int numStockItems) {
	int i, j, tempId;
	double tempNum;

	for (i = 0; i < numStockItems - 1; i++) {    
		for (j = 0; j < numStockItems - i - 1; j++) {
			if (numbers[j] < numbers[j + 1]) {
				tempId = ids[j];
				tempNum = numbers[j];

				ids[j] = ids[j + 1];
				numbers[j] = numbers[j + 1];

				ids[j + 1] = tempId;
				numbers[j + 1] = tempNum;
			}
		}
	}
	return;
}

void printTopSellers(struct StockRecord stock[], 
	struct SalesRecord topSellers[], int num, int cat) {
	int i;
	topSellersText(50, '-', num, categories[cat]);
	printf("\n");
	printf("%s%30s%8s\n", "Rank", "Product", "Sales");

	for (i = 0; i < num; i++) {
		if (topSellers[i].amount > 0.0) {
			int k = topSellers[i].id - 1;
			printf("%4d%30s%8.2lf\n", i + 1, stock[k].records.name, 
				topSellers[i].amount);
		}
		else printf("%4d%30s%8.2lf\n", i + 1, "<none>", 0.0);

		topSellers[i].id = 0; // make them empty 
		topSellers[i].amount = 0.0;
	}
	return;
}