#include <array>
#include <print>
#include <fstream>

#include <array>
#include <random>
#include <stdexcept>
#include <string_view>

constexpr std::array<std::string_view, 15> surnames{
    "Иванов", "Смирнов", "Кузнецов", "Попов", "Васильев",
    "Петров", "Соколов", "Михайлов", "Новиков", "Фёдоров",
    "Морозов", "Волков", "Алексеев", "Лебедев", "Семёнов"};

constexpr std::array<std::string_view, 15> names{
    "Александр", "Дмитрий", "Максим", "Сергей", "Андрей",
    "Алексей", "Артём", "Илья", "Кирилл", "Михаил",
    "Никита", "Матвей", "Роман", "Егор", "Арсений"};

constexpr std::array<std::string_view, 15> patronymics{
    "Александрович", "Дмитриевич", "Сергеевич", "Андреевич", "Алексеевич",
    "Иванович", "Михайлович", "Николаевич", "Владимирович", "Петрович",
    "Павлович", "Викторович", "Олегович", "Игоревич", "Юрьевич"};

int main() {
    std::ofstream file{"input.txt"};
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> day_distr{1, 30};
    std::uniform_int_distribution<> month_distr{1, 12};
    std::uniform_int_distribution<> year_distr{2018, 2026};

    std::uniform_int_distribution<> name_distr{0, names.size() - 1};

    std::uniform_int_distribution<> id_distr{0, 20000};

    for (auto i = 0; i < 1'000'000; ++i) {
        const auto year = year_distr(gen);
        const auto month = month_distr(gen);
        const auto day = day_distr(gen);

        const auto& name = names[name_distr(gen)];
        const auto& surname = surnames[name_distr(gen)];
        const auto& patron = patronymics[name_distr(gen)];

        const auto id = id_distr(gen);

        file << std::format("{:02} {:02} {:02} {} {} {} {}", day, month, year, surname, name, patron, id) << std::endl;
    }

    return 0;
}
