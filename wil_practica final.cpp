#include <iostream>
#include <string>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

using namespace std;

// Capa de Acceso a Datos (Data Access Layer - DAL)
class EmpleadoDAL {
public:
    EmpleadoDAL(const string& connectionString) : connectionString(connectionString) {}

    void InsertarEmpleado(const string& nombre, const string& apellidos, float sueldoBruto, char categoria) {
        SQLHENV henv;
        SQLHDBC hdbc;
        SQLRETURN retcode;

        retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
        retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
        retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

        retcode = SQLDriverConnect(hdbc, NULL, (SQLCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

        SQLHSTMT hstmt;
        retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

        string query = "INSERT INTO Empleados (Nombre, Apellidos, SueldoBruto, Categoria) "
            "VALUES ('" + nombre + "', '" + apellidos + "', " + to_string(sueldoBruto) + ", '" + categoria + "')";

        retcode = SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        SQLDisconnect(hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
    }

private:
    string connectionString;
};

// Capa de Lógica de Negocio (Business Logic Layer - BLL)
class EmpleadoBLL {
public:
    float CalcularSueldoNeto(float sueldoBruto) {
        float porcentajeAumento;
        if (sueldoBruto <= 1000)
            porcentajeAumento = 0.1;
        else if (sueldoBruto <= 2000)
            porcentajeAumento = 0.2;
        else if (sueldoBruto <= 4000)
            porcentajeAumento = 0.3;
        else
            porcentajeAumento = 0.4;

        return sueldoBruto + (porcentajeAumento * sueldoBruto);
    }
};

// Capa de Presentación (Consola)
int main() {
    cout << "Registro de Empleados" << endl;
    cout << "---------------------" << endl;

    string nombre, apellidos;
    float sueldoBruto;
    char categoria;

    cout << "Ingrese el nombre del empleado: ";
    getline(cin, nombre);

    cout << "Ingrese los apellidos del empleado: ";
    getline(cin, apellidos);

    cout << "Ingrese el sueldo bruto del empleado: ";
    cin >> sueldoBruto;

    cout << "Ingrese la categoría del empleado (A/B/C): ";
    cin >> categoria;

    // Establecer la cadena de conexión a la base de datos SQL Server
    string connectionString = "DRIVER={SQL Server};SERVER=DESKTOP-OJCDQ8R\WIL;DATABASE=E_FINAL;UID=wilfv;PWD=12345";

    // Insertar empleado en la base de datos
    EmpleadoDAL empleadoDAL(connectionString);
    empleadoDAL.InsertarEmpleado(nombre, apellidos, sueldoBruto, categoria);

    // Calcular y mostrar sueldo neto
    EmpleadoBLL empleadoBLL;
    float sueldoNeto = empleadoBLL.CalcularSueldoNeto(sueldoBruto);
    cout << "Nombre: " << nombre << " " << apellidos << " - Sueldo Bruto: " << sueldoBruto << " - Sueldo Neto: " << sueldoNeto << endl;

    return 0;
}
