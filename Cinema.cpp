#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <tuple>

using namespace std;
using std::vector;

static double budget = 1000;

class Genre {
public:
    Genre(string genreName, int genrePopularity)
    {
        this->genreName = genreName;
        this->genrePopularity = genrePopularity;
    }

    string genreName;
    int genrePopularity =0;

    bool operator==(const Genre& rhs) const
    {
        return (genreName == rhs.genreName) && (genrePopularity == rhs.genrePopularity);
        // or, in C++11 (must #include <tuple>) 
        return tie(genreName, genrePopularity) == tie(rhs.genreName, rhs.genrePopularity); 
    }

    void Revival(double summ) {
        if (budget >= summ) {
            int increase = round(summ/15);
            this->genrePopularity += increase;
            if (genrePopularity < 100) {
                cout << "\nПопулярность " << genreName << " увеличилась на " << increase;
            }
            else {
                genrePopularity = 100;
                cout << "\nПопулярность " << genreName << " равна 100%";
            }
        }
        else
        {
            cout << "\nУ Вас не хватает денег для операции.";
        }
    }
};

class Audience
{
public:
    Audience(int age, bool gender, Genre favGenre, int loyalty) : favGenre(favGenre)
    {
        this->age = age;
        this->gender = gender;
        this->favGenre = favGenre;
        this->loyalty = loyalty;
    }

    int age =0;
    bool gender = false;
    int loyalty = 0;
    Genre favGenre;

    void Discount(double summ) {
        if (budget >= summ) {
            int increase = round(summ / 10);
            this->loyalty += increase;
            if (loyalty < 100) {
                cout << "\nЛояльнось ЦА увеличилась на " << increase;
            }
            else {
                loyalty = 100;
                cout << "\nЛояльнось ЦА равна 100%";
            }
        }
        else
        {
            cout << "\nУ Вас не хватает денег для операции.";
        }
    }
};

class Actor : public Audience
{
public:
    Actor(string name, bool ignore, int popularity, int age, bool gender, Genre favGenre, int loyalty) : Audience(age, gender, favGenre, loyalty)
    {
        this->name = name;
        this->ignore = ignore;
        this->popularity = popularity;
    }

    string name;
    bool ignore;
    int popularity;

    bool Invite(vector<Genre> genreList) {
        for ( Genre genre : genreList) {
            if (favGenre == genre) {
                cout << "Актёру не нравится один из жанров, он будет игнорировать вас до первого успешного проката";
                return false;
            }
        }
        return true;
    }
};

class Book {
public:
    Book(string title, vector<Genre> genreList, int ageRestriction, Audience targetAudience) : TargetAudience(targetAudience)
    {

        this->Title = title;
        this->GenreList = genreList;
        this->AgeRestriction = ageRestriction;
        this->TargetAudience = targetAudience;
    }

    string Title;
    vector<Genre> GenreList;
    int AgeRestriction;
    Audience TargetAudience;
};

class Movie : public Book
{
public:
    Actor mainActor;
    int successChance =0;

    Movie(string title, vector<Genre> genreList, int ageRestriction, Actor mainActor, Audience targetAudience) : Book(title, genreList, ageRestriction, targetAudience), mainActor(mainActor)
    {
        this->successChance = 0;
    };

    bool Screening(vector<Actor> actors) {
        int genrepop =0;
        int isMatch = 0;
        for (Genre genre : GenreList) {
            genrepop += genre.genrePopularity;
            if (genre == TargetAudience.favGenre) {
                isMatch = 100;
            }
        }
        successChance += round((TargetAudience.loyalty + isMatch + round((genrepop / GenreList.size() + mainActor.popularity)))/4);
        int random = rand() % 100;
        if (random < successChance) {
            return true;
        }
        else
        {
            return false;
        }
    }
};

static vector<Genre> genres;
static vector<Audience> audiences;
static vector<Actor> actors;
static vector<Book> books;
static vector<Movie> films;


static vector<Genre> book1list; static vector<Genre> book2list;
static vector<Genre> book3list; static vector<Genre> book4list;
static vector<Genre> book5list; static vector<Genre> book6list;
static vector<Genre> book7list; static vector<Genre> book8list;

void ReturnBudget() {
    cout << "\nВаш бюджет: " << budget;
}
int FilmCreate() {
    int i = 1;
    int choice;
    Book selectBook = books[0];
    Actor selectActor = actors[0];
    cout << "\nВыберете книгу, по которой будете снимать фильм\n";
    for (const auto& book : books) {
        cout <<"\n" << i << ". Название: " << book.Title << "\nЖанры: ";
        for (const auto& genre : book.GenreList) {
            cout << genre.genreName << ", ";
        }
        cout << "\nВозрастное ограничение: " << book.AgeRestriction << "+";
        i++;
    }
    try {
        cout << "\n";
        cin >> choice;
        selectBook = books[choice - 1];
    }
    catch (...) {
        cout << "\nВведите верное число";
        FilmCreate();
        return 0;
    }
    i = 1;

    cout << "\nВыберете главного актёра\n";
    for (const auto& actor : actors) {
        string sex;
        if (!actor.ignore){
            if (actor.gender) {
                sex = "жен";
            }
            else {
                sex = "муж";
            }
            cout << "\n" << i << ". Имя: " << actor.name << "  Популярность: " << actor.popularity << endl << "Пол: " << sex << "  Возраст: " << actor.age << endl << " Нелюбимый жанр: " << actor.favGenre.genreName;
            i++;
        }
    }
    try {
        cout << "\n";
        cin >> choice;
        if (actors[choice - 1].Invite(selectBook.GenreList)) {
            cout << "\nАктёр согласился";
            selectActor = actors[choice - 1];
        }
        else {
            actors[choice - 1].ignore = true;
            cout << "\nАктёр отказался, попробуйте снова.";
            FilmCreate();
            return 0;
        }
    }
    catch (...) {
        cout << "\nВведите верное число";
        FilmCreate();
        return 0;
    }
    Movie film{ selectBook.Title, selectBook.GenreList, selectBook.AgeRestriction, selectActor, selectBook.TargetAudience };
    films.push_back(film);
    cout << "\nВы создали сняли фильм. Чтобы повысить его успех или выпустить в прокат, перейдите в раздел показа фильма.";

    return 0;
}

int FilmMenu(Movie film) {
    int i = 1;
    int choice;
    string operation;
    Genre selectGenre = genres[0];
    Audience selectAudience = audiences[0];
    cout << "\nЧто вы хотите сделать?\n1. Повысить популярность одного из жанров\n2. Сделать скидку для ЦА\n3. Показать фильм\n";
    cin >> operation;
    if (operation == "1") {
        cout << "\nВыберете жанр, популярность которого хотите возродить\n";
        for (const auto& genre : film.GenreList) {
            cout <<"\n" << i << ". " << genre.genreName;
            i++;
        }
        try {
            cout << "\n";
            cin >> choice;
            selectGenre = film.GenreList[choice - 1];
            cout << "\nСколько вы хотите потратить на маркетинг?\n";
            double value;
            cin >> value;
            selectGenre.Revival(value);
        }
        catch (...) {
            cout << "\nВведите верное число";
            FilmMenu(film);
            return 0;
        }
    }
    else if (operation == "2") {
        try {
            selectAudience = film.TargetAudience;
            cout << "\nСколько вы хотите потратить на маркетинг?\n";
            double value;
            cin >> value;
            selectAudience.Discount(value);
        }
        catch (...) {
            cout << "\nВведите верное число";
            FilmMenu(film);
            return 0;
        }
    }
    else if (operation == "3") {
        if (film.Screening(actors)) {
            budget += 500;
            for (Actor actor : actors) {
                actor.ignore = false;
            }
            cout << film.Title << " завоевал огромную популярность, вы знатно заработали.\nФильм удалён из доступных к показу.";
            //films.erase(find(films.begin(), films.end(), film));
        }
        else {
            budget -= 250;
            cout << film.Title << " провалился в прокате, вы потеряли деньги.\nФильм удалён из доступных к показу.";
            //films.erase(find(films.begin(), films.end(), film));
        }
    }
    else {
        cout << "Введите число от 1 до 3";
        return 0;
    }

    return 0;
}

int main()
{
    setlocale(LC_ALL, "RUssian");

    Genre genre1{ "Вестерн", 10 };
    Genre genre2{ "Романтика", 80 };
    Genre genre3{ "Триллер", 70 };
    Genre genre4{ "Фэнтези", 40 };
    Genre genre5{ "Комедия", 50 };
    Genre genre6{ "Драма", 90 };
    Genre genre7{ "Ужасы", 70 };
    Genre genre8{ "Фантастика", 40 };
    Genre genre9{ "Автобиография", 50 };
    Genre genre10{ "Боевик", 90 };
    genres.push_back(genre1); genres.push_back(genre2); genres.push_back(genre3); genres.push_back(genre4); genres.push_back(genre5);
    genres.push_back(genre6); genres.push_back(genre7); genres.push_back(genre8); genres.push_back(genre9); genres.push_back(genre10);

    Audience audience1(16, true, genre2, 70);
    Audience audience2{ 40, true, genre6, 95 };
    Audience audience3{ 23, false, genre3, 40 };
    Audience audience4{ 56, false, genre5, 67 };
    audiences.push_back(audience1); audiences.push_back(audience2); audiences.push_back(audience3); audiences.push_back(audience4);

    Actor actor1{ "Джонни Депп", false, 95, 60, false, genre5, 100 };
    Actor actor2{ "Тимоти Шаламе", false, 78, 28, false, genre3, 100 };
    Actor actor3{ "Зендея", false, 69, 27, true, genre9, 100 };
    Actor actor4{ "Эмма Томпсон", false, 79, 65, true, genre7, 100 };
    Actor actor5{ "Джим Керри", false, 88, 62, false, genre1, 100 };
    actors.push_back(actor1); actors.push_back(actor2); actors.push_back(actor3); actors.push_back(actor4); actors.push_back(actor5);

    book1list.push_back(genre8); book1list.push_back(genre10);
    book2list.push_back(genre9); book2list.push_back(genre5);
    book3list.push_back(genre1); book3list.push_back(genre6); book3list.push_back(genre10);
    book4list.push_back(genre2); book4list.push_back(genre6);
    book5list.push_back(genre5); book5list.push_back(genre7);
    book6list.push_back(genre4); book6list.push_back(genre2);
    book7list.push_back(genre8); book7list.push_back(genre3); book7list.push_back(genre7);
    book8list.push_back(genre10); book8list.push_back(genre4); book8list.push_back(genre6);


    Book book1{ "Звезда на Марсе", book1list, 0, audience1 };
    Book book2{ "Во всём виноват Дима", book2list, 16, audience2 };
    Book book3{ "Ураган перекати-поле", book3list, 18, audience4 };
    Book book4{ "Покуда обещание в силе", book4list, 16, audience2 };
    Book book5{ "10 ошибок Боба Лафгуд", book5list, 12, audience4 };
    Book book6{ "Её Высочество Мия", book6list, 0, audience1 };
    Book book7{ "Дары плесени", book7list, 21, audience3 };
    Book book8{ "Мох", book8list, 21, audience3 };
    books.push_back(book1); books.push_back(book2); books.push_back(book3); books.push_back(book4); books.push_back(book5);
    books.push_back(book6); books.push_back(book7); books.push_back(book8);

    string choice;

    do {
        cout << "\n\nДобро пожаловать!\nВаша цель: успешно показать фильм. Обращайте внимание на соответствие в предпочтениях и популярность.\nЧто хотите сделать?";
        cout << "\n1. Создать фильм\n2. Узнать подробнее об Актёрах\n3. Узнать подробнее про ЦА\n4. Узнать подробнее про книги\n5. Узнать популярность жанров\n6. Узнать бюджет компании\n7. Показать фильм\n";
        cin >> choice;

        if (choice == "1") {
            FilmCreate();
        }
        else if (choice == "2") {
            string sex;
            string status;
            cout << "Список актеров:\n";
            for (const auto& actor : actors) {
                if (actor.gender) {
                    sex = "жен";
                }
                else {
                    sex = "муж";
                }
                if (actor.ignore) {
                    status = "игнорирует";
                }
                else {
                    status = "доступен";
                }
                cout << "\nИмя: " << actor.name << "  Популярность: " << actor.popularity << endl << "Пол: " << sex << "  Возраст: " << actor.age << endl << " Нелюбимый жанр: " << actor.favGenre.genreName << "  Статус: " << status << "\n";
            }
        }
        else if (choice == "3") {
            string sex;
            cout << "Список ЦА:\n";
            for (const auto& audit : audiences) {
                if (audit.gender) {
                    sex = "жен";
                }
                else {
                    sex = "муж";
                }
                cout << "\nСредний возраст: " << audit.age << "  Пол: " << sex << " Любимый жанр: " << audit.favGenre.genreName << "  Лояльность: " << audit.loyalty;
            }
        }
        else if (choice == "4") {
            for (const auto& book : books) {
                cout << "\nНазвание: " << book.Title << "\nЖанры: ";
                for (const auto& genre : book.GenreList) {
                    cout << genre.genreName << ", ";
                }
                cout << "\nВозрастное ограничение: " << book.AgeRestriction << "+";
            }
        }
        else if (choice == "5") {
            for (const auto& genre : genres) {
                cout << "\nНазвание: " << genre.genreName << " Популярность: " << genre.genrePopularity;
            }
        }
        else if (choice == "6") {
            ReturnBudget();
        }
        else if (choice == "7") {
            if (films.size() > 0) {
                int choice;
                int i =1;
                cout << "\nВыберете фильм для работы\n";
                for (Movie film : films) {
                    cout<< "\n" << i << ". " << film.Title;
                    i++;
                }
                try {
                    cout << "\n";
                    cin >> choice;
                    FilmMenu(films[choice - 1]);
                }
                catch (...) {
                    cout << "\nВведите верное число";
                }
            }
            else {
                cout << "Вы не сняли ни одного фильма";
            }
        }
        else {
            cout << "Введите число от 1 до 7";
        }

    } while (true);

    return 0;
}