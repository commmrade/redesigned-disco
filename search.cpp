#include <print>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <iostream>
#include <filesystem>

struct Entry {
    int day;
    int month;
    int year;

    std::string name;
    std::string surname;
    std::string patron;

    int id;

    int orig_row;

    friend auto operator<=>(const Entry& l, const Entry& r) {
        return std::tie(l.surname, l.name, l.patron, l.id) <=> std::tie(r.surname, r.name, r.patron, r.id);
    }
    friend auto operator==(const Entry& l, const Entry& r) {
        return l.surname == r.surname && l.name == r.name && l.patron == r.patron && l.id == r.id;
    }
};

std::pair<std::vector<int>, int> binary_search(const std::vector<Entry>& entries, const Entry& key) {
    // FIO, id
    int low = 0;
    int high = entries.size() - 1;

    int cnt = 0;
    while (low <= high) {
        const auto mid = low + (high - low) / 2;

        if ((++cnt, entries[mid] == key)) {
            int l = mid;
            while (l > 0 && (++cnt, entries[l - 1] == key)) {
                --l;
            }
            int r = mid;
            while (r + 1 < static_cast<int>(entries.size()) && (++cnt, entries[r + 1] == key)) {
                ++r;
            }

            std::vector<int> rows;
            for (int i = l; i <= r; ++i) {
                rows.push_back(i);
            }
            return {rows, cnt};
        }

        if ((++cnt, entries[mid] < key)) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return {{}, cnt};
}

std::pair<std::vector<int>, int> sentinel_search(std::vector<Entry>& entries, const Entry& key) {
    int cnt = 0;
    std::vector<int> rows;
    const int last = static_cast<int>(entries.size()) - 1;

    Entry saved = std::move(entries.back());
    entries.back() = key;

    int idx = 0;
    while (true) {
        while ((++cnt, !(entries[idx] == key))) {
            ++idx;
        }
        if (idx == last) {
            break;
        }
        rows.push_back(idx);
        ++idx;
    }

    entries.back() = std::move(saved);
    if ((++cnt, entries.back() == key)) {
        rows.push_back(last);
    }

    return {rows, cnt};
}

std::vector<Entry> read_entries(const std::filesystem::path& path, const int n) {
    std::ifstream file{path};
    if (!file.is_open()) {
        throw std::runtime_error("could not open the file");
    }

    std::vector<Entry> entries;
    entries.reserve(n);

    for (auto i = 0; i < n; ++i) {
        std::string line;
        std::getline(file, line);
        std::stringstream ss{line};
        Entry entry{};
        ss >> entry.day >> entry.month >> entry.year >> entry.surname >> entry.name >> entry.patron >> entry.id;
        entry.orig_row = i;
        entries.push_back(std::move(entry));
    }

    return entries;
}

int main() {
    int n;
    std::println("enter n:");
    std::cin >> n;

    Entry entry;
    std::println("Введите surname,name, patron., id:");
    std::cin >> entry.surname >> entry.name >> entry.patron >> entry.id;

    std::println("Binary search:");
    {
        const auto entries = read_entries("input.txt", n);
        const auto [row_idx, op_cnt] = binary_search(entries, entry);
        std::println("Results: row index - {}, operation count - {}", row_idx, op_cnt);
    }
    {
        const auto entries = read_entries("output_sort1.txt", n);
        const auto [row_idx, op_cnt] = binary_search(entries, entry);
        std::println("Results: row index - {}, operation count - {}", row_idx, op_cnt);
    }
    {
        const auto entries = read_entries("output_sort2.txt", n);
        const auto [row_idx, op_cnt] = binary_search(entries, entry);
        std::println("Results: row index - {}, operation count - {}", row_idx, op_cnt);
    }

    std::println("Sentinel search:");
    {
        auto entries = read_entries("input.txt", n);
        const auto [row_idx, op_cnt] = sentinel_search(entries, entry);
        std::println("Results: row index - {}, operation count - {}", row_idx, op_cnt);
    }
    {
        auto entries = read_entries("output_sort1.txt", n);
        const auto [row_idx, op_cnt] = sentinel_search(entries, entry);
        std::println("Results: row index - {}, operation count - {}", row_idx, op_cnt);
    }
    {
        auto entries = read_entries("output_sort2.txt", n);
        const auto [row_idx, op_cnt] = sentinel_search(entries, entry);
        std::println("Results: row index - {}, operation count - {}", row_idx, op_cnt);
    }

    return 0;
}
