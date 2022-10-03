#include <iostream>
#include <libpq-fe.h>
using namespace std;

void CheckInfAboutCar(PGresult* res, int EnterId) {
    EnterId--;
    cout << "Id: " << PQgetvalue(res, EnterId, 0) << endl << "Brand: " << PQgetvalue(res, EnterId, 1) << endl << "Price: " << PQgetvalue(res, EnterId, 2) << "$" << endl;
}

void do_exit(PGconn* conn) {
    PQfinish(conn);
    exit(1);
}

void ShowAllCars(int& rows, PGresult* res) {
    cout << "All Cars" << endl;
    rows = PQntuples(res);

    for (int i = 0; i < rows; i++) {
        cout << PQgetvalue(res, i, 0) << " | " << PQgetvalue(res, i, 1) << endl;
    }
}

void CheckErrorConnect(PGconn* conn) {
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
        do_exit(conn);
    }
}

void CheckErrorResult(PGresult* res, PGconn* conn) {
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cout << "No data retrieved" << endl;
        PQclear(res);
        do_exit(conn);
    }
}

int main() {
    PGconn* conn = PQconnectdb("user=* dbname=* password=*"); //change for your db
    CheckErrorConnect(conn);

    PGresult* res = PQexec(conn, "SELECT * FROM cars");
    CheckErrorResult(res, conn);

    int rows;

Point:
    ShowAllCars(rows, res);
    cout << "Choise car(Enter id): ";
    int EnterId;
    cin >> EnterId;
    system("cls");
    if (EnterId <= 0 || EnterId > rows) {
        cout << "Error" << endl << "Enter any button...";
        system("pause>nul");
        system("cls");
        goto Point;
    }
    else {
        CheckInfAboutCar(res, EnterId);
        cout << endl << "Do you want to continue?" << endl << "1: Yes" << "  " << "2: No";
        int choise;
        cin >> choise;
        switch (choise) {
        case 1:
            system("cls");
            goto Point;
            break;
        case 2:
            system("cls");
            do_exit(conn);
        }
    }

    PQclear(res);
    PQfinish(conn);
}