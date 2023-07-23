/*
 * Copyright (C) 2023  The Anilab Development Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 */

#include <python/Python.h>

#include <cmd_lists.h>

void login(dpp::cluster& client, const dpp::slashcommand_t& event)
{
    Py_Initialize();

    PyObject* genshin = PyImport_ImportModule("genshin");
    if (genshin == NULL)
    {
        std::cout << "Failed to import genshin module" << std::endl;
        Py_Finalize();
        return;
    }

    PyObject* Client = PyObject_GetAttrString(genshin, "Client");
    if (Client == NULL || !PyCallable_Check(Client))
    {
        std::cout << "Failed to get Client class" << std::endl;
        Py_DECREF(genshin);
        Py_Finalize();
        return;
    }

    PyObject* clientInstance = PyObject_CallObject(Client, NULL);
    if (clientInstance == NULL)
    {
        std::cout << "Failed to create Client instance" << std::endl;
        Py_DECREF(Client);
        Py_DECREF(genshin);
        Py_Finalize();
        return;
    }

    PyObject* loginMethod = PyObject_GetAttrString(clientInstance, "login_with_password");
    if (loginMethod == NULL || !PyCallable_Check(loginMethod))
    {
        std::cout << "Failed to get login_with_password method" << std::endl;
        Py_DECREF(clientInstance);
        Py_DECREF(Client);
        Py_DECREF(genshin);
        Py_Finalize();
        return;
    }

    PyObject* args = PyTuple_Pack(2, PyUnicode_FromString("email"), PyUnicode_FromString("password"));
    if (args == NULL)
    {
        std::cout << "Failed to create arguments" << std::endl;
        Py_DECREF(loginMethod);
        Py_DECREF(clientInstance);
        Py_DECREF(Client);
        Py_DECREF(genshin);
        Py_Finalize();
        return;
    }

    PyObject* cookies = PyObject_CallObject(loginMethod, args);
    if (cookies != NULL)
    {
        const char* cookiesStr = PyUnicode_AsUTF8(cookies);
        std::cout << "Cookies: " << cookiesStr << std::endl;
    }
    else
        std::cout << "Login failed" << std::endl;

    Py_DECREF(cookies);
    Py_XDECREF(args);
    Py_DECREF(loginMethod);
    Py_DECREF(clientInstance);
    Py_DECREF(Client);
    Py_DECREF(genshin);

    Py_Finalize();
}