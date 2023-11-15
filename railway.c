#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 4

typedef struct TRAIN
{
	int total_seats;
	struct TRAIN *next;
} train;

train *createtrain(char trainname[], int total_seats)
{
	train *newtrain;
	newtrain = (train *)malloc(sizeof(train));
	newtrain->total_seats = total_seats;
	newtrain->next = NULL;
	return newtrain;
}

train *availabletrains(train *t, char trainname[], int total_seats)
{
	train *newtrain;
	train *aux = t;
	newtrain = createtrain(trainname, total_seats);

	if (t == NULL)
	{
		return newtrain;
	}
	else
	{
		while (aux->next != NULL)
			aux = aux->next;
		aux->next = newtrain;
		return t;
	}
}

typedef struct NODE
{
	int reg_no;
	int age;
	char name[20];
	char berth_type[10]; // "Lower" or "Upper" berth
	struct NODE *next;
} node;

node *start = NULL;
int lower_berth_available = 0;
int upper_berth_available = 0;
int current_reg_number = 0;

node *deq();
node *create();
int reserve();
void display();
int check_availability();
int resetSeat(char *, node *);

node *create()
{
	node *new;
	int choice;
	new = (node *)malloc(sizeof(node));
	printf("Name: ");
	scanf("%s", new->name);
	printf("Age : ");
	scanf("%d", &new->age);
	new->reg_no = current_reg_number; // Increment the registration number
	new->next = NULL;
	return new;
}

int resetSeat(char *berthType, node *new_node)
{
	node *empty;
	empty = start;
	while ((strcmp(empty->name, "empty") != 0) || (strcmp(empty->berth_type, berthType) != 0))
	{
		empty = empty->next;
	}
	strcpy(empty->name, new_node->name);
	empty->age = new_node->age;
	strcpy(empty->berth_type, new_node->berth_type);
	return empty->reg_no;
}

int reserve()
{
	node *temp, *new_node;
	int choice, regno;
	new_node = create();
	if (new_node->age >= 90 || new_node->age <= 10)
	{
		free(new_node);
		return -1;
	}
	if (new_node->age >= 60)
	{
		if (lower_berth_available < 2)
		{
			++lower_berth_available;
			strcpy(new_node->berth_type, "Lower");
			if (current_reg_number < (lower_berth_available + upper_berth_available))
			{
				++current_reg_number;
				regno = current_reg_number;
				new_node->reg_no = current_reg_number;
			}
			else
			{
				return resetSeat("Lower", new_node);
			}
		}
		else
		{
			if (upper_berth_available < 2)
			{
				printf("seats are not available in lowerberth\nDo you want to book upper berthseats?\n1.YES\t2.NO\t");
				scanf("%d", &choice);
				if (choice == 1)
				{
					++upper_berth_available;
					strcpy(new_node->berth_type, "Upper");
					if (current_reg_number < (lower_berth_available + upper_berth_available))
					{
						++current_reg_number;
						new_node->reg_no = current_reg_number;
					}
					else
					{
						return resetSeat("Upper", new_node);
					}
				}
				else
				{
					return -2;
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		if (upper_berth_available < 2)
		{
			++upper_berth_available;
			strcpy(new_node->berth_type, "Upper");
			if (current_reg_number < (lower_berth_available + upper_berth_available))
			{
				++current_reg_number;
				new_node->reg_no = current_reg_number;
			}
			else
			{
				return resetSeat("Upper", new_node);
			}
		}
		else
		{
			if (lower_berth_available < 2)
			{
				++lower_berth_available;
				strcpy(new_node->berth_type, "Lower");
				if (current_reg_number < (lower_berth_available + upper_berth_available))
				{
					++current_reg_number;
					new_node->reg_no = current_reg_number;
				}
				else
				{
					return resetSeat("Lower", new_node);
				}
			}
			else
			{
				return 0;
			}
		}
	}
	if (start == NULL)
	{
		start = new_node;
		temp = start;
	}
	else
	{
		temp = start;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new_node;
	}
	return new_node->reg_no;
}

void cancel(int regNo)
{
	node *temp = start;
	int found = 0;

	while (temp != NULL)
	{
		if (temp->reg_no == regNo)
		{

			strcpy(temp->name, "empty");
			temp->age = 18;
			printf("Details updated successfully for Registration Number %d.\n", regNo);
			found = 1;
			if (strcmp(temp->berth_type, "Upper") == 0)
			{
				--upper_berth_available;
			}
			else
			{
				--lower_berth_available;
			}
			break;
		}
		temp = temp->next;
	}

	if (!found)
	{
		printf("No booking found with Registration Number %d.\n", regNo);
	}
}

void display()
{
	int bookingsExist = 0; // Flag to check if bookings are present

	if (start == NULL)
	{
		printf("No bookings available to display.\n");
		return;
	}

	node *temp = start;
	while (temp != NULL)
	{
		if (strcmp(temp->berth_type, "Lower") == 0 || strcmp(temp->berth_type, "Upper") == 0)
		{
			bookingsExist = 1; // Set flag to indicate bookings are present
			printf("\nRegistration Number: %d\n", temp->reg_no);
			printf("Name : %s\n", temp->name);
			printf("Age : %d\n", temp->age);
			printf("Berth Type: %s\n", temp->berth_type);
			printf("Train name: ChennaiExpress\n");
		}
		temp = temp->next;
	}

	if (!bookingsExist)
	{
		printf("No bookings available to display.\n");
	}
}

int check_availability()
{
	return MAX - (lower_berth_available + upper_berth_available);
}

int main()
{
	int ch, ch1, status = 0, canc = 0, reg = 0, regno, checkAvailability;
	char trainname[20];
	do
	{
		printf("\t\t\t*RAILWAY RESERVATION*\t\t\t\n");
		printf("\nTrain:ChennaiExpress\n");
		train *head = NULL;
		head = availabletrains(head, "chennaiexpress", 6);
		printf("\n\nDo you want to - \n 1. Book a ticket \n 2. cancel \n 3. Display passenger details \n 4.check avilability \n 5. exit\n\n");
		scanf("%d", &ch);
		switch (ch)
		{
		case 1:
			status = reserve(trainname);
			if (status == 0)
			{
				printf("\nSeats in both upper and lower berths are fully booked. Cannot make a reservation.\n\n");
			}
			else if (status == -1)
			{
				printf("\nAge not eligible!\n");
			}
			else if (status == -2)
			{
				printf("Booking Cancelled.\n\n");
			}

			else
			{
				printf("\nBooking Successful!!! Enjoy your journey! Your Reg No is %d\n\n", status);
			}
			break;

		case 2:
			printf("Enter your Registration Number: ");
			scanf("%d", &reg);
			cancel(reg);
			break;
		case 3:
			display();
			break;
		case 4:

			checkAvailability = check_availability();
			printf("Available: %d\n", checkAvailability);
			if (checkAvailability > 0 && checkAvailability <= MAX)
			{
				printf("Do you want to proceed with the booking 1.YES 2.NO\t");
				scanf("%d", &ch1);
				if (ch1 == 1)
				{
					status = reserve(trainname);
					if (status == 0)
					{
						printf("\nSeats in both upper and lower berths are fully booked. Cannot make a reservation.\n\n");
					}
					else if (status == -1)
					{
						printf("\nAge not eligible!\n");
					}
					else if (status == -2)
					{
						printf("Booking Cancelled.\n\n");
					}

					else
					{
						printf("\nBooking Successful!!! Enjoy your journey! Your Reg No is %d\n\n", status);
					}
				}
			}
			break;
		}
	} while (ch != 5);
	return 0;
}
