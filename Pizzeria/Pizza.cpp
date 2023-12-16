/*
Реализовать программу демонстрирующую предметную область "Пиццерия"

Заказ - это список из выбранных пользователем пицц.

Пицца - это блюдо, которое выбирает пользователь. Пицца обладает именем(текст), описанием(текст), 
размером (один из вариантов 25, 30, 35, 40, который выбирает пользователь), ценой(зависит от размера выбранной пиццы и ингредиентов). 
Пользователь может подсолить любую пиццу N-раз или добавить сыра M-раз - это увеличивает цену в зависимости от вида и размера пиццы 
(формулу придумать на ваше усмотрение)

Заказ - формируется пользователем через консоль, возможность добавить пиццу, распечатать заказ в консоль.

Печать заказа - вывод всего списка выбранных пользователем пицц, с указанием названия, описания, 
N-соли и M-сыра, размера пиццы и стоимости пиццы с учётом всех параметров

В конце распечатанного заказа вывести итоговую сумму за заказ

(В рамках выполнения задания необходимо продемонстрировать умение использовать Ad-hoc полиморфизм: overloading/overriding)
*/
#include <iostream>
#include <vector>

using namespace std;

class Pizza
{
protected:
    int saltCount;
    int cheezCount;

public:
    virtual ~Pizza() = default;

    virtual void outputOrder() const = 0;
    virtual float costCounter() const = 0;

    virtual void setSalt(int salt) {
        saltCount = salt;
    }

    virtual int getSalt() const {
        return saltCount;
    }

    virtual void setCheez(int cheez) {
        cheezCount = cheez;
    }

    virtual int getCheez() const {
        return cheezCount;
    }
};

class BasePizza : public Pizza
{
protected:
    string name;
    string description;
    unsigned int size;
    float cost;

public:
    BasePizza(string nameVal, string descriptionVal, unsigned int sizeVal)
        : name(nameVal), description(descriptionVal), size(sizeVal), cost(0) {}

    float costCounter() const override
    {
        const int cheezCost = 50;
        const int saltCost = 10;

        return cost + saltCost * saltCount + cheezCost * cheezCount;
    }

    void outputOrder() const override
    {
        cout << "Название: " << name << endl;
        cout << "Описание: " << description << endl;
        cout << "Размер: " << size << "см" << endl;
        cout << "Соль: " << saltCount << endl;
        cout << "Сыр: " << cheezCount << endl;
        cout << "Цена: " << costCounter() << "р" << endl;
    }
};

class Pipperoni : public BasePizza
{
public:
    Pipperoni(unsigned int size) : BasePizza("Пипперони", "Пипперони, сыр, чили, соус", size)
    {
        if (size == 25)
            cost = 300;
        else if (size == 30)
            cost = 400;
        else if (size == 35)
            cost = 550;
        else if (size == 40)
            cost = 700;
    }
};

class Margaritta : public BasePizza
{
public:
    Margaritta(unsigned int size) : BasePizza("Маргарита", "Сыр, помидоры, соус, базилик, трюфель", size)
    {
        if (size == 25)
            cost = 350;
        else if (size == 30)
            cost = 450;
        else if (size == 35)
            cost = 600;
        else if (size == 40)
            cost = 750;
    }
};

void displayMenu()
{
    cout << "Меню пицц:" << endl;
    cout << "-------------------------------------------------------------------------" << endl;
    cout << "1. Пицца Пипперони: 300р (25см) / 400р (30см) / 550р (35см) / 700р (40см)" << endl;
    cout << "2. Пицца Маргарита: 350р (25см) / 450р (30см) / 600р (35см) / 750р (40см)" << endl;
    cout << "-------------------------------------------------------------------------\n" << endl;
}

void displayOrder(const vector<Pizza*>& order)
{
    cout << "Заказ:" << endl;
    for (const auto& pizza : order)
    {
        pizza->outputOrder();
        cout << "----------------------------------" << endl;
    }

    float totalCost = 0;
    for (const auto& pizza : order)
    {
        totalCost += pizza->costCounter();
    }

    cout << "Итоговая сумма: " << totalCost << "р" << endl;
}

int main()
{
    vector<Pizza*> order;

    while (true)
    {
        cout << "1. Посмотреть меню" << endl;
        cout << "2. Выбрать пиццу" << endl;
        cout << "3. Распечатать заказ" << endl;
        cout << "4. Завершить заказ и распечатать итоговую сумму" << endl;
        cout << "Выберите действие (1-4): ";

        int choice, pizzaNumChoice, sizeChoice;
        cin >> choice;

        if (choice == 1)
        {
            displayMenu();
        }
        else if (choice == 2)
        {
            cout << "Введите номер пиццы из меню: ";
            do {
                cin >> pizzaNumChoice;
                if (!(pizzaNumChoice > 0 && pizzaNumChoice < 3)) {
                    cout << "Введите кореектный номер пиццы из меню: ";
                }
            } while (!(pizzaNumChoice > 0 && pizzaNumChoice < 3));

            cout << "Выберите размер пиццы (1-4):\n"
                 << "1. 25см\n2. 30см\n3. 35см\n4. 40см\n";
            do {
                cin >> sizeChoice;
                if (!(sizeChoice > 0 && sizeChoice < 5)) {
                    cout << "Введите кореектный номер размера пиццы из меню: ";
                }
            } while (!(sizeChoice > 0 && sizeChoice < 5));

            int numOfSalt = 0, numOfCheez = 0;
            cout << "Введите 0, если не хотите ничего добавлять к пицце" << endl;
            int saltAndCheez;
            do
            {
                cout << "Выберите, что добавить к пицце:\n"
                     << "1. Соль\n2. Сыр\n0. Завершить добавление\n";
                cin >> saltAndCheez;

                switch (saltAndCheez)
                {
                case 1:
                    numOfSalt++;
                    cout << "Вы добавили соль: " << numOfSalt << endl;
                    break;
                case 2:
                    numOfCheez++;
                    cout << "Вы добавили сыр: " << numOfCheez << endl;
                    break;
                case 0:
                    break;
                default:
                    cout << "Выберите верный вариант" << endl;
                    break;
                }
            } while (saltAndCheez != 0);

            Pizza* pizza = nullptr;
            if (pizzaNumChoice == 1)
                switch (sizeChoice)
                {
                case 1:
                    pizza = new Pipperoni(25);
                    break;
                case 2:
                    pizza = new Pipperoni(30);
                    break;
                case 3:
                    pizza = new Pipperoni(35);
                    break;
                case 4:
                    pizza = new Pipperoni(40);
                    break;
                default:
                    break;
                }
            else if (pizzaNumChoice == 2)
                switch (sizeChoice)
                {
                case 1:
                    pizza = new Margaritta(25);
                    break;
                case 2:
                    pizza = new Margaritta(30);
                    break;
                case 3:
                    pizza = new Margaritta(35);
                    break;
                case 4:
                    pizza = new Margaritta(40);
                    break;
                default:
                    break;
                }

            pizza->setSalt(numOfSalt);
            pizza->setCheez(numOfCheez);

            order.push_back(pizza);
        }
        else if (choice == 3)
        {
            displayOrder(order);
        }
        else if (choice == 4)
        {
            displayOrder(order);

            for (const auto& pizza : order)
            {
                delete pizza;
            }

            break;
        }
        else
        {
            cout << "Неправильное значение. Попробуйте ещё раз." << endl;
        }
    }

    return 0;
}