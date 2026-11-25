#include <stdio.h>
#include <string.h>

#define MAX_PASSENGERS 100
#define TRAIN_SEATS 5

struct Passenger {
    int pnr;
    char name[50];
    int seatNo;
    char status[15];
};

struct Passenger records[MAX_PASSENGERS];
int count = 0;

int getConfirmedCount(void) {
    int i, total = 0;
    for (i = 0; i < count; i++) {
        if (strcmp(records[i].status, "CONFIRMED") == 0)
            total++;
    }
    return total;
}

int findEarliestWaiting(void) {
    int i, index = -1;
    for (i = 0; i < count; i++) {
        if (strcmp(records[i].status, "WAITING") == 0) {
            if (index == -1 || records[i].pnr < records[index].pnr)
                index = i;
        }
    }
    return index;
}

void bookTicket(void) {
    int i, seatUsed[TRAIN_SEATS + 1] = {0};
    int seat = 0;

    if (count >= MAX_PASSENGERS) {
        printf("\nBooking record limit reached.\n");
        return;
    }

    printf("Enter passenger name: ");
    scanf(" %49[^\n]", records[count].name);

    records[count].pnr = 1001 + count;
    records[count].seatNo = 0;

    for (i = 0; i < count; i++) {
        if (strcmp(records[i].status, "CONFIRMED") == 0)
            seatUsed[records[i].seatNo] = 1;
    }

    if (getConfirmedCount() < TRAIN_SEATS) {
        for (i = 1; i <= TRAIN_SEATS; i++) {
            if (!seatUsed[i]) {
                seat = i;
                break;
            }
        }
        records[count].seatNo = seat;
        strcpy(records[count].status, "CONFIRMED");
        printf("\nBooking confirmed! PNR: %d, Seat: %d\n",
               records[count].pnr, seat);
    } else {
        strcpy(records[count].status, "WAITING");
        printf("\nAdded to waiting list. PNR: %d\n",
               records[count].pnr);
    }
    count++;
}

void cancelTicket(void) {
    int pnr, i, releasedSeat, waitingIndex;

    printf("Enter PNR to cancel: ");
    scanf("%d", &pnr);

    for (i = 0; i < count; i++) {
        if (records[i].pnr == pnr)
            break;
    }

    if (i == count) {
        printf("\nInvalid PNR.\n");
        return;
    }

    if (strcmp(records[i].status, "CANCELLED") == 0) {
        printf("\nThis booking is already cancelled.\n");
        return;
    }
if (strcmp(records[i].status, "WAITING") == 0) {
        strcpy(records[i].status, "CANCELLED");
        printf("\nWaiting-list booking cancelled.\n");
        return;
    }

    releasedSeat = records[i].seatNo;
    records[i].seatNo = 0;
    strcpy(records[i].status, "CANCELLED");
    printf("\nConfirmed booking cancelled.\n");

    waitingIndex = findEarliestWaiting();
    if (waitingIndex != -1) {
        records[waitingIndex].seatNo = releasedSeat;
        strcpy(records[waitingIndex].status, "CONFIRMED");
        printf("PNR %d promoted from waiting list. Seat: %d\n",
               records[waitingIndex].pnr, releasedSeat);
    }
}

void displayBookings(void) {
    int i, waitingPosition;

    if (count == 0) {
        printf("\nNo booking records available.\n");
        return;
    }

    printf("\n%-8s %-20s %-8s %-12s %-8s\n",
           "PNR", "NAME", "SEAT", "STATUS", "WL POS");
    printf("----------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        waitingPosition = 0;

        if (strcmp(records[i].status, "WAITING") == 0) {
            int j;
            for (j = 0; j < count; j++) {
                if (strcmp(records[j].status, "WAITING") == 0 &&
                    records[j].pnr <= records[i].pnr)
                    waitingPosition++;
            }
        }

        if (strcmp(records[i].status, "WAITING") == 0) {
            printf("%-8d %-20s %-8s %-12s %-8d\n",
                   records[i].pnr, records[i].name, "--",
                   records[i].status, waitingPosition);
        } else if (strcmp(records[i].status, "CONFIRMED") == 0) {
            printf("%-8d %-20s %-8d %-12s %-8s\n",
                   records[i].pnr, records[i].name,
                   records[i].seatNo, records[i].status, "--");
        } else {
            printf("%-8d %-20s %-8s %-12s %-8s\n",
                   records[i].pnr, records[i].name, "--",
                   records[i].status, "--");
        }
    }
}

int main(void) {
    int choice;

    do {
        printf("\n===== RAILWAY SEAT WAITING-LIST SIMULATOR =====\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Display Bookings\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input. Program terminated.\n");
            return 0;
        }

        switch (choice) {
            case 1:
                bookTicket();
                break;
            case 2:
                cancelTicket();
                break;
            case 3:
                displayBookings();
                break;
            case 4:
                printf("\nThank you for using the simulator.\n");
                break;
            default:
                printf("\nInvalid choice. Try again.\n");
        }
    } while (choice != 4);

    return 0;
}