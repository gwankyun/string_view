#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <lite/string_view.hpp>
#include <cstring>
#include <string_view>
#include <type_traits>

typedef lite::string_view string_view_t;
//typedef std::string_view string_view_t;

int main(int argc, char* argv[])
{
    doctest::Context context;

    context.applyCommandLine(argc, argv);

    context.setOption("no-breaks", true);

    int res = context.run();

    if (context.shouldExit())
    {
        return res;
    }

    context.clearFilters();

    return res;
}

TEST_CASE("string_view 1")
{
    string_view_t sv;

    CHECK(sv.data() == NULLPTR);
    CHECK(sv.size() == 0);
}

TEST_CASE("string_view 2")
{
    string_view_t sv;
    string_view_t sv1(sv);
    CHECK(sv.data() == sv1.data());
    CHECK(sv.size() == sv1.size());
}

TEST_CASE("string_view 3")
{
    std::string str("123");
    string_view_t sv(str.c_str(), str.size());

    CHECK(std::memcmp(sv.data(), "123", sv.size()) == 0);
}

TEST_CASE("string_view 4")
{
    const char* str = "123";
    string_view_t sv(str, std::char_traits<char>::length(str));
    CHECK(std::memcmp(sv.data(), "123", sv.size()) == 0);
}

TEST_CASE("empty")
{
    string_view_t sv;
    CHECK(sv.empty());
}

TEST_CASE("operator[]")
{
    string_view_t sv("123");
    CHECK(sv[1] == '2');
}

TEST_CASE("front")
{
    std::string str("123");
    string_view_t sv(str.c_str(), str.size());

    CHECK(sv.front() == '1');
}

TEST_CASE("back")
{
    std::string str("123");
    string_view_t sv(str.c_str(), str.size());

    CHECK(sv.back() == '3');
}

TEST_CASE("remove_prefix")
{
    std::string str("12345");
    string_view_t sv(str.c_str(), str.size());
    sv.remove_prefix(2);

    CHECK(std::memcmp(sv.data(), "345", sv.size()) == 0);
}

TEST_CASE("remove_suffix")
{
    std::string str("12345");
    string_view_t sv(str.c_str(), str.size());
    sv.remove_suffix(2);

    CHECK(std::memcmp(sv.data(), "123", sv.size()) == 0);
}

TEST_CASE("swap")
{
    string_view_t sv1("123");
    string_view_t sv2("456");

    CHECK(std::memcmp(sv1.data(), "123", sv1.size()) == 0);
    CHECK(std::memcmp(sv2.data(), "456", sv2.size()) == 0);
    sv1.swap(sv2);
    CHECK(std::memcmp(sv2.data(), "123", sv2.size()) == 0);
    CHECK(std::memcmp(sv1.data(), "456", sv1.size()) == 0);
}

TEST_CASE("copy")
{
    std::vector<char> v(10);
    std::vector<char> v2(10);

    {
        string_view_t sv("123");
        sv.copy(v.data(), sv.size());
        sv.copy(v2.data(), sv.size(), 1);
    }

    CHECK(std::memcmp(v.data(), "123", 3) == 0);
    CHECK(std::memcmp(v2.data(), "23", 2) == 0);
}

TEST_CASE("substr")
{
    string_view_t::size_type npos = 0;
    if constexpr (std::is_same_v<string_view_t, lite::string_view>)
    {
        npos = lite::string_view::npos();
    }
    else
    {
        npos = std::string_view::npos;
    }
    string_view_t sv1("12345");
    string_view_t sv2(sv1.substr(0, npos));
    CHECK(std::memcmp(sv2.data(), sv1.data(), sv1.size()) == 0);

    string_view_t sv3(sv1.substr(2, 2));
    CHECK(std::memcmp(sv3.data(), "34", sv3.size()) == 0);

    string_view_t sv4(sv1.substr(3, 10));
    CHECK(std::memcmp(sv4.data(), "45", sv3.size()) == 0);
}

TEST_CASE("compare")
{
    string_view_t sv("123");

    // 1
    CHECK(sv.compare(string_view_t("234")) < 0);
    CHECK(sv.compare(string_view_t("2345")) < 0);
    CHECK(sv.compare(string_view_t("123")) == 0);
    CHECK(sv.compare(string_view_t("12")) > 0);
    CHECK(sv.compare(string_view_t("012")) > 0);

    // 2
    CHECK(sv.compare(0, 2, string_view_t("23")) < 0);
    CHECK(sv.compare(0, 2, string_view_t("12")) == 0);
    CHECK(sv.compare(0, 2, string_view_t("01")) > 0);

    // 3
    CHECK(sv.compare(0, 2, string_view_t("234"), 0, 2) < 0);
    CHECK(sv.compare(1, 2, string_view_t("234"), 0, 2) == 0);
    CHECK(string_view_t("234").compare(0, 2, sv, 0, 2) > 0);

    // 4
    CHECK(sv.compare("2345") < 0);
    CHECK(sv.compare("123") == 0);
    CHECK(sv.compare("12") > 0);

    // 5
    CHECK(sv.compare(0, 3, "2345") < 0);
    CHECK(sv.compare(0, 3, "123") == 0);
    CHECK(sv.compare(0, 3, "12") > 0);

    // 6
    CHECK(sv.compare(0, 2, "234", 2) < 0);
    CHECK(sv.compare(1, 2, "234", 2) == 0);
    CHECK(string_view_t("234").compare(0, 2, "123", 2) > 0);
}

TEST_CASE("starts_with")
{
    string_view_t sv("123");

    CHECK(sv.starts_with(string_view_t("12"))); // 1
    CHECK(sv.starts_with('1')); // 2
    CHECK(sv.starts_with("12")); // 3
}

TEST_CASE("ends_with")
{
    string_view_t sv("123");

    CHECK(sv.ends_with(string_view_t("23"))); // 1
    CHECK(sv.ends_with('3')); // 2
    CHECK(sv.ends_with("23")); // 3
}

TEST_CASE("find")
{
    string_view_t sv("12345");

    CHECK(sv.find(string_view_t("34"), 1) == 2); // 1
    CHECK(sv.find('3', 1) == 2); // 2
    CHECK(sv.find("345", 1, 2) == 2); // 3
    CHECK(sv.find("34", 1) == 2); // 4
}

TEST_CASE("contains")
{
    string_view_t sv("12345");

    CHECK(sv.contains(string_view_t("34"))); // 1
    CHECK(sv.contains('3')); // 2
    CHECK(sv.contains("34")); // 3
}

TEST_CASE("rfind")
{
    string_view_t sv("123412");

    CHECK(sv.rfind(string_view_t("12")) == 4); // 1

    std::string_view sv_s("123412");
    CHECK(sv_s.rfind(std::string_view("12")) == 4); // 1
}

TEST_CASE("find_first_of")
{
    string_view_t sv("123412");

    CHECK(sv.find_first_of(string_view_t("12")) == 0); // 1
}

TEST_CASE("find_last_of")
{
    string_view_t sv("123412");

    CHECK(sv.find_last_of(string_view_t("12")) == 5); // 1

    std::string_view sv_s("123412");
    CHECK(sv_s.find_last_of(std::string_view("12")) == 5); // 1
}

TEST_CASE("find_first_not_of")
{
    string_view_t sv("123412");

    CHECK(sv.find_first_not_of(string_view_t("12")) == 2); // 1
}

TEST_CASE("operator=")
{
    string_view_t sv1("123");
    auto sv2 = sv1;
    CHECK(std::memcmp(sv2.data(), "123", sv2.size()) == 0);
}

TEST_CASE("operator== != < <= > >=")
{
    string_view_t sv1("123");
    string_view_t sv2("456");
    string_view_t sv3("123");
    CHECK((sv1 == sv3));
    CHECK((sv1 != sv2));
    CHECK((sv1 < sv2));
    CHECK((sv1 <= sv2));
    CHECK((sv2 > sv1));
    CHECK((sv2 >= sv1));
}
