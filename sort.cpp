#include <chrono>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <print>
#include <iostream>
#include <vector>

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
};

void merge(std::span<Entry> entries) {
    const auto size = entries.size();
    const int p1_size = size / 2;
    const int p2_size = size - size / 2;

    std::vector<Entry> p1(p1_size);
    std::vector<Entry> p2(p2_size);

    for (auto i = 0; i < p1_size; ++i) {
        p1[i] = entries[i];
    }
    for (auto i = 0; i < p2_size; ++i) {
        p2[i] = entries[size / 2 + i];
    }

    int p1_idx = 0;
    int p2_idx = 0;
    int merge_idx = 0;

    while (p1_idx < p1_size && p2_idx < p2_size) {
        if (p1[p1_idx] <= p2[p2_idx]) {
            entries[merge_idx] = p1[p1_idx];
            ++p1_idx;
        } else {
            entries[merge_idx] = p2[p2_idx];
            ++p2_idx;
        }
        ++merge_idx;
    }

    while (p1_idx < p1_size) {
        entries[merge_idx] = p1[p1_idx];
        ++p1_idx;
        ++merge_idx;
    }

    while (p2_idx < p2_size) {
        entries[merge_idx] = p2[p2_idx];
        ++p2_idx;
        ++merge_idx;
    }
}

void merge_sort_impl(std::span<Entry> entries) {
    if (entries.empty() || entries.size() == 1) {
        return;
    }

    int mid = entries.size() / 2;
    merge_sort_impl(entries.subspan(0, mid));
    merge_sort_impl(entries.subspan(mid));
    merge(entries);
}

std::vector<Entry> merge_sort(std::vector<Entry> entries) {
    merge_sort_impl(entries);
    return entries;
}

void tw_insert_sort_impl(std::vector<Entry>& entries) {
    const long n = static_cast<long>(entries.size());
    if (n < 2) return;

    std::vector<Entry> out(n);
    auto at = [&](long p) -> Entry& { return out[((p % n) + n) % n]; };

    at(0) = std::move(entries[0]);
    long left = 0;
    long right = 0;

    for (long i = 1; i < n; ++i) {
        Entry key = std::move(entries[i]);

        if (key < at(0)) {
            long slot = -left - 1;
            while (slot + 1 <= -1 && !(key < at(slot + 1))) {
                at(slot) = std::move(at(slot + 1));
                ++slot;
            }
            at(slot) = std::move(key);
            ++left;
        } else {
            long slot = right + 1;
            while (slot - 1 >= 1 && key < at(slot - 1)) {
                at(slot) = std::move(at(slot - 1));
                --slot;
            }
            at(slot) = std::move(key);
            ++right;
        }
    }

    long k = 0;
    for (long p = -left; p <= right; ++p)
        entries[k++] = std::move(at(p));
}

std::vector<Entry> tw_insert_sort(std::vector<Entry> entries) {
    tw_insert_sort_impl(entries);
    return entries;
}

std::vector<Entry> read_entries(const int n) {
    std::ifstream file{"input.txt"};
    if (!file.is_open()) {
        throw std::runtime_error("could not open the file");
    }

    std::vector<Entry> entries;
    entries.reserve(n);

    for (auto i = 0; i < n; ++i) {
        Entry entry{};
        file >> entry.day >> entry.month >> entry.year >> entry.surname >> entry.name >> entry.patron >> entry.id;
        entry.orig_row = i;
        entries.push_back(std::move(entry));
    }

    return entries;
}

void write_entries(const std::filesystem::path& path, const std::vector<Entry>& entries, const long time) {
    std::ofstream file{path};
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    for (const auto& entry : entries) {
        file << std::format("{:02} {:02} {:02} {} {} {} {} {}", entry.day, entry.month, entry.year, entry.surname, entry.name, entry.patron, entry.id, entry.orig_row) << std::endl;
    }
    file << time;
}

int main() {
    int n;
    std::println("enter n:");
    std::cin >> n;
    // ключ: ФИО, номер заявки

    auto entries = read_entries(n);

    {
        const auto start = std::chrono::high_resolution_clock::now();
        const auto sorted_entries = merge_sort(entries);
        const auto end = std::chrono::high_resolution_clock::now();
        write_entries("output_sort1.txt", sorted_entries, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    {
        const auto start = std::chrono::high_resolution_clock::now();
        const auto sorted_entries = tw_insert_sort(entries);
        const auto end = std::chrono::high_resolution_clock::now();
        write_entries("output_sort2.txt", sorted_entries, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }

    return 0;
}
